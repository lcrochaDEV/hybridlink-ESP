#ifndef CONSOLE_H
#define CONSOLE_H

#include <Arduino.h>

#include "AccessControl.h"

class Console: public AccessControl {
  public:
    Console(const char* consoleText = nullptr);
    //MENSAGEM DE TODO O PROGRAMA
    void helloWord(const char* consoleText = "Hello Mochi");
    void menssageViewMsg(const char* consoleText = "Hello Mochi");
    void consoleView();

  private:
    const char* consoleText;
    void commands_envio(const String& consoleText);
};
 
#endif