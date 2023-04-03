#include <FastLED.h> //Kütüphane import 
#define NUM_LEDS 4 //Led Sayısı
#define DATA_PIN 7 //Led Din giriş pini
CRGB leds[NUM_LEDS]; //Led Dizisi oluşturduk
uint8_t last_led = 0;

void setup() {
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS); //Fastlede led dizimizi ekledik
  FastLED.setBrightness(255); //Parlaklık ayarı 0-255
  TurnOffLeds();
}
void loop() {
  LedSlide();

}

void LedSlide() {
  leds[last_led].setRGB(255, 255, 255); //sonraki 5 ledi
  FastLED.show();
  delay(100);
  last_led++;

  if (last_led == NUM_LEDS) {
    last_led = 0;
    TurnOffLeds();
  }
}

void TurnOffLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0); //sonraki 5 ledi
  }
  FastLED.show();
  delay(100);

}
