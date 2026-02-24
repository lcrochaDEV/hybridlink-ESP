#ifndef FOOTSWETCH_H
#define FOOTSWETCH_H

#if defined(ESP8266)
const int pinoBotao = 4; 
const int pinoLED = 2;   
#elif defined(ESP32)

#endif

#include <Arduino.h>

class FootSwetch {
  public:
    FootSwetch(const char* mode_edit = NULL, const char* mode_loop = NULL);
    void begin();
    void pinAction(int btnId = NULL, int ledId = NULL, int pinMode = NULL);
  private:
    unsigned long tmpInicio;
    const unsigned long tmpLongo = 5000;
    const unsigned long tmpCurto = 500;

  protected:
    const char* mode_edit; 
    const char* mode_loop;
};

#endif