// Joystick 1 (esquerdo)
const int VRy1 = A1;  // Frente/Trás
const int VRx1 = A0;  // Esquerda/Direita
const int SW1  = 3;   // Botão joystick 1

// Joystick 2 (direito)
const int VRy2 = A3;  // Frente/Trás
const int VRx2 = A2;  // Esquerda/Direita
const int SW2  = 2;   // Botão joystick 2

const int limiar = 100;
const int centro = 512;

void setup() {
  Serial1.begin(9600);

  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
}

void loop() {
  int y1 = analogRead(VRy1);
  int x1 = analogRead(VRx1);
  int y2 = analogRead(VRy2);
  int x2 = analogRead(VRx2);

  // --- Joystick 1 (esquerdo) ---
  if (y1 < centro - limiar) {
    Serial1.println("1T" + String(1023 - y1));
  } else if (y1 > centro + limiar) {
    Serial1.println("1F" + String(y1));
  }

  if (x1 < centro - limiar) {
    Serial1.println("1D" + String(1023 - x1));  // Direita
  } else if (x1 > centro + limiar) {
    Serial1.println("1E" + String(x1));         // Esquerda
  }

  if (digitalRead(SW1) == LOW) {
    Serial1.println("J1");
    delay(100);  // Joystick 1 pressionado
  }

  // --- Joystick 2 (direito) ---
  if (y2 < centro - limiar) {
    Serial1.println("2T" + String(1023 - y2));
  } else if (y2 > centro + limiar) {
    Serial1.println("2F" + String(y2));
  }

  if (x2 < centro - limiar) {
    Serial1.println("2D" + String(1023 - x2));
  } else if (x2 > centro + limiar) {
    Serial1.println("2E" + String(x2));
  }

  if (digitalRead(SW2) == LOW) {
    Serial1.println("J2");
    delay(100);  // Joystick 2 pressionado
  }

  delay(100);
}
