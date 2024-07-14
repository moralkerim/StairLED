#include <FastLED.h>  //Kütüphane import
#include "NewPing.h"

#define DEBUG

#define NUM_LEDS 1400  //Led Sayısı
#define DATA_PIN 4    //Led Din giriş pini

#define S1_PIN 2
#define S2_PIN 3

#define FADE_NUM 5

#define TOGGLE_RATE 2000
#define MES_RATE 100
#define INT_INTERVAL 5000

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

void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(S1_PIN), S1_INT, RISING);
  attachInterrupt(digitalPinToInterrupt(S2_PIN), S2_INT, RISING);
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);  //Fastlede led dizimizi ekledik
  FastLED.setBrightness(255);                              //Parlaklık ayarı 0-255
  TurnOffLeds();
}

void loop() {

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

  if (millis() - s1_time > INT_INTERVAL) {



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

    s1_time = millis();
    //d//delay2000);
  }
}

void S2_INT() {

  if (millis() - s2_time > INT_INTERVAL) {

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

    s2_time = millis();
  }
  //de//delay000);
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
