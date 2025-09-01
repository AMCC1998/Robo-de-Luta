#include <Adafruit_NeoPixel.h>

#define LED_PIN A0

#define NUM_LEDS 8  // Número de LEDs na fita

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
}

void loop() {

  for (int i = 1; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255));  // Azul
    strip.show();
    i++;
  }
  delay(200);
  strip.clear();
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));  // Azul
    strip.show();
    i++;
    //delay(100);
  }
  delay(200);
  strip.clear();
/*
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 255));  // Azul
    strip.show();
    delay(100);
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));  // Azul
    strip.show();
    delay(100);
  }*/
}
