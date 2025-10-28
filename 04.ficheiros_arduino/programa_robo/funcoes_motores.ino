
// Funções de movimento
void moverFrente(int velocidade) {
  digitalWrite(dirA, LOW);
  digitalWrite(dirB, LOW);
  analogWrite(pwmA, velocidade);
  analogWrite(pwmB, velocidade);
}

void moverTras(int velocidade) {
  velocidade = 255 - constrain(velocidade, 0, 255);  // inverte
  digitalWrite(dirA, HIGH);
  digitalWrite(dirB, HIGH);
  analogWrite(pwmA, velocidade);
  analogWrite(pwmB, velocidade);
}

void virarEsquerda(int velocidade) {
  digitalWrite(dirA, HIGH);
  digitalWrite(dirB, LOW);
  analogWrite(pwmB, velocidade);
  velocidade = 255 - constrain(velocidade, 0, 255);  // inverte
  analogWrite(pwmA, velocidade);
}

void virarDireita(int velocidade) {
  digitalWrite(dirA, LOW);
  digitalWrite(dirB, HIGH);
  analogWrite(pwmA, velocidade);
  velocidade = 255 - constrain(velocidade, 0, 255);  // inverte
  analogWrite(pwmB, velocidade);
}

void parar() {
  digitalWrite(dirB, LOW);
  analogWrite(pwmB, 0);
  analogWrite(pwmA, 0);
  digitalWrite(dirA, LOW);
}
