#include <FastLED.h>  //Kütüphane import
#include "NewPing.h"

#define DEBUG

#define NUM_LEDS 1400  //Led Sayısı
#define DATA_PIN 4    //Led Din giriş pini


#define TRIGGER_PIN2 3
#define ECHO_PIN2 7

#define TRIGGER_PIN1 2
#define ECHO_PIN1 6

#define FADE_NUM 7

#define TOGGLE_RATE 2000
#define MES_RATE 100
#define INT_INTERVAL 5000
#define MAX_DISTANCE 400

CRGB leds[NUM_LEDS];  //Led Dizisi oluşturduk
int8_t last_led = 0;
unsigned long s1_time, s2_time;
long print_time;


typedef enum LED_STATE {
  acik,
  kapali,
  asagi_acil,
  asagi_kapan,
  yukari_acil,
  yukari_kapan
};

LED_STATE led_state = kapali;

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
float duration1, distance1;
float duration2, distance2;

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);  //Fastlede led dizimizi ekledik
  FastLED.setBrightness(130);                              //Parlaklık ayarı 0-255
  TurnOffLeds();
}


void loop() {

  distance1 = sonar1.ping_cm();
  distance2 = sonar2.ping_cm();

  Serial.print("Distance1:"); Serial.println(distance1);
  Serial.print("Distance2:"); Serial.println(distance2);
  Serial.println("------------------------------");

  if (distance1 < 60 && distance1 > 10) {
    S1_INT();
  }

  else if(distance2 < 90 && distance2 > 10) {
    S2_INT();
  }

  //  switch (led_state) {
  //    case acik:
  //      led_state = acik;
  //      break;
  //
  //    case kapali:
  //      led_state = kapali;
  //      break;
  //  }


  if (millis() - print_time > 2000) {
    Serial.println(led_state);
    print_time = millis();
  }
}

void S1_INT() {


    Serial.println("S1 İnt.");
    Serial.println("-------");
    if (led_state == acik || led_state == kapali) {


      switch (led_state) {
        case acik:
          led_state = asagi_kapan;
          AsagiKapan();
          //TurnOffLeds();
          led_state = kapali;
          break;

        case kapali:
          led_state = yukari_acil;
          YukariAcil();
          //TurnOnLeds();
          led_state = acik;
          break;
      }

    }
}

void S2_INT() {

    Serial.println("S2 İnt.");
    Serial.println("-------");
    if (led_state == acik || led_state == kapali) {

      switch (led_state) {
        case acik:
          led_state = yukari_kapan;
          YukariKapan();
          //TurnOffLeds();
          led_state = kapali;
          break;

        case kapali:
          led_state = asagi_acil;
          AsagiAcil();
          //TurnOnLeds();
          led_state = acik;
          break;
      }
    }

}

void YukariAcil() {
  // for (int i = 0; i < NUM_LEDS; i++) {
  //   FadeIn(0, 255, i);
  //   //last_led++;
  //   //    leds[i].setRGB(255, 255, 255); //sonraki 5 ledi
  //   //    FastLED.show();
  //   //delay(100);
  // }
  Serial.println("Yukarı açılıyor.");
  int count1 = 1;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
    //leds[i].setRGB(255, 255, 30);
    count1++;
    if (count1 == FADE_NUM) {
      FastLED.show();
      count1 = 1;
    }

    //  delayMicroseconds(1);
  }
}

void AsagiAcil() {
  int count = 1;
  Serial.println("Aşağı Açılıyor.");
  int count2 = 1;
  for (int i = NUM_LEDS - 1; i > -1; i--) {
    leds[i] = CRGB::White; 
    //leds[i].setRGB(253, 251, 155);
    count2++;
    if (count2 == FADE_NUM) {
      FastLED.show();
      count2 = 1;
    }

    //delayMicroseconds(1);
  }
}

void AsagiKapan() {
  Serial.println("Aşağı kapanıyor.");
  int count3 = 1;
  for (int i = NUM_LEDS - 1; i > -1; i--) {
    leds[i] = CRGB::Black;
    count3++;
    if (count3 == FADE_NUM) {
      FastLED.show();
      count3 = 1;
    }

    //  delayMicroseconds(1);
  }

  TurnOffLeds();
}

void YukariKapan() {
  Serial.println("Yukarı Kapanıyor.");
  int count4 = 1;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    count4++;
    if (count4 == FADE_NUM) {
      FastLED.show();
      count4 = 1;
    }

    //delayMicroseconds(1);
  }

  TurnOffLeds();
}

void FadeIn(uint8_t start_val, uint8_t end_val, uint8_t led) {
  for (int i = start_val; i <= end_val; i = i + FADE_NUM) {
    leds[led].setRGB(i, i, i);  //sonraki 5 ledi
    FastLED.show();
    //delayMicroseconds(1);
  }

  leds[led].setRGB(end_val, end_val, end_val);  //sonraki 5 ledi
  FastLED.show();
}

void FadeOut(uint8_t start_val, uint8_t end_val, uint8_t led) {
  for (int i = start_val; i >= end_val; i = i - FADE_NUM) {
    leds[led].setRGB(i, i, i);  //sonraki 5 ledi
    FastLED.show();
    //delayMicroseconds(1);
  }
  leds[led].setRGB(end_val, end_val, end_val);  //sonraki 5 ledi
  FastLED.show();
}



void TurnOffLeds() {
  last_led = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);  //sonraki 5 ledi
  }
  FastLED.show();
  delay(100);
}

void TurnOnLeds() {
  last_led = NUM_LEDS;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(255, 255, 255);  //sonraki 5 ledi
  }
  FastLED.show();
  delay(100);
}
