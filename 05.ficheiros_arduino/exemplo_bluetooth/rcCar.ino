void rcCar() {
  char dadoBluetooth = '\0';
  int velocidade = 255;
  do {
    if (Bluetooth.available()) {
      dadoBluetooth = Bluetooth.read();
      //debugln(dadoBluetooth);

     /* if (isDigit(dadoBluetooth)) {
        debugln("dadoBluetooth: " + velocidade);
        velocidade = atoi(dadoBluetooth);
        debugln("atoi: " + velocidade);
        velocidade = (velocidade * 255) / 9;
      }
      debugln("Velocidade: " + velocidade);*/

      switch (dadoBluetooth) {
        case 'F':
          do {
            dadoBluetooth = Bluetooth.read();
            andarFrente(velocidade);
          } while (dadoBluetooth != '!');
          break;


        case 'R':
          do {
            dadoBluetooth = Bluetooth.read();
            andarDir();
          } while (dadoBluetooth != '!');
          break;

        case 'L':
          do {
            dadoBluetooth = Bluetooth.read();
            andarEsq();
          } while (dadoBluetooth != '!');
          break;

        case 'B':
          do {
            dadoBluetooth = Bluetooth.read();
            andarTras();
          } while (dadoBluetooth != '!');
          break;
      }


      desligaMotores();
    }
  } while (dadoBluetooth != 'P');
}