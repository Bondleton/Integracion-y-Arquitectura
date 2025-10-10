const int potPin = 34;  // Entrada analógica del potenciómetro
const int ledPin = 23;  // Salida LED

int potValue = 0;
int dutyCycle = 0; // Ciclo de trabajo PWM (0–255)

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // Leer el valor del potenciómetro (0-4095)
  potValue = analogRead(potPin);

  // Escalar a ciclo de trabajo (0-255)
  dutyCycle = map(potValue, 0, 4095, 0, 255);

  // PWM por software
  for (int i = 0; i < 255; i++) {
    if (i < dutyCycle) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
    delayMicroseconds(50); // Ajusta para cambiar frecuencia PWM
  }

  Serial.print("Potenciometro: ");
  Serial.print(potValue);
  Serial.print(" | Ciclo: ");
  Serial.println(dutyCycle);
}
