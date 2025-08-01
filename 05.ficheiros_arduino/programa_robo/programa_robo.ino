#include <IBusBM.h>
#include <Servo.h>

IBusBM IBus;

Servo esc;
const int motorArma = 3;
const int velocidadeMinima = 1000;

const int IN1 = 5;
const int IN2 = 6;
const int IN3 = 7;
const int IN4 = 4;
const int ENA = 10;
const int ENB = 9;

void ativaArma(int velocidadeArma = velocidadeMinima);
void desativaArma();

void moverFrente(int velocidade);
void moverTras(int velocidade);
void virarEsquerda(int velocidade);
void virarDireita(int velocidade);
void parar();

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  esc.attach(motorArma);
  esc.writeMicroseconds(1200);
  delay(200);
  esc.writeMicroseconds(0);

  IBus.begin(Serial1);
  Serial.println("Inicializacao completa");
  parar();
}

// Mapeia canal iBUS (1000-2000) para valor entre -255 e 255 com zona morta
int mapChannelToPWM(uint16_t val) {
  int pwm = map(val, 1000, 2000, -255, 255);
  if (abs(pwm) < 20) pwm = 0;  // zona morta
  return pwm;
}

void loop() {
  IBus.loop();

  int acel = mapChannelToPWM(IBus.readChannel(2));  // aceleração -255 a 255
  int dir = mapChannelToPWM(IBus.readChannel(3));   // direção -255 a 255

  if (dir < -10) {
    virarEsquerda(255);
  } else if (dir > 10) {
    virarDireita(255);
  } else if (acel > 100) {
    moverFrente(255);
  } else if (acel < -100) {
    moverTras(255);
  } else {
    parar();
  }

  // Controle da arma no canal 3 (index 2)
  uint16_t canalArma = IBus.readChannel(4);
  if (canalArma > 1500) {
    ativaArma(1100);
  } else {
    desativaArma();
  }


  Serial.print("Acel: ");
  Serial.print(acel);
  Serial.print("\tDir: ");
  Serial.print(dir);
  Serial.print("\tArma: ");
  Serial.println(canalArma);

  //delay(50);
}
