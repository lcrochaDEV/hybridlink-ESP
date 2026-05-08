#include "EspMqtt.h"

EspMqtt::EspMqtt(const char* server, int port, const char* clientId) 
    : client(espClient), _server(server), _port(port), _clientId(clientId), 
      _user(""), _passw(""), _qos(0), _useSsl(false) {
    lastReconnectAttempt = 0;
}

void EspMqtt::begin(const char* topic, MQTT_CALLBACK_SIGNATURE) {
    _topic = topic;
    client.setServer(_server, _port);
    client.setCallback(callback);
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

bool EspMqtt::publish(const char* payload) {
    if (client.connected()) {
        return client.publish(_topic, payload);
    }
    return false;
}

bool EspMqtt::publishToTopic(const char* customTopic, const char* payload) {
    if (client.connected()) {
        return client.publish(customTopic, payload);
    }
    return false;
}
