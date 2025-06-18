void setup() {
  Serial1.begin(9600);
}

void loop() {
  Serial1.println("Olá do Arduino");
  delay(2000); // envia a cada 2 segundos
}
