#include "WirelessConnection.h"
#include "servidorweb.h"

WirelessConnection wirelessConnection;

void setup() {
  Serial.begin(115200);
  delay(1000); 

  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);

  wirelessConnection.accesspoint();
  startServer();   
}

void loop() {}