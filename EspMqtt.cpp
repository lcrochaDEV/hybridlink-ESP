#include "HardwareSerial.h"
#include "EspMqtt.h"

void processarMensagemMqtt(char* topic, byte* payload, unsigned int length) {
    // 1. O Tópico: diz-te de onde veio a ordem
    Serial.print("Tópico: ");
    Serial.println(topic);

    // 2. O Payload: é o conteúdo "bruto" (em bytes)
    // Precisamos de o converter para algo legível, como uma String
    String mensagem = "";
    for (int i = 0; i < length; i++) {
        mensagem += (char)payload[i];
    }

    // 3. A Lógica: o que o hardware deve fazer
    if (mensagem == "ON") {
        digitalWrite(0, HIGH); 
    } else if (mensagem == "OFF") {
        digitalWrite(0, LOW);
    }
}


EspMqtt::EspMqtt(const char* server, int port, const char* clientId) 
    : client(espClient), _server(server), _port(port), _clientId(clientId), 
      _user(""), _passw(""), _qos(0), _useSsl(false) {
    lastReconnectAttempt = 0;
}

void EspMqtt::begin() {
    // Garante que o servidor seja configurado antes de qualquer tentativa
    if (_server != "") {
        client.setServer(_server, _port);
        client.setCallback(processarMensagemMqtt);
        Serial.printf("MQTT Iniciado para o servidor: %s\n", _server);
    } else {
        Serial.println("Erro: Tentativa de begin sem servidor configurado!");
    }
}

void EspMqtt::updateConfig(const char* newServer, int newPort, const char* newTopic, 
                           const char* newUser, const char* newPassw, int newQos, bool useSsl) {
    _server = newServer;
    _port = newPort;
    _topic = newTopic;
    _user = newUser;
    _passw = newPassw;
    _qos = newQos;
    _useSsl = useSsl;

    // Lógica para alternar o cliente de rede
    if (_useSsl) {
        // No ESP32, setInsecure() permite conectar via SSL sem validar o certificado CA
        // (Útil para brokers como HiveMQ ou Mosquitto com certificados padrão)
        secureClient.setInsecure(); 
        client.setClient(secureClient);
    } else {
        client.setClient(espClient);
    }

    if (client.connected()) client.disconnect();
    
    client.setServer(_server, _port);
    Serial.printf("Configuração MQTT Atualizada. SSL: %s\n", _useSsl ? "Sim" : "Não");
}
void EspMqtt::reconnect() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Aguardando WiFi para conectar MQTT...");
        return;
    }
    Serial.printf("Tentando MQTT em %s:%d...\n", _server, _port);
    
    // Tenta conectar com usuário e senha
    bool connected = (_user != "") ? 
        client.connect(_clientId, _user, _passw) : 
        client.connect(_clientId);

    if (connected) {
        Serial.println("MQTT Conectado!");
        client.subscribe(_topic, _qos);
    } else {
        Serial.printf("Falha rc=%d. Tentando em 5s\n", client.state());
    }
}
void EspMqtt::update() {
    if (!client.connected()) {
        unsigned long now = millis();
        // Verifica se passaram 5 segundos desde a última tentativa
        if (now - lastReconnectAttempt > 5000) {
            lastReconnectAttempt = now;
            reconnect();
        }
    } else {
        // Mantém o processamento interno da biblioteca
        client.loop();
    }
}
void EspMqtt::forceUpdate() { lastReconnectAttempt = 0; }

bool EspMqtt::publish(const char* payload) {
    if (client.connected()) return client.publish(_topic, payload);
    return false;
}

bool EspMqtt::publishToTopic(const char* customTopic, const char* payload) {
    if (client.connected()) return client.publish(customTopic, payload);
    return false;
}
void EspMqtt::disconnect() {
    if (client.connected()) {
        client.disconnect();
        Serial.println(F("MQTT Desconectado manualmente via Interface."));
    }
    lastReconnectAttempt = millis();
}