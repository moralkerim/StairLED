#include <FastLED.h> //Kütüphane import 
#include "NewPing.h"

#define DEBUG

#define NUM_LEDS 4 //Led Sayısı
#define DATA_PIN 7 //Led Din giriş pini
#define TRIGGER_PIN 9
#define ECHO_PIN 10

#define MAX_DISTANCE 400
#define TOGGLE_RATE 2000
#define MES_RATE 100
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float duration, distance;

CRGB leds[NUM_LEDS]; //Led Dizisi oluşturduk
int8_t last_led = 0;
bool out_of_range;
unsigned long toggle_time, mes_time;

typedef enum LED_STATE
{
  turn_on,
  turn_off,
  idle
};

LED_STATE led_state = turn_on;
LED_STATE prev_state = led_state;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS); //Fastlede led dizimizi ekledik
  FastLED.setBrightness(255); //Parlaklık ayarı 0-255
  TurnOffLeds();
}
void loop() {
  MeasureDistance();

  if (!out_of_range) {
    if (distance < 100 && millis() - toggle_time > TOGGLE_RATE) {
      toggle_time = millis();
      ToggleLED();
    }
  }

  switch (led_state) {
    case turn_on:
      LedSlide();
      break;

    case turn_off:
      LedSlideOff();
      break;

    default:
      break;

  }

#ifdef DEBUG
  Serial.print("last_led = ");  Serial.println(last_led);
  Serial.print("led_state = "); Serial.println(led_state);
  Serial.print("distance = ");  Serial.println(distance);
  Serial.println("--------------");
#endif


}

void LedSlide() {
  for (int i = 0; i < NUM_LEDS; i++) {
    FadeIn(0, 255, i);
    //    leds[i].setRGB(255, 255, 255); //sonraki 5 ledi
    //    FastLED.show();
    //delay(100);

  }
  prev_state = led_state;
  led_state = idle;

}

void LedSlideOff() {
  for (int i = NUM_LEDS - 1; i > -1; i--) {
    FadeOut(255, 0, i);
    //    leds[i].setRGB(0, 0, 0); //sonraki 5 ledi
    //    FastLED.show();
    //delay(100);

  }
  prev_state = led_state;
  led_state = idle;
}

void FadeIn(uint8_t start_val, uint8_t end_val, uint8_t led) {
  for (int i = start_val; i <= end_val; i = i + 7) {
    leds[led].setRGB(i, i, i); //sonraki 5 ledi
    FastLED.show();
    delayMicroseconds(1);

  }

  leds[led].setRGB(end_val, end_val, end_val); //sonraki 5 ledi
  FastLED.show();
}

void FadeOut(uint8_t start_val, uint8_t end_val, uint8_t led) {
  for (int i = start_val; i >= end_val; i = i - 7) {
    leds[led].setRGB(i, i, i); //sonraki 5 ledi
    FastLED.show();
    delayMicroseconds(1);

  }
  leds[led].setRGB(end_val, end_val, end_val); //sonraki 5 ledi
  FastLED.show();
}

void CheckOnOff() {
  switch (led_state) {
    case turn_on:
      last_led++;
      if (last_led == NUM_LEDS) {
        led_state = turn_off;
        last_led--;
        delay(1000);
      }
      break;
    case turn_off:
      last_led--;
      if (last_led == -1) {
        led_state = turn_on;
        last_led++;
        delay(1000);
      }
      break;
  }
#ifdef DEBUG
  Serial.print("last_led = "); Serial.println(last_led);
  Serial.print("led_state = "); Serial.println(led_state);
  Serial.println("--------------");
#endif
}

void ToggleLED() {
  switch (prev_state) {
    case turn_on:
      led_state = turn_off;
      break;

    case turn_off:
      led_state = turn_on;
      break;

    default:
      break;
  }
}


void MeasureDistance() {
  if (millis() - mes_time > MES_RATE) {
    mes_time = millis();
    distance = sonar.ping_cm();
    //Serial.println(distance);
    if (distance >= 400 || distance <= 2)
    {
      out_of_range = true;
    }

    else {
      out_of_range = false;
    }
  }

}

void TurnOffLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0); //sonraki 5 ledi
  }
  FastLED.show();
  delay(100);

}
