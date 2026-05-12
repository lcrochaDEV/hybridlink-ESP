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
#include "EspMqtt.h"

#include "UUID.h"

UUID uuid;

AccessControl accessSys; 

EspMqtt mqttService;

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
            // AÇÃO 1: Atualiza o arquivo JSON e o mapa de Mirroring em RAM
            accessSys.setPinState(pin, state); 
            // AÇÃO 2: Aplica fisicamente no pino mestre
            digitalWrite(pin, state);
            // AÇÃO 3: Força o espelhamento agora (faz os escravos seguirem o mestre)
            accessSys.runMirroring();
        
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
        JsonDocument doc;

        // Se carregar o config e ele não for nulo
        if (accessSys.loadConfig(doc)) {
            
            // Sincroniza pinos apenas se o array existir
            if(doc["pins"].is<JsonArray>()){
                for (JsonObject p : doc["pins"].as<JsonArray>()) {
                    if (p.containsKey("pin")) {
                        p["state"] = digitalRead(p["pin"].as<int>());
                    }
                }
            } else {
                doc["pins"].to<JsonArray>(); // Garante [] se não existir
            }

            // Garante que mqtt seja sempre um array, sem duplicar chaves
            if (!doc["mqtt"].is<JsonArray>()) {
                doc["mqtt"].to<JsonArray>();
            }

            String response;
            serializeJson(doc, response);
            request->send(200, "application/json", response);

        } else {
            // Fallback limpo: apenas o básico necessário
            request->send(200, "application/json", "{\"pins\":[],\"mqtt\":[]}");
        }
    });

    server.on("/config_link", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, 
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        
        JsonDocument doc;
        deserializeJson(doc, data, len);

        int pin = doc["pin"];    // Ex: 2
        int targetPin = doc["target"]; // Ex: 0
        bool active = doc["active"];   // true/false

        accessSys.setPinLink(pin, active ? targetPin : -1);

        request->send(200, "application/json", "{\"status\":\"Vínculo atualizado\"}");
    });

    server.on("/config_mqtt", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, 
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        
        static std::vector<uint8_t> buffer;
        if (index == 0) { buffer.clear(); buffer.reserve(total); }
        buffer.insert(buffer.end(), data, data + len);

        if (index + len < total) return; 

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, buffer.data(), buffer.size());
        std::vector<uint8_t>().swap(buffer); // Libera RAM imediatamente

        if (error) return request->send(400);

        // 1. Geração de UUID consistente
        if (!doc.containsKey("uuid")) {
            uuid.generate();
            doc["uuid"] = (char*)uuid.toCharArray(); 
        }

        // 2. Validação de campo obrigatório
        if (doc.containsKey("broker")) {
            
            // EXECUÇÃO ÚNICA: Tenta salvar e verifica o retorno booleano
            if (accessSys.saveMqttFullConfig(doc.as<JsonObject>())) {
                
                // Se salvou, atualiza o serviço ativo imediatamente
                if (doc["active"] | false) {
                    mqttService.updateConfig(
                        doc["broker"] | "", 
                        doc["port"]   | 1883, 
                        doc["topic"]  | "", 
                        doc["user"]   | "", 
                        doc["passw"]  | ""
                    );
                    Serial.println(F("Serviço MQTT atualizado: Perfil Ativo."));
                } else {
                    Serial.println(F("Perfil salvo em background (Inativo)."));
                }
                
                // Resposta JSON profissional de sucesso
                String responseUUID = doc["uuid"].as<String>();
                request->send(200, "application/json", "{\"status\":\"success\",\"uuid\":\"" + responseUUID + "\"}");
                
            } else {
                // Se cair aqui, ou o limite de 10 perfis estourou ou é duplicado
                request->send(409, "application/json", "{\"status\":\"error\",\"message\":\"Conflito: IP/Topico ja cadastrado ou limite atingido\"}");
            }
        } else {
            request->send(422, "application/json", "{\"status\":\"error\",\"message\":\"Falta o campo broker\"}");
        }
    });

  // Inicia o Servidor 
  server.begin();
  Serial.println("Servidor HTTP Async Iniciado!");
}

#endif