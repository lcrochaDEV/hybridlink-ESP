#include "WString.h"
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

#include "ModuloESP.h"
ModuloESP meuEsp; 

// Cria o objeto Servidor na porta 80 (porta HTTP padrão)
AsyncWebServer server(80);

// Protótipos de Funções
String processor(const String& var);

// Função de Processamento para substituir o marcador %STATE% no HTML
String processor(const String& var){
    Serial.print("Placeholder requisitado: "); Serial.println(var);

    if(var == "SSID_VALUE") return (WiFi.status() == WL_CONNECTED) ? WiFi.SSID() : "Desconectado"; // Retorna o SSID atual ou uma mensagem se desconectado
    
    if(var == "IP_VALUE")   return (WiFi.status() == WL_CONNECTED) ? WiFi.localIP().toString() : "0.0.0.0"; // Retorna o IP local
    
    if(var == "MAC_VALUE")  return WiFi.macAddress(); // Retorna o endereço MAC

    if(var == "MODULE_VALUE")         return meuEsp.modelBoardESP();

    if(var == "TOTAL_RAN_VALUE")      return meuEsp.total_ran();

    if(var == "FLASH_SIZE_VALUE")     return meuEsp.flash_size();
    
    if(var == "MENOR_RAN_SIZE_VALUE") return meuEsp.menor_ran_size();

    if(var == "SKETCH_SIZE_VALUE")    return meuEsp.sketch_Size();
    
    // Para qualquer outro placeholder não mapeado
    return String("");
}

void startServer() {
    // Configuração de Rotas (Endpoints)
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, processor);
    });
    
    server.on("/config_pinos", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "application/json", meuEsp.pinGPIO());
    });

    // Rota de Controle (JSON)
    server.on("/controlar", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, 
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        
        JsonDocument doc, config;
        deserializeJson(doc, data, len);
        deserializeJson(config, meuEsp.pinGPIO());

        int pino = doc["pino"];
        int estado = doc["estado"];
        
    // Verifica se o número do pino existe em qualquer "valor" do JSON de configuração
        bool pinoValido = false;
        for (JsonPair kv : config["gpios"].as<JsonObject>()) { 
            if(kv.value().as<int>() == pino) { 
                pinoValido = true; 
                break; 
            } 
        }
        if (pinoValido) {
            digitalWrite(pino, estado);
            Serial.printf("Acionado o Pino: %d com estado: %d\n", pino, estado);
            request->send(200, "application/json", "{\"status\":\"ok\"}");
        } else {
            request->send(403, "application/json", "{\"status\":\"Pino não encontrado no módulo\"}");
        }
    });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
        // 1. Criamos os documentos JSON
        JsonDocument statusDoc; 
        JsonDocument pinsDoc;

        // 2. Pegamos a String do seu método e transformamos em um objeto JSON usável
        deserializeJson(pinsDoc, meuEsp.pinGPIO());
        JsonObject gpios = pinsDoc["gpios"];

        // 3. Iteramos sobre cada par (Chave/Valor) dentro do objeto "gpios"
        for (JsonPair kv : gpios) {
            const char* gpioName = kv.key().c_str(); // Ex: "GPIO18"
            int pinNumber = kv.value().as<int>();    // Ex: 18

            // 4. Lemos o estado do pino fisicamente e adicionamos ao JSON de resposta
            statusDoc[gpioName] = digitalRead(pinNumber);
        }

        String response;
        serializeJson(statusDoc, response);
        
        request->send(200, "application/json", response);
    });
    
  // Inicia o Servidor 
  server.begin();
  Serial.println("Servidor HTTP Async Iniciado!");
}

#endif