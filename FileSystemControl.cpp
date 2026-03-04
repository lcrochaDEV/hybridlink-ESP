#include "HardwareSerial.h"

// FileSystemControl.cpp
#include "FileSystemControl.h"

FileSystemControl::FileSystemControl(const char* ssid, const char* password)
     : _ssid(ssid ? ssid : ""), _password(password ? password : "") {
  }
// Inicializa o LittleFS (Essencial para resolver o erro de partição)
bool FileSystemControl::begin() {
    Serial.println("\n--- SISTEMA DE ARQUIVOS ---");
    
    // Tenta montar primeiro. Se conseguir, não formata.
    if (!LittleFS.begin()) {
        Serial.println("Falha ao montar LittleFS. Tentando formatar...");
        
        // Verifica se o tamanho da IDE bate com o hardware antes de tentar formatar
        if (ESP.getFlashChipSize() > ESP.getFlashChipRealSize()) {
            Serial.println("ERRO: Flash Size incorreto na IDE!");
            return false;
        }

        if (LittleFS.format()) {
            Serial.println("Formatação concluída com sucesso!");
            return LittleFS.begin();
        } else {
            Serial.println("FALHA CRÍTICA: Não foi possível formatar.");
            return false;
        }
    }

    Serial.println("LittleFS montado e pronto!");
    return true;
}
void FileSystemControl::saveConfig(JsonDocument& doc) {
    File file = LittleFS.open(FILE_PATH, "w");
    if (!file) { 
        Serial.println("Erro ao abrir arquivo para escrita"); 
        return; 
    }
    
    if (serializeJson(doc, file) == 0) {
        Serial.println("Falha ao escrever JSON no arquivo");
    }

    file.close();
    delay(50);
    Serial.println("Configuração salva com sucesso!");
}

bool FileSystemControl::loadConfig(JsonDocument& doc) {
    if (!LittleFS.exists(FILE_PATH)) {
        doc.to<JsonObject>();
        Serial.println("Arquivo de configuração não existe.");
        return false;
    }

    File file = LittleFS.open(FILE_PATH, "r");
    if (!file) {
        doc.to<JsonObject>();
        Serial.println("Falha ao abrir arquivo para leitura"); 
        return false; 
    }

    DeserializationError error = deserializeJson(doc, file);

    file.close();
  
    if (error) {
        Serial.print("Erro no JSON: ");
        doc.to<JsonObject>();
        Serial.println(error.c_str());
        return false;
    }
    
    return true;
}

// Corrigido: Agora o nome coincide com o header e o uso interno
void FileSystemControl::fillJson(JsonDocument& doc, const char* ssid, const char* pass) {
    doc["ssid"] = ssid;
    doc["pass"] = pass;
}

bool FileSystemControl::credentials() {
    JsonDocument doc; 

    loadConfig(doc); // Carrega Objeto
    // Usa as variáveis privadas da classe
    // Verifica se houve mudança real para poupar a vida útil da Flash
    if (doc["ssid"] == _ssid && doc["pass"] == _password) {
        Serial.println("Credenciais idênticas às já salvas. Ignorando escrita.");
        return true;
    }
    fillJson(doc, _ssid.c_str(), _password.c_str());
    saveConfig(doc);
    return true;
}

void FileSystemControl::returnObjectData() {
    // 1. Criamos o "contêiner" para o objeto literal
    JsonDocument config; 

    // 2. Passamos para a classe preencher
    if(loadConfig(config)) {
        Serial.println("Objeto Literal Recebido:");
        
        // 3. Imprimindo o objeto literal formatado
        serializeJsonPretty(config, Serial);
        Serial.println();

    } else {
        Serial.println("Arquivo vazio ou inexistente. Criando padrão...");
        //credentials();
    }
}

void FileSystemControl::addPinConfig(int pin, String mode, String state) {
    JsonDocument doc;
    
    // 1. O loadConfig garante que 'doc' seja o conteúdo atual ou {}
    loadConfig(doc);

    // 2. Acesso seguro ao array "pins"
    // Se a chave não existe, o .to<JsonArray>() a cria. 
    // Se já existe, o .as<JsonArray>() permite adicionar sem limpar os antigos.
    JsonArray pins = doc["pins"].is<JsonArray>() ? 
                     doc["pins"].as<JsonArray>() : 
                     doc["pins"].to<JsonArray>();

    bool found = false;
    for (JsonObject p : pins) {
        if (p["pin"] == pin) {
            p["mode"] = mode;
            p["state"] = state;
            found = true;
            break;
        }
    }

    if (!found) {
        JsonObject pinObj = pins.add<JsonObject>();
        pinObj["pin"] = pin;
        pinObj["mode"] = mode;
        pinObj["state"] = state;
    }

    // 3. Salva o documento completo (WiFi + todos os pinos)
    saveConfig(doc);
    
    Serial.printf("Pino %d processado. Total no arquivo: %d\n", pin, pins.size());
}
// DELETA ARQUIVO DE CONFIGURAÇÕES
void FileSystemControl::factoryReset() {
    if (LittleFS.exists(FILE_PATH)) {
        LittleFS.remove(FILE_PATH);
        Serial.println("Arquivo de configuração deletado! Resetando...");
        ESP.restart(); // Reinicia para aplicar o estado inicial
    }
}

/*
{
  "ssid": "MinhaRede",
  "pass": "12345678",
  "pins": [
    {
      "pin": 0,
      "status": 0,
      "mode": "OUTPUT",
      "state": "LOW"
    }
  ]
}
*/
/*
void setup() {
  //Serial.begin(115200);
  
  if(sysFS.begin()) {
      JsonDocument doc;
      // Só grava as credenciais se o arquivo não puder ser carregado
      if(!sysFS.loadConfig(doc)) {
          sysFS.credentials();
          sysFS.loadConfig(doc); // Carrega após gravar
      }
      
      Serial.print("SSID Ativo: ");
      Serial.println(doc["ssid"].as<const char*>());
  }
}
*/