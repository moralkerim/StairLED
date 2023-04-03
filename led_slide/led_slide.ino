#include <FastLED.h> //Kütüphane import 

#define DEBUG

#define NUM_LEDS 4 //Led Sayısı
#define DATA_PIN 7 //Led Din giriş pini
CRGB leds[NUM_LEDS]; //Led Dizisi oluşturduk
int8_t last_led = 0;

typedef enum LED_STATE
{
  turn_on = 0,
  turn_off = 1
};

LED_STATE led_state = turn_on;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS); //Fastlede led dizimizi ekledik
  FastLED.setBrightness(255); //Parlaklık ayarı 0-255
  TurnOffLeds();
}
void loop() {
  if (led_state == turn_on) {
    LedSlide();
  }

  else if (led_state == turn_off) {
    LedSlideOff();
  }
  CheckOnOff();


  delay(100);
}

void LedSlide() {
  leds[last_led].setRGB(255, 255, 255); //sonraki 5 ledi
  FastLED.show();
}

void LedSlideOff() {
  leds[last_led].setRGB(0, 0, 0); //sonraki 5 ledi
  FastLED.show();
}

void CheckOnOff() {
  switch (led_state) {
    case turn_on:
      last_led++;
      if (last_led == NUM_LEDS) {
        led_state = turn_off;
        last_led--;
      }
      break;
    case turn_off:
      last_led--;
      if (last_led == -1) {
        led_state = turn_on;
        last_led++;
      }
      break;
  }

#ifdef DEBUG
  Serial.print("last_led = "); Serial.println(last_led);
  Serial.print("led_state = "); Serial.println(led_state);
  Serial.println("--------------");
#endif
}

void TurnOffLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0); //sonraki 5 ledi
  }
  FastLED.show();
  delay(100);

}
