#ifndef MODULOESP_H
#define MODULOESP_H

#include <Arduino.h>

class ModuloESP {
  public:
    ModuloESP();
      String  modelBoardESP();
      String total_ran();
      String flash_size();
      String menor_ran_size();
      String sketch_Size();
       String pinGPIO();
};

#endif