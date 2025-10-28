String version = "1.0";

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#include "fonts/TimesNewRoman13.h"
#include "fonts/TimesNewRoman16.h"
#include "fonts/Stang5x7.h"



// ********************** Ecra **************************
SSD1306AsciiWire oled;

const uint8_t* currentFont;

void clearCenteredText(const char* text, int row, int charWidth = 6, int startCol = -1);
void clearLine(const char* text, int row, int charWidth = -1, int startCol = -1);
// *******************************************************




// Joystick 1 (esquerdo)
const int VRy1 = A1;  // Frente/Trás
const int VRx1 = A0;  // Esquerda/Direita
const int SW1 = 9;    // Botão joystick 1

// Joystick 2 (direito)
const int VRy2 = A3;  // Frente/Trás
const int VRx2 = A2;  // Esquerda/Direita
const int SW2 = 8;    // Botão joystick 2

const int limiar = 100;
const int centro = 512;

String comando = "";
String ultimoComando = "";

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(1000);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  Wire.setClock(400000);

  //Ecra
  // Use next line if no RST_PIN or reset is not required.
  Wire.begin();                       // Inicializa I2C hardware
  oled.begin(&Adafruit128x64, 0x3C);  // Endereço I2C padrão
  oled.clear();

  screenSetFont(Stang5x7);
  oled.setCursor(0, 0);
  oled.print("V: " + version);

  oled.setCursor(70, 0);
  oled.print("Loading...");

  screenSetFont(TimesNewRoman16);
  printCentered("Bem Vindo!", 3);

  screenSetFont(Stang5x7);
  printCentered("amcc_projects 2025", 7);

  delay(2000);

  oled.clear();
  screenSetFont(Stang5x7);
  oled.setCursor(0, 0);
  oled.print("V: " + version);
  oled.setCursor(81, 0);
  oled.print("Bat: 60%");
  screenSetFont(TimesNewRoman16);
  printCentered(("Send: " + comando).c_str(), 3);
}

void loop() {


  int y1 = analogRead(VRy1);
  int x1 = analogRead(VRx1);
  int y2 = analogRead(VRy2);
  int x2 = analogRead(VRx2);

  // --- Joystick 1 (esquerdo) ---
  if (y1 < centro - limiar) {
    comando = "1T" + String(1023 - y1);
  } else if (y1 > centro + limiar) {
    comando = "1F" + String(y1);
  }

  if (x1 < centro - limiar) {
    comando = "1D" + String(1023 - x1);  // Direita
  } else if (x1 > centro + limiar) {
    comando = "1E" + String(x1);  // Esquerda
  }

  if (digitalRead(SW1) == LOW) {
    comando = "J1";
    delay(100);  // Joystick 1 pressionado
  }

  // --- Joystick 2 (direito) ---
  if (y2 < centro - limiar) {
    comando = "2T" + String(1023 - y2);
  } else if (y2 > centro + limiar) {
    comando = "2F" + String(y2);
  }

  if (x2 < centro - limiar) {
    comando = "2D" + String(1023 - x2);
  } else if (x2 > centro + limiar) {
    comando = "2E" + String(x2);
  }

  if (digitalRead(SW2) == LOW) {
    comando = "J2";
    delay(100);  // Joystick 2 pressionado
  }

  Serial1.print(comando);



  if (comando != ultimoComando) {
    int charWidth = 6;  // ou 10 se for TimesNewRoman16
    String texto = "Send: " + comando;

    clearLine(texto.c_str(), 3, charWidth, 60);
    printCentered(texto.c_str(), 3);
  }

  ultimoComando = comando;
  delay(100);
}
