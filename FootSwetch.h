#ifndef FOOTSWETCH_H
#define FOOTSWETCH_H

#include <Arduino.h>


class FootSwetch {
  public:
    FootSwetch();
    void pinAction(int GPIO = 0, int MODE = 0);

  private:
    unsigned long tmpInicio;
    const unsigned long tmpLongo = 5000;
    const unsigned long tmpCurto = 500;

};

#endif