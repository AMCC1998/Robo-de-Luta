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

const int SensorLinhaDir = 4;
const int SensorLinhaEsq = 7;


#define USAIR 0  //Colocar a 0 no caso de não usar o sensor IR para detetar o oponente mas sim o ultrassons


#if (USAIR == 1)
const int SensorObjt = 3;  // Para Usar o sensor IR
#define trigPin 0
#define echoPin 0
#else
const int SensorObjt = 0;
//Para o sensor de distancia HC
#define trigPin 2
#define echoPin 3
#endif
float duration, distance;
int distaciaDetecao = 50; //Valor em cm de ate onde o sensor de ultrassons vai detectar


#define DEBUG 1  //Colocar a 0 para não imprimir mensagens de debug na consola

#if (DEBUG == 1)
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif


//Para calibração da direção
int VelocidadeBaseDir = 255;
int VelocidadeBaseEsq = 255;

int tempoAccao, tempoInicial = 0;
bool encontrei = false;

#define Bluetooth Serial1

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


// Funções
void menuBluetooth();

void sumoBot();

void desligaMotores();
void andarFrente(int velocidade);
void andarEsq();
void andarDir();
void andarTras();
EstadoSensoresLinha avaliaSensoresLinha();
bool avaliaSensorObj();
String readBluetoothString();
float calculaDistancia();



void setup() {
  pinMode(MotorEsqIn1, OUTPUT);
  pinMode(MotorEsqIn2, OUTPUT);
  pinMode(MotorDirIn1, OUTPUT);
  pinMode(MotorDirIn2, OUTPUT);
  pinMode(SensorLinhaEsq, INPUT);
  pinMode(SensorLinhaDir, INPUT);


  if (USAIR == 1) {
    pinMode(SensorObjt, INPUT);
  } else {
    //Para o sensor de distancia HC
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }


  Serial.begin(9600);     //Inicialização Serial da comunicação com a consola
  Bluetooth.begin(9600);  //Inicialização Serial da comunicação bluetooth

  desligaMotores();

  delay(1000);

  //Tempo no qual foi iniciado o programa
  tempoInicial = millis();
  debugln("Fim void Setup");
}


void loop() {
  char dadoBluetooth = '\0';

  debugln(avaliaSensorObj());
  delay(500);

  if (Bluetooth.available()) {
    dadoBluetooth = Bluetooth.read();
    debugln(dadoBluetooth);
    switch (dadoBluetooth) {
      case 'S': sumoBot(); break;  //Sumobot
      case 'C': rcCar(); break;    //rcar
      case 'L': break;             //seguelinha
    }
  } else {
    dadoBluetooth = '\0';
  }
}
