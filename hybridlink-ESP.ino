#include "ConnectWifi.h"
#include "servidorweb.h"

ConnectWifi connectWifi;

void setup() {
  Serial.begin(115200);
  delay(1000); 

  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  
  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);

  connectWifi.accesspoint();
  startServer();   
}

void loop() {

}