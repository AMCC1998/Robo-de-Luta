String readBluetoothString() {
  while (!Bluetooth.available()) {
    // Aguardar até que dados estejam disponíveis no módulo Bluetooth
  }
  return Bluetooth.readStringUntil('\n');
}

void desligaMotores() {
  analogWrite(MotorEsqIn1, 0);
  analogWrite(MotorEsqIn2, 0);
  analogWrite(MotorDirIn1, 0);
  analogWrite(MotorDirIn2, 0);
}

void andarFrente(int velocidade = 0) {
  if (velocidade == 0) {
    analogWrite(MotorEsqIn1, VelocidadeBaseEsq);
    analogWrite(MotorEsqIn2, 0);
    analogWrite(MotorDirIn1, VelocidadeBaseDir);
    analogWrite(MotorDirIn2, 0);
  } else {
    analogWrite(MotorEsqIn1, velocidade);
    analogWrite(MotorEsqIn2, 0);
    analogWrite(MotorDirIn1, velocidade);
    analogWrite(MotorDirIn2, 0);
  }
}

void andarTras() {
  analogWrite(MotorEsqIn1, 0);
  analogWrite(MotorEsqIn2, VelocidadeBaseEsq);
  analogWrite(MotorDirIn1, 0);
  analogWrite(MotorDirIn2, VelocidadeBaseDir);
}

void andarEsq() {
  analogWrite(MotorEsqIn1, 0);
  analogWrite(MotorEsqIn2, VelocidadeBaseEsq);
  analogWrite(MotorDirIn1, VelocidadeBaseDir);
  analogWrite(MotorDirIn2, 0);
}

void andarDir() {
  analogWrite(MotorEsqIn1, VelocidadeBaseEsq);
  analogWrite(MotorEsqIn2, 0);
  analogWrite(MotorDirIn1, 0);
  analogWrite(MotorDirIn2, VelocidadeBaseDir);
}

EstadoSensoresLinha avaliaSensoresLinha() {

  //Se o sensor esq detectar a linha, vamos mudar a direção para DIREITA
  if (digitalRead(SensorLinhaDir) == true && digitalRead(SensorLinhaEsq) == true) {
    //Se ambos os sensores detectarem a linha
    Bluetooth.println("F");
    debugln("F");
    return LINHA_FRENTE;
  } else if (digitalRead(SensorLinhaEsq)) {
    //Se estiver a tocar na linha da esquerda
    Bluetooth.println("E");
    debugln("E");
    return LINHA_ESQ;
  } else if (digitalRead(SensorLinhaDir)) {
    //Se estiver a tocar na linha da esquerda
    Bluetooth.println("D");
    debugln("D");
    return LINHA_DIR;
  }
  //Bluetooth.println("K");
  Bluetooth.flush();
  //debugln("K");
  return LINHA_NENHUMA;
}

bool avaliaSensorObj() {


  if (USAIR == 1) {
    //Se usar o sensor IR
    if ((bool)!digitalRead(SensorObjt)) {
      Bluetooth.println("O");
      return true;
    }
  } else {
    //Se usar o sensor ultrassons
    if (calculaDistancia() <= distaciaDetecao) {
      debugln("Sensor HC, Encontrei objs");
      Bluetooth.println("O");
      return true;
    }
  }

  Bluetooth.println("K");
  return false;
}

float calculaDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .0343) / 2;
  return distance;
}
