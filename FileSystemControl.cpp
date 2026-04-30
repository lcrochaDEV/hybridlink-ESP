#include "HardwareSerial.h"

// FileSystemControl.cpp
#include "FileSystemControl.h"

FileSystemControl::FileSystemControl(const char* ssid, const char* password)
     : _ssid(ssid ? ssid : ""), _password(password ? password : "") {
  }

/**
 * @brief Inicializa o LittleFS (Essencial para resolver o erro de partição)
 */
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
    syncHardwareWithDisk();
    return true;
}

/**
 * @brief Garante que os estados físicos dos pinos correspondam ao JSON no boot.
 */
void FileSystemControl::syncHardwareWithDisk() {
    JsonDocument doc;
    if (!loadConfig(doc) || !doc["pins"].is<JsonArray>()) return;

    for (JsonObject p : doc["pins"].as<JsonArray>()) {
        int pin   = p["pin"];
        int mode  = p["mode"];
        int level = p["level"];
        int state = p["state"]; // Adicionado o estado salvo

        if (mode != MODE_KEEP) {
            #if defined(ESP8266)
            if (mode == MODE_INPUT_PULLDOWN && pin == 16) pinMode(pin, INPUT_PULLDOWN_16);
            else pinMode(pin, (uint8_t)mode);
            #else
            pinMode(pin, (uint8_t)mode);
            #endif
        }

        if (mode == MODE_OUTPUT) {
            // Se o state for válido (não for PIN_KEEP), usamos ele, senão usamos o level
            int finalState = (level != PIN_KEEP) ? level : state;
            digitalWrite(pin, finalState);
        }
    }
    Serial.println("Hardware sincronizado (Mode/Level/State).");
}

/**
 * @brief Salva o JSON da memória para o disco.
 */
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

/**
 * @brief Carrega o JSON do disco para a memória.
 */
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

void FileSystemControl::_addPinConfig(int pin, PinMode_t mode, int state, int level, int vincularpin) {
    JsonDocument doc;
    loadConfig(doc);

    JsonArray pins = doc["pins"].is<JsonArray>() ? 
                     doc["pins"].as<JsonArray>() : 
                     doc["pins"].to<JsonArray>();

    JsonObject target;
    bool found = false;

    for (JsonObject p : pins) {
        if (p["pin"] == pin) {
            target = p;
            found = true;
            break;
        }
    }

    if (!found) {
        target = pins.add<JsonObject>();
        target["pin"] = pin;
        target["mode"] = (int)MODE_OUTPUT; 
        target["state"] = 0;
        target["level"] = 0;
        target["vincularpin"] = -1; // Valor padrão: sem vínculo
    }

    // A lógica profissional de "Patch" (Só altera o que foi solicitado)
    if (mode != MODE_KEEP)  target["mode"] = (int)mode;
    if (state != PIN_KEEP)  target["state"] = state;
    if (level != PIN_KEEP)  target["level"] = level;
    if (vincularpin != LINK_KEEP) target["vincularpin"] = vincularpin; // Grava o novo vínculo

    saveConfig(doc);
}

void FileSystemControl::setPinMode(int pin, PinMode_t mode) {
    _addPinConfig(pin, mode, PIN_KEEP, PIN_KEEP, LINK_KEEP);
}

void FileSystemControl::setPinLevel(int pin, int level) {
    _addPinConfig(pin, MODE_KEEP, PIN_KEEP, level, LINK_KEEP);
}

void FileSystemControl::setPinState(int pin, int state) {
    _addPinConfig(pin, MODE_KEEP, state, PIN_KEEP, LINK_KEEP);
}

/**
 * @brief Define o vínculo de espelhamento entre dois pinos no JSON.
 */
void FileSystemControl::setPinLink(int pin, int vincularpin) {
    // Chama o addPinConfig mantendo tudo igual e alterando apenas o vínculo
    _addPinConfig(pin, MODE_KEEP, PIN_KEEP, PIN_KEEP, vincularpin);
}

/**
 * @brief Busca no JSON qual pino de saída está vinculado a um pino de origem.
    MANIPULAÇÃO FÍSICA DO MODULO 
 */
int FileSystemControl::getLinkedPin(int originPin) {
    JsonDocument doc;
    if (!loadConfig(doc)) return -1;

    if (doc["pins"].is<JsonArray>()) {
        JsonArray pins = doc["pins"].as<JsonArray>();
        for (JsonObject p : pins) {
            // "Me dê o pino que está vinculado ao pino de origem X"
            if (p.containsKey("vincularpin") && p["vincularpin"].as<int>() == originPin) {
                return p["pin"].as<int>();
            }
        }
    }
    return -1; 
}
void FileSystemControl::runMirroring() {
    // 1. Controle de tempo para não sobrecarregar o processador
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck < 50) return; 
    lastCheck = millis();

    // 2. Carrega a configuração atual do arquivo
    JsonDocument doc;
    if (!loadConfig(doc) || !doc["pins"].is<JsonArray>()) return;
    JsonArray pins = doc["pins"].as<JsonArray>();

    bool mudancaDetectada = false;

    // 3. Primeira Varredura: Identificar quem tem vínculo (Escravos)
    for (JsonObject pinoEscravo : pins) {
        int vPin = pinoEscravo.containsKey("vincularpin") ? pinoEscravo["vincularpin"].as<int>() : -1;

        if (vPin != -1) {
            // 4. Segunda Varredura: Achar o estado do Mestre no objeto
            for (JsonObject pinoMestre : pins) {
                if (pinoMestre["pin"] == vPin) {
                    int estadoMestre = pinoMestre["state"];
                    int estadoAtualEscravo = pinoEscravo["state"];

                    // 5. Sincroniza o Objeto: Se o estado no JSON estiver diferente, atualiza
                    if (estadoAtualEscravo != estadoMestre) {
                        pinoEscravo["state"] = estadoMestre;
                        mudancaDetectada = true; // Marca que o JSON mudou na RAM
                    }

                    // 6. Sincroniza o Hardware: Aplica o estado físico no pino escravo
                    digitalWrite(pinoEscravo["pin"].as<int>(), estadoMestre);
                    break; 
                }
            }
        }
    }

    // 7. Persistência: Se o estado de algum escravo mudou no objeto, salvamos no disco
    if (mudancaDetectada) {
        saveConfig(doc);
        Serial.println("Mirroring: Estados sincronizados no JSON e salvos.");
    }
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
      "state": 0,
      "mode": "OUTPUT",
      "level": "LOW"
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