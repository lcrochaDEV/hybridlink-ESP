#include "FootSwetch.h"

FootSwetch::FootSwetch() : tmpInicio(0) {}


void FootSwetch::pinAction(int btnId, int ledId) {
  if (digitalRead(btnId) == LOW) {
    tmpInicio = millis();
    
     // Liga enquanto está segurando para dar feedback
    digitalWrite(ledId, HIGH);
    
    // Aguarda soltar ou atingir tempo longo
    while (digitalRead(btnId) == LOW && (millis() - tmpInicio < tmpLongo));
    
    long duracao = millis() - tmpInicio;

    if (duracao < tmpCurto) {
      // AÇÃO CURTA: Pisca e apaga
      Serial.println("Clique Curto - Piscando");
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