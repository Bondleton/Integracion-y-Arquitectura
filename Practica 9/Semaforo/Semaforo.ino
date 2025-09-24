/*
  Semáforo en ESP32 con parpadeo del LED verde
*/

const int ledRojo = 21;
const int ledAmarillo = 19;
const int ledVerde = 18;

void setup() {
  // Configuración de pines como salida
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
}

void loop() {

  // Fase 0: Verde encendido
  digitalWrite(ledRojo, LOW);     // Asegurarse de que el rojo esté apagado
  digitalWrite(ledVerde, HIGH);   // Encender verde
  delay(2000);                    // 2 segundos con verde encendido
  
  // Fase 1: Verde parpadea 3 veces
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledVerde, HIGH);  // Encender verde
    delay(500);                    // Esperar 500 ms (medio segundo)
    digitalWrite(ledVerde, LOW);   // Apagar verde
    delay(500);                    // Esperar 500 ms (medio segundo)
  }

  // Fase 2: Amarillo encendido
  digitalWrite(ledVerde, LOW);     // Asegurarse de que el verde esté apagado
  digitalWrite(ledAmarillo, HIGH); // Encender amarillo
  digitalWrite(ledRojo, LOW);      // Asegurarse de que el rojo esté apagado
  delay(2000);                     // 2 segundos con amarillo encendido

  // Fase 3: Rojo encendido
  digitalWrite(ledVerde, LOW);     // Asegurarse de que el verde esté apagado
  digitalWrite(ledAmarillo, LOW);  // Apagar amarillo
  digitalWrite(ledRojo, HIGH);     // Encender rojo
  delay(2000);                     // 2 segundos con rojo encendido
}
