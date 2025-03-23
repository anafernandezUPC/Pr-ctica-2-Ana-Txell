#include <Arduino.h>

#define LED_PIN 2        // Pin del LED
#define BUTTON_PIN_1 4   // Pin del primer pulsador
#define BUTTON_PIN_2 5   // Pin del segundo pulsador

volatile unsigned long blinkInterval = 1000;  // Intervalo inicial de parpadeo (en milisegundos)
volatile bool ledState = false;  // Estado actual del LED

// Variables para el filtrado de rebotes
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long debounceDelay = 50;  // 50 ms para evitar rebotes

// Variables para el control de los pulsadores
volatile bool buttonState1 = LOW;
volatile bool buttonState2 = LOW;
volatile bool lastButtonState1 = LOW;
volatile bool lastButtonState2 = LOW;

// Función de interrupción para el temporizador
void IRAM_ATTR onTimer() {
  ledState = !ledState;  // Cambiar el estado del LED
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);  // Encender o apagar el LED
}

// Función para manejar la lectura de pulsadores
void IRAM_ATTR button1Interrupt() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastDebounceTime1 > debounceDelay) {
    lastDebounceTime1 = currentMillis;
    buttonState1 = digitalRead(BUTTON_PIN_1);
    if (buttonState1 == HIGH) {
      // Aumentar la frecuencia si se presiona el primer pulsador
      blinkInterval = max(100, blinkInterval - 100);  // Evitar que la frecuencia sea demasiado baja
    }
  }
}

void IRAM_ATTR button2Interrupt() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastDebounceTime2 > debounceDelay) {
    lastDebounceTime2 = currentMillis;
    buttonState2 = digitalRead(BUTTON_PIN_2);
    if (buttonState2 == HIGH) {
      // Disminuir la frecuencia si se presiona el segundo pulsador
      blinkInterval = min(2000, blinkInterval + 100);  // Evitar que la frecuencia sea demasiado alta
    }
  }
}

void setup() {
  // Inicializar el pin del LED como salida
  pinMode(LED_PIN, OUTPUT);

  // Inicializar los pines de los pulsadores como entradas con resistencias pull-up internas
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);

  // Configurar las interrupciones para los pulsadores
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_1), button1Interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_2), button2Interrupt, FALLING);

  // Configurar el temporizador (por ejemplo, usando el temporizador 0)
  TimerGroup *group = TIMERG0;
  Timer0.attachInterrupt(onTimer);  // Asociar la interrupción al temporizador
  Timer0.start((blinkInterval / 1000));  // Establecer el temporizador inicial

  Serial.begin(115200);  // Inicializar la comunicación serial para depuración
}

void loop() {
  // El bucle principal está vacío, ya que las interrupciones manejan el parpadeo del LED y los pulsadores
  delay(10);  // Pequeña pausa para evitar el consumo excesivo de CPU
}