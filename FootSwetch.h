#ifndef FOOTSWETCH_H
#define FOOTSWETCH_H

#include <Arduino.h>

typedef void (*FootSwetchAction)(int pin);

struct SwetchActions {
    FootSwetchAction onShort  = nullptr; // Ação para < 500ms
    FootSwetchAction onMedium = nullptr; // Ação para 500ms - 5000ms
    FootSwetchAction onLong   = nullptr; // Ação para > 5000ms
};

class FootSwetch {
  private:
    int _pin;
    int _feedbackPin = -1;
    SwetchActions _actions;
    
    // Configurações de tempo (em milissegundos)
    const unsigned long _tmpCurto = 500;
    const unsigned long _tmpLongo = 5000;
    const unsigned long _debounce = 50;

    // Estados de controle interno
    unsigned long _tempoPress;
    bool _pressionado;
    bool _longoExecutado;

  public:

    FootSwetch(int pin, SwetchActions actions);
    void setFeedbackPin(int targetPin);
    void pinAction();

    void setActions(SwetchActions newActions) { _actions = newActions; }
};

#endif // FOOTSWETCH_H