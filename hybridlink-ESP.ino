//sketch.ino
#include "WirelessConnection.h"
#include "servidorweb.h"

#include "AccessControl.h"
#include "Console.h"
Console console;


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
  //sysFS.factoryReset(); 
  sysFS.returnObjectData();
}

void loop() {
  //CONSOLE
  console.consoleView();
}

void setupFS() {
  //Serial.begin(115200);
  
  if(sysFS.begin()) {
      JsonDocument doc;
      
      sysFS.loadConfig(doc);
      // Só grava as credenciais se o arquivo não puder ser carregado
      if(!sysFS.loadConfig(doc)) {
          Serial.println("Arquivo não encontrado. Criando base...");
      }

      //sysFS.credentials();
      //sysFS.addPinConfig();
      //sysFS.loadConfig(doc); // Carrega após gravar
      //sysFS.addPinConfig(0, "OUTPUT", "LOW");
      //sysFS.addPinConfig(2, "OUTPUT", "HIGH");

      
      Serial.print("SSID Ativo: ");
      Serial.println(doc["ssid"].as<const char*>());
  }
}


