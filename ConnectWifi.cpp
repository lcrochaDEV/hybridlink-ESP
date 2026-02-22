// Configuração automática de bibliotecas baseada na placa
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  
#elif defined(ESP32)
  #include <WiFi.h>
#endif
#include <WiFiManager.h> // Biblioteca:
#include "ConnectWifi.h"

ConnectWifi::ConnectWifi(const char* ssid, const char* password)
  : ssid(ssid), password(password) {
  }

void ConnectWifi::connectionsMethod(){
  if (ssid == nullptr) {
        Serial.println("Erro: SSID não fornecido para conexão manual.");
        return;
    }
  // 1. Força a desconexão total para limpar o rádio
  WiFi.disconnect(true);
  delay(100);
  //Serial.begin(115200);
  Serial.printf("Conectando a %s ", ssid);
  // Configura para reconectar automaticamente se cair
  WiFi.persistent(false);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password); // Inicia a conexão

  // Reinicia contador para nova tentativa
  tentativaAtual = 0;

  while (WiFi.status() != WL_CONNECTED) {// Aguarda a conexão ser estabelecida
    delay(500);
    if (WiFi.status() == WL_CONNECTED) break;
    Serial.print(".");

    if(tentativaAtual == maxTentativas){
      Serial.println("\nFalha ao conectar Wifi!");
      return;
    }
    tentativaAtual++; // Incrementa o contador
  }
  connect_status();
}

void ConnectWifi::accesspoint(){
  WiFiManager wm; 
  
  // Configura para reconectar automaticamente se cair
  //WiFi.persistent(false);
  WiFi.setAutoReconnect(true);
  
  //wm.resetSettings();
  wm.setDebugOutput(false); 
  if(!wm.autoConnect("ESP:AP", "senha123")) { 
    Serial.println("Falha na conexão ou tempo esgotado"); 
    return;
  } 
  connect_status();
}

bool ConnectWifi::connect_status(){
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("\nConectado à Rede: %s\n", WiFi.SSID().c_str()); 
        Serial.printf("Endereco IP: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("Endereco MAC: %s\n", WiFi.macAddress().c_str()); 
        Serial.printf("Canal Wi-Fi atual: %d\n", WiFi.channel()); 
        return true; // Retorna TRUE, indicando sucesso.
    } else {
      Serial.println("\nWifi Desconectado!");
        return false; // Retorna FALSE, indicando falha.
  }
}