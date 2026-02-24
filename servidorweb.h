#ifndef SERVIDORWEB_H
#define SERVIDORWEB_H

#include <Arduino.h>
#include <ArduinoJson.h>
#if defined(ESP8266)
  #include <ESPAsyncTCP.h>
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <AsyncTCP.h>
  #include <WiFi.h>
#endif
#include <ESPAsyncWebServer.h>
#include "index_html.h"

// Cria o objeto Servidor na porta 80 (porta HTTP padrão)
AsyncWebServer server(80);

// Protótipos de Funções
String processor(const String& var);

// Função de Processamento para substituir o marcador %STATE% no HTML
String processor(const String& var){
    Serial.print("Placeholder requisitado: "); Serial.println(var);

    if(var == "SSID_VALUE") return (WiFi.status() == WL_CONNECTED) ? WiFi.SSID() : "Desconectado"; // Retorna o SSID atual ou uma mensagem se desconectado
    
    if(var == "IP_VALUE") return (WiFi.status() == WL_CONNECTED) ? WiFi.localIP().toString() : "0.0.0.0"; // Retorna o IP local
    
    if(var == "MAC_VALUE") return WiFi.macAddress(); // Retorna o endereço MAC

    if(var == "TOTAL_RAN_VALUE")      return String((unsigned long)ESP.getFreeHeap() / 1024) + " KB";

    if(var == "FLASH_SIZE_VALUE")     return String((unsigned long)ESP.getFlashChipSize() / (1024 * 1024)) + " MB";

    if(var == "MENOR_RAN_SIZE_VALUE") return String((unsigned long)ESP.getFreeHeap() / 1024) + " KB"; 

    if(var == "SKETCH_SIZE_VALUE")    return String((unsigned long)ESP.getSketchSize() / 1024) + " KB";

    
    // Para qualquer outro placeholder não mapeado
    return String("");
}

void startServer() {
    // Configuração de Rotas (Endpoints)
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, processor);
    });

    // Rota de Controle (JSON)
    server.on("/controlar", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, 
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        
        JsonDocument doc;
        deserializeJson(doc, data, len);
        int pino = doc["pino"];
        int estado = doc["estado"];
        
        if (pino == 0 || pino == 2) {
            digitalWrite(pino, estado);
            Serial.printf("Acionado o Pino: %d com estado: %d\n", pino, estado);
            request->send(200, "application/json", "{\"status\":\"ok\"}");
        } else {
            request->send(400, "application/json", "{\"status\":\"erro\"}");
        }
    });
  // Inicia o Servidor 
  server.begin();
  Serial.println("Servidor HTTP Async Iniciado!");
}

#endif