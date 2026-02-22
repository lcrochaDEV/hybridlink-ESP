#ifndef CONNECTWIFI_H
#define CONNECTWIFI_H

#include <Arduino.h>

class ConnectWifi {
  public:
    ConnectWifi(const char* ssid = nullptr, const char* password = nullptr);
    void connectionsMethod();
    bool connect_status();
    void accesspoint();
    
  private:
    int maxTentativas = 20;
    int tentativaAtual = 0;

  protected:
    const char* ssid; 
    const char* password;
};
 
#endif