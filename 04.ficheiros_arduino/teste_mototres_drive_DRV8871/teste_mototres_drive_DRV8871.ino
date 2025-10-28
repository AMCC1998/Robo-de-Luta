// Motor A
int pwmA = 5;    
int dirA = A0;   

// Motor B
int pwmB = 6;    
int dirB = A1;   

void setup() {
  pinMode(pwmA, OUTPUT);
  pinMode(dirA, OUTPUT);

  pinMode(pwmB, OUTPUT);
  pinMode(dirB, OUTPUT);
}

void loop() {
  // --- Motor A frente, Motor B frente ---
  digitalWrite(dirA, LOW);
  analogWrite(pwmA, 200);  // ~78% velocidade
  digitalWrite(dirB, LOW);
  analogWrite(pwmB, 200);
  delay(2000);

  // --- Motor A trás, Motor B trás ---
  digitalWrite(dirA, HIGH);
  analogWrite(pwmA, 200);
  digitalWrite(dirB, HIGH);
  analogWrite(pwmB, 200);
  delay(2000);

  // --- Reduz velocidade gradualmente ---
  for (int v = 200; v >= 0; v -= 20) {
    analogWrite(pwmA, v);
    analogWrite(pwmB, v);
    delay(200);
  }

  // --- Parar os motores ---
  analogWrite(pwmA, 0);
  analogWrite(pwmB, 0);
  delay(2000);
}
