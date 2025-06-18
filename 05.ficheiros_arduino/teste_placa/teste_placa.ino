/*
* Robot Sumo
* André Carvalho / 2023
* V 1.0
*
*/

const int MotorEsqIn1 = 6;
const int MotorEsqIn2 = 5;
const int MotorDirIn1 = 9;
const int MotorDirIn2 = 10;
const int SensorObjt = 3;
const int SensorLinhaDir = 4;
const int SensorLinhaEsq = 7;

//Para calibração da direção
int VelocidadeBaseDir = 255;
int VelocidadeBaseEsq = 255;

enum Estado { PARADO,
              FRENTE,
              ESQUERDA,
              DIREITA,
              TRAS };
Estado estadoAtual = PARADO;

enum EstadoSensoresLinha { LINHA_ESQ,
                           LINHA_DIR,
                           LINHA_FRENTE,
                           LINHA_NENHUMA
};

//Funções
void desligaMotores();
void andarFrente();
void andarEsq();
void andarDir();
void andarTras();
void executaAccao(Estado estadoAtual);
EstadoSensoresLinha avaliaSensoresLinha();
void oponenteDerrotado();

int tempoInicial, tempoAtual = 0;

void setup() {
  pinMode(MotorEsqIn1, OUTPUT);
  pinMode(MotorEsqIn2, OUTPUT);
  pinMode(MotorDirIn1, OUTPUT);
  pinMode(MotorDirIn2, OUTPUT);
  pinMode(SensorObjt, INPUT);
  pinMode(SensorLinhaEsq, INPUT);
  pinMode(SensorLinhaDir, INPUT);

  //Setup da consola
  Serial.begin(9600);

  desligaMotores();

  delay(1000);

  //Tempo no qual foi iniciado o programa
  tempoInicial = millis();
}

void loop() {
  tempoAtual = millis();
  avaliaSensoresLinha();
return;
  //Serial.println(avaliaSensoresLinha());

 executaAccao(FRENTE);
  if (digitalRead(SensorObjt) == false) {
    Serial.println("OBJ");
  }
  return;

  executaAccao(FRENTE);
  Serial.println("Frente");
  delay(5000);


  executaAccao(TRAS);
  Serial.println("Tras");

  delay(5000);
  executaAccao(DIREITA);
  Serial.println("Direita");

  delay(5000);
  executaAccao(ESQUERDA);
  Serial.println("Esquerda");

  delay(5000);
  desligaMotores();

  tempoInicial = millis();

  EstadoSensoresLinha estado = LINHA_NENHUMA;
  do {
    tempoAtual = millis();
    avaliaSensoresLinha();
  } while (tempoAtual < (tempoInicial + 20000));


  return;
}



void executaAccao(Estado estadoAtual) {
  switch (estadoAtual) {
    case FRENTE: andarFrente(); break;
    case DIREITA: andarDir(); break;
    case ESQUERDA: andarEsq(); break;
    case TRAS: andarTras(); break;
    default: desligaMotores(); break;
  }
}


void desligaMotores() {
  analogWrite(MotorEsqIn1, 0);
  analogWrite(MotorEsqIn2, 0);
  analogWrite(MotorDirIn1, 0);
  analogWrite(MotorDirIn2, 0);
}

void andarFrente() {
  analogWrite(MotorEsqIn1, VelocidadeBaseEsq);
  analogWrite(MotorEsqIn2, 0);
  analogWrite(MotorDirIn1, VelocidadeBaseDir);
  analogWrite(MotorDirIn2, 0);
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
  if (digitalRead(SensorLinhaDir) == false && digitalRead(SensorLinhaEsq) == false) {
    //Se ambos os sensores detectarem a linha
    Serial.println("NENHUMA");
    return LINHA_FRENTE;

  } else if (digitalRead(SensorLinhaEsq) == false) {
    Serial.println("ESQ");
    //Se estiver a tocar na linha da esquerda
    return LINHA_ESQ;
  } else if (digitalRead(SensorLinhaDir) == false) {
    Serial.println("DIR");
    //Se estiver a tocar na linha da esquerda
    return LINHA_DIR;
  }
  return LINHA_NENHUMA;
}

void oponenteDerrotado() {
  int tempoInicial = millis();
  int tempoAtual = 0;
  do {
    tempoAtual = millis();


    if ((tempoAtual - tempoInicial) > 500) {
      break;
    }

    executaAccao(TRAS);
  } while (true);

  tempoInicial = millis();
  do {
    tempoAtual = millis();


    if ((tempoAtual - tempoInicial) > 200) {
      break;
    }

    executaAccao(DIREITA);
  } while (true);

  tempoInicial = millis();
  do {
    tempoAtual = millis();


    if ((tempoAtual - tempoInicial) > 200) {
      break;
    }

    executaAccao(ESQUERDA);
  } while (true);

  desligaMotores();

  delay(2000);
  return;
}
