//sketch.ino
#include "WirelessConnection.h"
#include "servidorweb.h"
#include "AccessControl.h"
#include "Console.h"

Console console = Console("Mochi> ");
WirelessConnection wirelessConnection;
AccessControl sysFS("MinhaRede", "12345678");

void setup() {
  Serial.begin(115200);
  delay(1000); 

  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);

  wirelessConnection.accesspoint();
  startServer();   
  setupFS();
  console.helloWord();
}

void loop() {
  console.consoleView();

  // Chama direto o update da classe. Ela mesma sabe se deve rodar e o tempo certo!
  mqttService.update(); 
}

void setupFS() {
  if(sysFS.begin()) {
      JsonDocument doc;
      
      if(!sysFS.loadConfig(doc)) {
          Serial.println("Arquivo não encontrado. Criando base...");
      } else {
          // Garante que inicie desligado por segurança antes de validar
          mqttService.disable(); 

          if (doc.containsKey("mqtt") && doc["mqtt"].is<JsonArray>()) {
              for (JsonObject item : doc["mqtt"].as<JsonArray>()) {
                  if (item["active"] == true) {
                      Serial.println("[MQTT] Perfil ativo encontrado no boot! Inicializando...");
                      
                      mqttService.updateConfig(
                          item["broker"] | "", 
                          item["port"]   | 1883, 
                          item["topic"]  | "", 
                          item["user"]   | "", 
                          item["passw"]  | "",
                          item["qos"]    | 0,
                          item["ssl"]    | false
                      );
                      
                      mqttService.begin();
                      
                      // ATIVA O FILTRO INTERNO DA CLASSE
                      mqttService.enable(); 
                      break; 
                  }
              }
          }
      }
      Serial.print("SSID Ativo: ");
      Serial.println(doc["ssid"].as<const char*>());
  }
}