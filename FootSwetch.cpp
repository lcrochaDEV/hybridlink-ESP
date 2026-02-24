FootSwetch::FootSwetch(const char mode_edit = NULL, const char mode_loop = NULL)
  : mode_edit(mode_edit), mode_loop(mode_loop) {
  
  }

void FootSwetch::begin() {
  Serial.begin(115200); // Essencial para ver o status no Monitor Serial
  pinMode(pinoBotao, INPUT_PULLUP); 
  pinMode(pinoLED, OUTPUT);
  digitalWrite(pinoLED, LOW);
  Serial.println("Nexus FootSwitch: Sistema Iniciado...");
}

void FootSwetch::pinAction(int btnId, int ledId) {
  // Se os parâmetros forem NULL, usamos os pinos padrão do .h
  int actualBtn = (btnId == 0) ? pinoBotao : btnId;
  int actualLed = (ledId == 0) ? pinoLED : ledId;

  if (digitalRead(btnId) == LOW) {
    tmpInicio = millis();
    
     // Liga enquanto está segurando para dar feedback
    digitalWrite(ledId, HIGH);
    
    // Aguarda soltar ou atingir tempo longo
    while (digitalRead(btnId) == LOW && (millis() - tmpInicio < tmpLongo));
    
    unsigned long duracao = millis() - tmpInicio;

    if (duracao < tmpCurto) {
      // AÇÃO CURTA: Pisca e apaga
      Serial.println("Nexus: Clique Curto");
      digitalWrite(ledId, LOW); delay(100);
      digitalWrite(ledId, HIGH); delay(100);
      digitalWrite(ledId, LOW);
    } 
    
    else {
      // Caso solte entre 300ms e 2000ms
      digitalWrite(ledId, LOW);
      Serial.println("Clique Medio - Apagou");
    }

    // Espera soltar o botão caso ainda esteja pressionado
    while (digitalRead(btnId) == LOW); 
    delay(50); // Debounce
  }
}