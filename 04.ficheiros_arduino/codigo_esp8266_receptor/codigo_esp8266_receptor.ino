#include <ESP8266WiFi.h>
#include <espnow.h>

typedef struct struct_message {
  char msg[32];
} struct_message;

struct_message incomingData;

void OnDataRecv(uint8_t *mac, uint8_t *incomingDataRaw, uint8_t len) {
  char senderMacStr[18];
  snprintf(senderMacStr, sizeof(senderMacStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  //Serial.print("Mensagem recebida de: ");
  //Serial.println(senderMacStr);

  memcpy(&incomingData, incomingDataRaw, sizeof(incomingData));
  //Serial.print("Conteúdo: ");
  Serial.println(incomingData.msg);
}

void setup() {
  Serial.begin(9600);  // mesma taxa de baud do Arduino
  delay(1000);
  Serial.println("teste");
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  wifi_set_channel(1);  // mesmo canal do emissor

  if (esp_now_init() != 0) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}
