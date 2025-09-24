/*
  Control de 5 LEDs en ESP32 con cambio inmediato de orientación
*/

const int ledRojo = 18;
const int ledAmarillo = 19;
const int ledVerde = 21;
const int ledAzul = 22;
const int ledBlanco = 23;
const int boton = 15;

// Array con los LEDs
const int leds[] = {ledRojo, ledAmarillo, ledVerde, ledAzul, ledBlanco};
const int numLeds = 5;

// Variables de control
bool direccionNormal = true;   // true = Rojo→Blanco, false = Blanco→Rojo
bool botonPresionado = false;  // Para detectar flancos

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
  }
  pinMode(boton, INPUT_PULLUP); // Botón con resistencia interna
}

void loop() {
  if (direccionNormal) {
    for (int i = 0; i < numLeds; i++) {
      encenderLed(i);
      if (revisarBoton()) {
        direccionNormal = false;
        break; // Rompe el ciclo y empieza la otra dirección
      }
    }
  } else {
    for (int i = numLeds - 1; i >= 0; i--) {
      encenderLed(i);
      if (revisarBoton()) {
        direccionNormal = true;
        break; // Rompe el ciclo y empieza la otra dirección
      }
    }
  }
}

// Función que enciende un LED y lo apaga tras 500ms
void encenderLed(int indice) {
  digitalWrite(leds[indice], HIGH);
  delay(500);
  digitalWrite(leds[indice], LOW);
}

// Función para revisar el botón con flanco
bool revisarBoton() {
  if (digitalRead(boton) == LOW && !botonPresionado) {
    botonPresionado = true;
    delay(50); // Anti-rebote
    return true;
  }
  if (digitalRead(boton) == HIGH) {
    botonPresionado = false;
  }
  return false;
}
