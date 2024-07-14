//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER


#include <FastLED.h> //Kütüphane import 

#define DEBUG

#define NUM_LEDS 600 //Led Sayısı
#define DATA_PIN 4 //Led Din giriş pini
#define TRIGGER_PIN 9
#define ECHO_PIN 10

#define MAX_DISTANCE 400
#define TOGGLE_RATE 2000
#define MES_RATE 100
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


void FadeIn(uint8_t start_val, uint8_t end_val, uint8_t led) {
  for (int i = start_val; i <= end_val; i = i + 7) {
    leds[led].setRGB(i, i, i); //sonraki 5 ledi
    FastLED.show();
    delayMicroseconds(1);

  }

  leds[led].setRGB(end_val, end_val, end_val); //sonraki 5 ledi
  FastLED.show();
}

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS); //Fastlede led dizimizi ekledik
  FastLED.setBrightness(255); //Parlaklık ayarı 0-255
}
void loop() {
  LedSlide();

  }


