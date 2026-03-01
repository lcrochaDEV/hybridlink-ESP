#ifndef WIRELESSCONNECTION_H
#define WIRELESSCONNECTION_H


#include <Arduino.h>

#include "WifiConnect.h"

class WirelessConnection: public WifiConnect{
  public:
    WirelessConnection (const char* ssid = nullptr, const char* password = nullptr);
};
 
#endif