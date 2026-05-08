// FileSystem.h
#ifndef FILESYSTEMCONTROL_H
#define FILESYSTEMCONTROL_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

// Definição de constantes
#define FILE_PATH "/config.json"

enum PinMode_t {
    MODE_INPUT = 0,
    MODE_OUTPUT = 1,
    MODE_INPUT_PULLUP = 2,
    MODE_INPUT_PULLDOWN = 3,
    MODE_KEEP = 99 // O nosso sentinela profissional
};

#define PIN_KEEP -1
#define LINK_KEEP -99

struct PinLink {
    int master;
    int slave;
};

class FileSystemControl {
  private:
    String _ssid;
    String _password;
    void _addPinConfig(int pin = 0, PinMode_t mode = MODE_OUTPUT, int state = 0, int level = 0, int vincularpin = -1);

  public:
    // Construtor
    FileSystemControl(const char* ssid = nullptr, const char* password = nullptr);

    // Inicializa o sistema de arquivos
    bool begin();
    void syncHardwareWithDisk(); // Recuora dados e Recofigura os Pinos
    // Métodos de manipulação de JSON
    void saveConfig(JsonDocument& doc);
    bool loadConfig(JsonDocument& doc);
    void fillJson(JsonDocument& doc, const char* ssid, const char* pass);
    // Inclusão de dados no json  
    void setPinMode(int pin, PinMode_t mode);
    void setPinLevel(int pin, int level);
    void setPinState(int pin, int state);
    void setPinLink(int pin, int vincularpin);
    // Método principal para persistir as credenciais da classe
    bool credentials();
    void returnObjectData();
    int getLinkedPin(int originPin);
    void runMirroring();
    void factoryReset(); // DELETA ARQUIVO DE CONFIGURAÇÕES   
    bool saveMqttFullConfig(JsonObject newConfig); //MQTT SAVE JSON
};

#endif