#include "FootSwetch.h"

/**
 * Construtor: Inicializa as variáveis de estado e configura o hardware.
 */
FootSwetch::FootSwetch(int pin, SwetchActions actions) 
    : _pin(pin), _actions(actions) {
    
    pinMode(_pin, INPUT_PULLUP);
    
    // Inicialização segura dos estados
    _pressionado = false;
    _longoExecutado = false;
    _tempoPress = 0;
}

void FootSwetch::setFeedbackPin(int targetPin) {
    _feedbackPin = targetPin;
    if (_feedbackPin != -1) {
        pinMode(_feedbackPin, OUTPUT);
        digitalWrite(_feedbackPin, LOW);
    }
}

/**
 * Lógica principal de detecção de tempo (Não-bloqueante).
 */
void FootSwetch::pinAction() {
    // LOW significa pressionado (devido ao INPUT_PULLUP)
    bool estadoAtual = (digitalRead(_pin) == LOW); 
    // --- LÓGICA DE IDENTIFICAÇÃO (MÉTODO INTERNO) ---
    if (_feedbackPin != -1) {
        // O pino alvo "segue" o estado do botão em tempo real
        digitalWrite(_feedbackPin, estadoAtual ? HIGH : LOW);
    }
    // 1. MOMENTO EM QUE APERTA (Borda de Descida)
    if (estadoAtual && !_pressionado) {
        _pressionado = true;
        _longoExecutado = false;
        _tempoPress = millis();
    }

    // 2. ENQUANTO ESTÁ SEGURANDO (Detecta Longo antes de soltar)
    if (estadoAtual && _pressionado && !_longoExecutado) {
        if (millis() - _tempoPress >= _tmpLongo) {
            
            // Executa ação Longa se existir
            if (_actions.onLong != nullptr) {
                _actions.onLong(_pin);
            } else {
                Serial.printf("Pino %d: Acao LONGA (Sem callback)\n", _pin);
            }
            
            _longoExecutado = true; 
        }
    }
    // 3. MOMENTO EM QUE SOLTA (Borda de subida)
    if (!estadoAtual && _pressionado) {
      unsigned long duracao = millis() - _tempoPress;
      _pressionado = false;

      // Só verifica Curto/Médio se a ação Longa ainda não disparou
      if (!_longoExecutado) { 
        if (duracao > _debounce && duracao < _tmpCurto) {
                
                // Executa ação Curta
                if (_actions.onShort != nullptr) {
                    _actions.onShort(_pin);
                } else {
                    Serial.printf("Pino %d: Acao CURTA\n", _pin);
                }
            }
        else if (duracao >= _tmpCurto) {
          // --- INSIRA AQUI A AÇÃO: PRESSÃO MÉDIA (Entre 0.5s e 2s) ---
          if (_actions.onMedium != nullptr) {
              _actions.onMedium(_pin);
          } else {
              Serial.printf("Pino %d: Acao MEDIA\n", _pin);
          }
        }
      }
    }
  };

/*
// --- EXEMPLO DE USO NO SKETCH ---

// Função que será chamada pelo botão
void acaoLongoPino0(int pinOrigem) {
    // Inverte o estado atual do pino 0 (Toggle)
    digitalWrite(0, !digitalRead(0));
    
    Serial.println("Pino 2 segurado por 5s -> Acionando Pino 0");
}

// 1. Criamos a configuração para o pino 2
SwetchActions configPino2 = {
    .onShort  = nullptr,       // Nada no curto
    .onMedium = nullptr,       // Nada no médio
    .onLong   = acaoLongoPino0 // Nossa função no longo (>5s)
};

// 2. Instanciamos o objeto para o pino 2
FootSwetch botaoPedal(2, configPino2);

void setup() {
    Serial.begin(115200);
    
    // IMPORTANTE: Configure o pino de SAÍDA (alvo)
    pinMode(0, OUTPUT);
    digitalWrite(0, LOW); // Começa desligado
    // MÉTODO NOVO: Define que o Pino 0 identificará o que acontece no Pino 2
    // A partir daqui, a classe assume o controle do Pino 0
    botaoPedal.setIdentificationPin(0);
}

void loop() {
    // O sistema monitora o pino 2 e, após 5s, chama acaoLongoPino0
    botaoPedal.pinAction();
}
*/