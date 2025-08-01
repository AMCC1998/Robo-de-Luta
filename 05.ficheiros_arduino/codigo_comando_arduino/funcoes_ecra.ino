
void printCentered(const char* text, int row) {
  int charWidth = 2;
  if (currentFont == TimesNewRoman16) {
    charWidth = 2;
  } else {
    charWidth = 3;
  }


  int len = strlen(text);

  int screenWidth = 128;  // largura total do ecrã em pixels
  int textPixelWidth = len * charWidth;

  int startPixel = (screenWidth - textPixelWidth) / 2;
  if (startPixel < 0) startPixel = 0;

  int startCol = startPixel / charWidth;

  /*Serial.print("startCol: ");
  Serial.print(startCol);
  Serial.print(" | row: ");
  Serial.println(row);*/

  oled.setCursor(startCol, row);
  oled.print(text);
}

void screenSetFont(const uint8_t* font) {
  oled.setFont(font);
  currentFont = font;
}

void clearCenteredText(const char* text, int row, int charWidth, int startCol) {

  if (currentFont == TimesNewRoman16) {
    charWidth = 2;
  } else {
    charWidth = 3;
  }

  int len = strlen(text);
  int screenWidth = 128;
  int textWidthPx = len * charWidth;
  if (startCol == -1) {
    int startPixel = (screenWidth - textWidthPx) / 2;
    if (startPixel < 0) startPixel = 0;
    startCol = startPixel / charWidth;
    int totalCols = 128 / charWidth;
  }


  oled.setCursor(startCol, row);
  for (int i = 0; i < len; i++) {
    oled.print(' ');
  }
}

void clearLine(const char* text, int row, int charWidth = -1, int startCol = -1) {
  // Determinar a largura do caractere com base na fonte atual se não for passada
  if (charWidth == -1) {
    if (currentFont == TimesNewRoman16) {
      charWidth = 10; // Largura aproximada dos caracteres TimesNewRoman16
    } else {
      charWidth = 6; // Largura padrão para System5x7 ou similar
    }
  }

  int len = strlen(text);
  int screenWidth = 128;
  int textWidthPx = len * charWidth;

  // Se startCol não foi especificado, calcula para centralizar
  if (startCol == -1) {
    int startPixel = (screenWidth - textWidthPx) / 2;
    if (startPixel < 0) startPixel = 0;
    startCol = startPixel / charWidth;
  }

  // Posiciona o cursor e apaga cada caractere
  oled.setCursor(startCol, row);
  for (int i = 0; i < len; i++) {
    oled.print(' ');
  }
}