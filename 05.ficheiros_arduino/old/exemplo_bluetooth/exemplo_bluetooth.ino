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
int VelocidadeBaseDir = 200;
int VelocidadeBaseEsq = 200;

int tempoAccao, tempoInicial = 0;
bool encontrei = false;

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
void sumoBot();

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
  Serial1.begin(9600);

  desligaMotores();

  delay(1000);

  //Tempo no qual foi iniciado o programa
  tempoInicial = millis();
}

void loop() {
  char dadoBluetooth = ""; 

  if (Serial1.available()) {
    dadoBluetooth = Serial1.read();
    switch(dadoBluetooth) {
      case 'S': sumoBot(); break;//Sumobot
      case 'C': break;//rcar
      case 'L': break;//seguelinha
    }
  }else{
    dadoBluetooth = "";
  }
}



void sumoBot() {
  Serial.println("Void Loop!");
  VelocidadeBaseEsq, VelocidadeBaseDir = 110;
  estadoAtual = FRENTE;
  bool accaoLinha = false;
  tempoInicial = millis();
  //Tempo atual
  int tempoAtual = millis();

  //Validar se estamos a ver o oponente
  if (!digitalRead(SensorObjt)) {
EmpurraOponente:
    Serial.println("EmpurraOponente!");

    do {

      //Validar sensores de linha
      switch (avaliaSensoresLinha()) {
        case LINHA_DIR: estadoAtual = ESQUERDA; break;
        case LINHA_ESQ: estadoAtual = DIREITA; break;
        case LINHA_FRENTE:
          oponenteDerrotado();
          goto ProcurarOponente;
          break;
        //Vamos validar se o estado atual é TRAS se for é porque empurramos o oponente para fora das linhas
        default:
          !digitalRead(SensorObjt) ? estadoAtual = FRENTE : estadoAtual = DIREITA;
          break;
      }

      if (estadoAtual == FRENTE) {
        //Para calibração da direção
        VelocidadeBaseDir, VelocidadeBaseEsq = 255;
      } else {
        //Para calibração da direção
        VelocidadeBaseEsq, VelocidadeBaseDir = 110;
      }
      executaAccao(estadoAtual);

      //Por vezes não era suficientemente rápido a detetar a linha por isso validamos 2x por ciclo
      if (avaliaSensoresLinha() == FRENTE) {
        oponenteDerrotado();
        goto ProcurarOponente;
        break;
      }
    } while (!digitalRead(SensorObjt));



    //Se perdermos de vista o oponente, vamos andar a volta durante x tempo para o tentar encontrar
    tempoInicial = millis();
    switch (avaliaSensoresLinha()) {
      case LINHA_DIR: estadoAtual = ESQUERDA; break;
      case LINHA_ESQ: estadoAtual = DIREITA; break;
      case LINHA_FRENTE: estadoAtual = TRAS; break;
      default: estadoAtual = DIREITA; break;
    }

ProcurarOponente:
    Serial.println("Procura Oponente!");
    do {
      VelocidadeBaseEsq, VelocidadeBaseDir = 110;
      tempoAtual = millis();

      switch (avaliaSensoresLinha()) {
        case LINHA_DIR: estadoAtual = ESQUERDA; break;
        case LINHA_ESQ: estadoAtual = DIREITA; break;
        case LINHA_FRENTE: estadoAtual = TRAS; break;
      }

      //Validar o sensor IR frontal
      if (!digitalRead(SensorObjt)) {
        Serial1.println("O");
        encontrei = true;
        executaAccao(estadoAtual == ESQUERDA ? DIREITA : ESQUERDA);
        tempoInicial = millis();
        do {
          tempoAtual = millis();

          if (!digitalRead(SensorObjt)) {
            Serial1.println("O");
            goto EmpurraOponente;
            break;
          }

          if ((tempoAtual - tempoInicial) > 2000) {
            break;
          }
          estadoAtual = FRENTE;
        } while (true);
        goto EmpurraOponente;
        break;
      }


      if ((tempoAtual - tempoInicial) > 2000) {
        break;
      }

      executaAccao(estadoAtual);
    } while (true);
  }
  //Fim do if que valida se estamos a ver o opunente



  switch (avaliaSensoresLinha()) {
    case LINHA_DIR: estadoAtual = ESQUERDA; break;
    case LINHA_ESQ: estadoAtual = DIREITA; break;
    case LINHA_FRENTE: estadoAtual = TRAS; break;
    default: estadoAtual = FRENTE; break;
  }


  if (estadoAtual != FRENTE) {
    VelocidadeBaseEsq, VelocidadeBaseDir = 110;
    //Entra aqui se detectar alguma linha
    tempoInicial = millis();

    if (estadoAtual != TRAS) {
      //Vamos desviar da linha
      tempoInicial = millis();
      do {
        tempoAtual = millis();

        //Validar o sensor IR frontal
        if (!digitalRead(SensorObjt)) {
          Serial1.println("O");
          encontrei = true;
          goto EmpurraOponente;
          break;
        }


        if ((tempoAtual - tempoInicial) > 400) {
          break;
        }

        executaAccao(TRAS);
      } while (true);
    }

    tempoInicial = millis();
    do {
      tempoAtual = millis();

      //Validar o sensor IR frontal
      if (!digitalRead(SensorObjt)) {
        Serial1.println("O");
        goto EmpurraOponente;
        break;
      }

      if ((tempoAtual - tempoInicial) > 2000) {
        break;
      }


      executaAccao(estadoAtual);
    } while (true);

    if (estadoAtual == TRAS) {
      switch (avaliaSensoresLinha()) {
        case LINHA_DIR: estadoAtual = ESQUERDA; break;
        case LINHA_ESQ: estadoAtual = DIREITA; break;
        default: estadoAtual = DIREITA; break;
      }
      tempoInicial = millis();
      do {
        tempoAtual = millis();

        //Validar o sensor IR frontal
        if (!digitalRead(SensorObjt)) {
          Serial1.println("O");
          goto EmpurraOponente;
          break;
        }


        if ((tempoAtual - tempoInicial) > 300) {
          break;
        }

        executaAccao(estadoAtual);
      } while (true);
    }

    estadoAtual = FRENTE;
  }

  //Validar o sensor IR frontal
  if (!digitalRead(SensorObjt)) {
    Serial1.println("O");
    encontrei = true;
    goto EmpurraOponente;
  }

  executaAccao(estadoAtual);

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
  if (digitalRead(SensorLinhaDir) == true && digitalRead(SensorLinhaEsq) == true) {
    //Se ambos os sensores detectarem a linha
    Serial1.println("K");
    return LINHA_FRENTE;
  } else if (digitalRead(SensorLinhaEsq) == true) {
    //Se estiver a tocar na linha da esquerda
    Serial1.println("L");
    return LINHA_ESQ;
  } else if (digitalRead(SensorLinhaDir) == true) {
    //Se estiver a tocar na linha da esquerda
    Serial1.println("R");
    return LINHA_DIR;
  }
  return LINHA_NENHUMA;
}

void oponenteDerrotado() {
  return;
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
