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

#include "AccessControl.h"
AccessControl accessSys; 

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

    if(var == "MODULE_VALUE")         return accessSys.modelBoardESP();

    if(var == "TOTAL_RAN_VALUE")      return accessSys.total_ran();

    if(var == "FLASH_SIZE_VALUE")     return accessSys.flash_size();
    
    if(var == "MENOR_RAN_SIZE_VALUE") return accessSys.menor_ran_size();

    if(var == "SKETCH_SIZE_VALUE")    return accessSys.sketch_Size();
    
    // Para qualquer outro placeholder não mapeado
    return String("");
}

void startServer() {
    // Configuração de Rotas (Endpoints)
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, processor);
    });
    
    server.on("/config_pinos", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "application/json", accessSys.pinGPIO());
    });

    // Rota de Controle (JSON)
    server.on("/controlar", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, 
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        
        JsonDocument doc, config;
        deserializeJson(doc, data, len);
        deserializeJson(config, accessSys.pinGPIO());

        int pin = doc["pin"];
        int state = doc["state"];
        
    // Verifica se o número do pino existe em qualquer "valor" do JSON de configuração
        bool pinoValidation = false;
        for (JsonPair kv : config["gpios"].as<JsonObject>()) { 
            if(kv.value().as<int>() == pin) { 
                pinoValidation = true; 
                break; 
            } 
        }
        if (pinoValidation) {
            digitalWrite(pin, state);
            accessSys.setPinState(pin, state);
            Serial.printf("Acionado o Pino: %d com estado: %d\n", pin, state);
            request->send(200, "application/json", "{\"status\":\"ok\"}");
        } else {
            request->send(403, "application/json", "{\"status\":\"Pino não encontrado no módulo\"}");
        }
    });
    // --- ROTA: Mudar Modo (INPUT/OUTPUT) ---
    server.on("/config_modo", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, 
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        
        JsonDocument doc;
        deserializeJson(doc, data, len);

        int pin = doc["pin"];
        String mode = doc["mode"]; // "INPUT" ou "OUTPUT"

        PinMode_t modeEnum = MODE_INPUT; // Valor padrão de segurança

        if (mode == "OUTPUT") {
            modeEnum = MODE_OUTPUT;
            pinMode(pin, OUTPUT);
        } else if (mode == "INPUT") {
            modeEnum = MODE_INPUT;
            pinMode(pin, INPUT);
        } else if (mode == "INPUT_PULLUP") {
            modeEnum = MODE_INPUT_PULLUP;
            pinMode(pin, INPUT_PULLUP);
        } else if (mode == "INPUT_PULLDOWN") {
            modeEnum = MODE_INPUT_PULLDOWN;
            // Tratamento específico para o GPIO 16 do ESP8266
            if (pin == 16) pinMode(pin, INPUT_PULLDOWN_16);
            else pinMode(pin, INPUT);
        }
        // 2. Persiste no LittleFS para o próximo boot
        // Usamos o método que criamos: ele lê o WiFi, acha o pino e só muda o 'mode'
        accessSys.setPinMode(pin, modeEnum);

        Serial.printf("Pino %d alterado para modo: %s\n", pin, mode.c_str());
        request->send(200, "application/json", "{\"status\":\"Modo atualizado\"}");
    });

    // --- ROTA: Definir Nível Lógico Inicial (HIGH/LOW) ---
    server.on("/set_level", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, 
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        
        JsonDocument doc;
        deserializeJson(doc, data, len);

        int pin = doc["pin"];
        int level = doc["level"]; // 0 ou 1
         

        // 1. Atualiza no LittleFS (Estado que ele deve assumir ao ligar)
        // Buscamos o modo atual no FS primeiro ou passamos o que já sabemos
        accessSys.setPinLevel(pin, level);

        // 2. Aplica agora
        digitalWrite(pin, level);

        Serial.printf("Pino %d nível lógico padrão: %d\n", pin, level);
        request->send(200, "application/json", "{\"status\":\"Nível gravado\"}");
    });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
        // 1. Criamos os documentos JSON
        JsonDocument statusDoc; 
        JsonDocument pinsDoc;

        // 2. Pegamos a String do seu método e transformamos em um objeto JSON usável
        deserializeJson(pinsDoc, accessSys.pinGPIO());
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