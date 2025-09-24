/*
  Control de 5 LEDs en ESP32
*/

const int ledRojo = 18;
const int ledAmarillo = 19;
const int ledVerde = 21;
const int ledAzul = 22;
const int ledBlanco = 23;

void setup() {
  // Configuración de pines como salida
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  pinMode(ledBlanco, OUTPUT);
}

void loop() {
  // Encendido secuencial de los LEDs
  digitalWrite(ledRojo, HIGH);
  delay(500);
  digitalWrite(ledRojo, LOW);

  digitalWrite(ledAmarillo, HIGH);
  delay(500);
  digitalWrite(ledAmarillo, LOW);

  digitalWrite(ledVerde, HIGH);
  delay(500);
  digitalWrite(ledVerde, LOW);

  digitalWrite(ledAzul, HIGH);
  delay(500);
  digitalWrite(ledAzul, LOW);

  digitalWrite(ledBlanco, HIGH);
  delay(500);
  digitalWrite(ledBlanco, LOW);

  delay(500); // Pausa antes de repetir el ciclo
}
