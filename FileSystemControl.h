// FileSystem.h
#ifndef FILESYSTEMCONTROL_H
#define FILESYSTEMCONTROL_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

// Definição de constantes
#define FILE_PATH "/config.json"

class FileSystemControl {
  private:
    String _ssid;
    String _password;

  public:
    // Construtor
    FileSystemControl(const char* ssid = nullptr, const char* password = nullptr);

    // Inicializa o sistema de arquivos
    bool begin();

    // Métodos de manipulação de JSON
    void saveConfig(JsonDocument& doc);
    bool loadConfig(JsonDocument& doc);
    void fillJson(JsonDocument& doc, const char* ssid, const char* pass);

    // Método principal para persistir as credenciais da classe
    bool credentials();

    void returnObjectData();
    void addPinConfig(int pin = 0,  String mode = "OUTPUT", String state = "LOW");
    void factoryReset(); // DELETA ARQUIVO DE CONFIGURAÇÕES
    
};

#endif