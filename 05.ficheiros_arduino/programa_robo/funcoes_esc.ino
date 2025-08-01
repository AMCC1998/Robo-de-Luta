


void ativaArma(int velocidadeArma = velocidadeMinima) {
  esc.writeMicroseconds(velocidadeArma);
}

void desativaArma() {
  esc.writeMicroseconds(velocidadeMinima);
}


/*

#include <Servo.h>

Servo esc;
String inputString = "";
int pwm = 1000;


void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Inicio");
  esc.attach(9);                // Substitua com o pino de controle usado
  esc.writeMicroseconds(1200);  // Envia sinal MÁXIMO (Throttle Máximo)
  delay(150);                  // Tempo para você ligar a bateria            // Aguarda calibração
  esc.writeMicroseconds(0);
  // Agora o ESC está calibrado
}


void loop() {
  // Se tiver dados disponíveis na serial
  if (Serial.available()) {
    char c = Serial.read();

    // Se for Enter, processa o comando
    if (c == '\n' || c == '\r') {
      pwm = inputString.toInt();

      // Verifica se está dentro do intervalo válido
      if (pwm >= 1000 && pwm <= 2000) {
        esc.writeMicroseconds(pwm);
        Serial.print("Enviado: ");
        Serial.println(pwm);
      } else {
        Serial.println("Valor inválido (use 1000–2000)");
      }

      inputString = ""; // Limpa o buffer
    } else {
      inputString += c;
    }
  }

  // Mantém o sinal sendo enviado
  esc.writeMicroseconds(pwm);
  delay(20);
}

*/