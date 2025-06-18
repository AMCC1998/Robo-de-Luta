void sumoBot() {
  debugln("SumoBot\n");
  bool EstadoSensorObjt = false;
  EstadoSensoresLinha EstadoLinha = LINHA_NENHUMA;
  Estado EstadoAtualAcao = PARADO;
  char dadoBluetooth = '\0';

  VelocidadeBaseDir = VelocidadeBaseEsq = 80;

  do {

    VelocidadeBaseDir = VelocidadeBaseEsq = 80;

    if (Bluetooth.available()) {
      dadoBluetooth = Bluetooth.read();
    }

    EstadoLinha = avaliaSensoresLinha();
    EstadoSensorObjt = avaliaSensorObj();
    //debugln(EstadoLinha);

    if (EstadoLinha == LINHA_NENHUMA && EstadoSensorObjt == false) {
      EstadoAtualAcao = FRENTE;
      Bluetooth.print("K");
      executaAcao(EstadoAtualAcao);

    } else if (EstadoLinha == LINHA_FRENTE) {
      EstadoAtualAcao = TRAS;
      executaAcao(EstadoAtualAcao);
      delay(1500);

      EstadoAtualAcao = ESQUERDA;
      executaAcao(EstadoAtualAcao);
      delay(600);

    } else if (EstadoLinha == LINHA_DIR) {
      EstadoAtualAcao = ESQUERDA;
      executaAcao(EstadoAtualAcao);
      delay(1000);
    } else if (EstadoLinha == LINHA_ESQ) {
      EstadoAtualAcao = DIREITA;
      executaAcao(EstadoAtualAcao);
      delay(1000);
    }

    if (EstadoSensorObjt == false) {
      //Encontrei o oponente
      do {
        VelocidadeBaseDir = VelocidadeBaseEsq = 240;
        EstadoSensorObjt = avaliaSensorObj();
        EstadoLinha = avaliaSensoresLinha();

        EstadoAtualAcao = FRENTE;

        if (EstadoLinha == LINHA_FRENTE) {
          EstadoAtualAcao = PARADO;
        }

        executaAcao(EstadoAtualAcao);
      } while (EstadoSensorObjt);
    }


    //debugln(EstadoAtualAcao);
  } while (dadoBluetooth != 'P');
}

void executaAcao(Estado acao) {
  switch (acao) {
    case PARADO:
      desligaMotores();
      break;
    case FRENTE:
      andarFrente();
      break;
    case TRAS:
      andarTras();
      break;
    case DIREITA:
      andarDir();
      break;
    case ESQUERDA:
      andarEsq();
      break;
  }
}
