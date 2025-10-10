// Pines
const int potPin = 34;     // Potenciómetro en pin 34 (entrada ADC)
const int led1 = 22;       // LED 1 en pin 22
const int led2 = 23;       // LED 2 en pin 23

void setup() {
  Serial.begin(115200);     // Inicia comunicación serie
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  int lectura = analogRead(potPin);             // Leer ADC (0-4095)
  float voltaje = (lectura * 3.3) / 4095.0;     // Convertir a voltaje real

  // Mostrar datos en monitor serie
  Serial.print("Lectura ADC: ");
  Serial.print(lectura);
  Serial.print("  Voltaje: ");
  Serial.print(voltaje, 2);   // Mostrar con 2 decimales
  Serial.println(" V");

  // Control de LEDs según el voltaje
  if (voltaje >= 1.0) {
    digitalWrite(led1, HIGH);   // Enciende LED 1
  } else {
    digitalWrite(led1, LOW);    // Apaga LED 1
  }

  if (voltaje >= 2.5) {
    digitalWrite(led2, HIGH);   // Enciende LED 2
  } else {
    digitalWrite(led2, LOW);    // Apaga LED 2
  }

  delay(200); // Pausa para estabilidad
}
