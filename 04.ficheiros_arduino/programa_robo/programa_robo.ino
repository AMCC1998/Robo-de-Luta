#include <IBusBM.h>
#include <Servo.h>

IBusBM IBus;

Servo esc;
const int motorArma = 3;
const int velocidadeMinima = 1000;

// Motor A
int pwmA = 5;
int dirA = A0;

// Motor B
int pwmB = 6;
int dirB = A1;

void ativaArma(int velocidadeArma = velocidadeMinima);
void desativaArma();

void moverFrente(int velocidade);
void moverTras(int velocidade);
void virarEsquerda(int velocidade);
void virarDireita(int velocidade);
void parar();

void setup() {
  Serial.begin(115200);

  pinMode(pwmA, OUTPUT);
  pinMode(dirA, OUTPUT);

  pinMode(pwmB, OUTPUT);
  pinMode(dirB, OUTPUT);

  parar();
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
  pwm = pwm > 255 ? 255 : pwm;
  return pwm;
}

void loop() {
  parar();
  IBus.loop();

  int acel = mapChannelToPWM(IBus.readChannel(2));  // aceleração -255 a 255
  int dir = mapChannelToPWM(IBus.readChannel(3));   // direção -255 a 255

  //Positivar a velocidade
  int velocidade = acel < 0 ? acel * -1 : acel;

  if (dir < -30) {
    velocidade = dir < 0 ? dir * -1 : dir;
    virarEsquerda(velocidade);
  } else if (dir > 30) {
    velocidade = dir < 0 ? dir * -1 : dir;
    virarDireita(velocidade);
  } else if (acel > 10) {
    moverFrente(velocidade);
  } else if (acel < -10) {  // garante os limites
    moverTras(velocidade);
  } else {
    parar();
  }

  // Controle da arma no canal 3 (index 2)
  uint16_t canalArma = IBus.readChannel(4);
  if (canalArma > 1500) {
    ativaArma(1200);
  } else {
    desativaArma();
  }


  // Controle da arma no canal 7 servo Virar o robo
  uint16_t canalServo = IBus.readChannel(6);
  //Serial.print("Canal servo: ");
  //Serial.println(canalServo);
  if (canalServo > 2005) {
    //ativaArma(1200);
  } else {
    //desativaArma();
  }


  Serial.print("Acel: ");
  Serial.print(acel);
  Serial.print("\tDir: ");
  Serial.print(dir);
  Serial.print("\tArma: ");
  Serial.println(canalArma);

  delay(10);
}
