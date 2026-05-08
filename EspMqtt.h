#ifndef ESP_MQTT_H
#define ESP_MQTT_H

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif
#include <PubSubClient.h>

class EspMqtt {
  private:
    WiFiClient espClient;
    WiFiClientSecure secureClient; // Cliente para conexões SSL
    PubSubClient client;
    bool _useSsl; // Armazena se deve usar SSL ou não
    const char* _server;
    int _port;
    const char* _topic;
    const char* _clientId;
    const char* _user;
    const char* _passw;
    int _qos;
    unsigned long lastReconnectAttempt;

    void reconnect();

  public:
    // Construtor: Inicializa com os dados básicos
    EspMqtt(const char* server = nullptr, int port = 1883, const char* clientId = "ESP32_Nexus");

    // Inicialização: Define o tópico e a função de callback para mensagens recebidas
    void begin(const char* topic, MQTT_CALLBACK_SIGNATURE);

    // Atualização Dinâmica: Essencial para o novo endpoint /config_mqtt
    // Permite trocar o Broker e Tópico sem resetar o ESP32
    void updateConfig(const char* newServer, int newPort, const char* newTopic, const char* newUser, const char* newPassw, int newQos = 0, bool useSsl = false);
    // Loop principal: Gerencia reconexão não-bloqueante
    void update();

    // Métodos de Publicação
    bool publish(const char* payload);
    bool publishToTopic(const char* customTopic, const char* payload);

    // Getters para validação no log ou no endpoint /status
    const char* getServer() { return _server; }
    const char* getTopic() { return _topic; }
    bool isConnected() { return client.connected(); }
};

#endif