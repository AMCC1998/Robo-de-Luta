#include <ESP8266WiFi.h>
#include <espnow.h>

typedef struct struct_message {
  char msg[32];
} struct_message;

struct_message myData;

//B4:E6:2D:45:48:A6
uint8_t receiverMac[] = { 0xB4, 0xE6, 0x2D, 0x45, 0x48, 0xA6 }; // MAC do ESP32-Receptor


void OnDataSent(uint8_t *mac_addr, uint8_t status) {
  Serial.print("Envio status: ");
  Serial.println(status == 0 ? "Sucesso" : "Falha");
}

void setup() {
  Serial.begin(9600); // mesmo baud do Arduino
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  wifi_set_channel(1);

  if (esp_now_init() != 0) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  if (!esp_now_add_peer(receiverMac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0)) {
    Serial.println("Falha ao adicionar peer");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // remove espaços e quebras

    if (input.length() > 0 && input.length() < sizeof(myData.msg)) {
      input.toCharArray(myData.msg, sizeof(myData.msg));
      int result = esp_now_send(receiverMac, (uint8_t *)&myData, sizeof(myData));

      if (result == 0) {
        Serial.println("Mensagem enviada com sucesso");
      } else {
        Serial.println("Erro ao enviar");
      }
    }
  }
}
