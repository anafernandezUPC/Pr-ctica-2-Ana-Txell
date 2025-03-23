#include <Arduino.h>

// Declaració de variables globals
volatile int interruptCounter;     // Comptador per registrar el nombre d'interrupcions que es produeixen (marcat com a 'volatile' per evitar optimitzacions del compilador)
int totalInterruptCounter;         // Comptador total d'interrupcions que s'han registrat al llarg del temps

// Declarem i inicialitzem un temporitzador (timer) per gestionar les interrupcions
hw_timer_t * timer = NULL;         // Punter al temporitzador que es farà servir per generar les interrupcions
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;  // Màquina de sincronització per accedir a variables compartides de manera segura

// Aquesta és la funció que s'executarà quan s'activi el temporitzador (interrupció)
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);    // Entrar en un bloc crític per protegir la variable compartida
  interruptCounter++;                   // Incrementar el comptador d'interrupcions
  portEXIT_CRITICAL_ISR(&timerMux);     // Sortir del bloc crític
}

void setup() {
  Serial.begin(115200);                 // Inicialitzar la comunicació sèrie per veure les dades al monitor sèrie
  timer = timerBegin(0, 80, true);      // Crear un temporitzador (timer 0), amb un prescaler de 80, mode de comptatge ascendent
  timerAttachInterrupt(timer, &onTimer, true);  // Assignar la funció onTimer com la funció d'interrupció
  timerAlarmWrite(timer, 1000000, true);  // Configurar l'alarma per activar la interrupció cada 1 segon (1000000 microsegons)
  timerAlarmEnable(timer);               // Habilitar l'alarma del temporitzador
}

void loop() {
  if (interruptCounter > 0) {  // Si s'ha produït una interrupció (el comptador és superior a zero)
    portENTER_CRITICAL(&timerMux);     // Entrar en un bloc crític per actualitzar les variables de forma segura
    interruptCounter--;                // Decrementar el comptador d'interrupcions (per processar-les una per una)
    portEXIT_CRITICAL(&timerMux);      // Sortir del bloc crític
    totalInterruptCounter++;           // Incrementar el comptador total d'interrupcions
    Serial.print("An interrupt has occurred. Total number: ");  // Imprimir un missatge al monitor sèrie
    Serial.println(totalInterruptCounter);  // Imprimir el nombre total d'interrupcions processades
  }
}
