#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
  char msg[32];
} struct_message;

void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
  //Serial.println("Callback recebido!");

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           recv_info->src_addr[0], recv_info->src_addr[1], recv_info->src_addr[2],
           recv_info->src_addr[3], recv_info->src_addr[4], recv_info->src_addr[5]);
  /*Serial.print("Recebido de: ");
  Serial.println(macStr);*/

  struct_message received;
  memcpy(&received, data, sizeof(received));
  /*Serial.print("Mensagem: ");*/
  Serial.println(received.msg);
}

uint8_t peerMac[] = { 0x84, 0xF3, 0xEB, 0x18, 0x46, 0x19 };  // MAC do ESP8266 que você passou

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.setChannel(1);  // Certifique que é o mesmo canal do transmissor
  Serial.println("Setup ok");
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peerMac, 6);
  peerInfo.channel = 1;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Falha ao adicionar peer");
  }
}

void loop() {
}
