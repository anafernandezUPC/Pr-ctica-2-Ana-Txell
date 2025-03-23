#include <Arduino.h>

// Definim l'estructura per al boto
struct Button {
  const uint8_t PIN;           // Pin associat al boto
  uint32_t numberKeyPresses;   // Comptador de les vegades que s'ha premut el boto
  bool pressed;                // Estat del boto (si s'ha premut o no)
};

// Inicialitzem el boto
Button button1 = {15, 0, false};  // Assignem el pin 15 i inicialitzem el comptador i l'estat a fals

// Interrupcio que s'executa quan el boto es premut (flanc de baixada)
void IRAM_ATTR isr() {
  button1.numberKeyPresses += 1;  // Augmentem el comptador de premudes
  button1.pressed = true;         // Marquem que el boto ha estat premut
}

void setup() {
  Serial.begin(115200);           // Inicialitzem la comunicacio serial
  pinMode(button1.PIN, INPUT_PULLUP);  // Configurem el pin del boto com a entrada amb resistència interna de pull-up
  attachInterrupt(button1.PIN, isr, FALLING);  // Assignem la interrupcio al pin quan es detecta un flanc de baixada (quan el boto es prem)
}

void loop() {
  // Quan el boto ha estat premut, imprimim el nombre de premudes
  if (button1.pressed) {
    Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
    button1.pressed = false;  // Reiniciem l'estat a fals despres d'imprimir el nombre de premudes
  }

  // Desenganxar la interrupcio despres d'1 minut
  static uint32_t lastMillis = 0;  // Variable per controlar el temps
  if (millis() - lastMillis > 60000) {  // Si ha passat 1 minut (60000 mil·lisegons)
    lastMillis = millis();          // Actualitzem el temps actual
    detachInterrupt(button1.PIN);   // Desenganxem la interrupcio del boto
    Serial.println("Interrupt Detached!");  // Imprimim que la interrupcio ha estat desenganxada
  }
}
