/*
  Control de dos semáforos con alternancia, espera y parpadeo en el LED verde
  Semáforo 1: GPIO 18, 19, 21
  Semáforo 2: GPIO 22, 23, 25
*/

const int ledRojo1 = 21;
const int ledAmarillo1 = 19;
const int ledVerde1 = 18;

const int ledRojo2 = 25;
const int ledAmarillo2 = 23;
const int ledVerde2 = 22;

void setup() {
  // Configuración de pines como salida para ambos semáforos
  pinMode(ledRojo1, OUTPUT);
  pinMode(ledAmarillo1, OUTPUT);
  pinMode(ledVerde1, OUTPUT);

  pinMode(ledRojo2, OUTPUT);
  pinMode(ledAmarillo2, OUTPUT);
  pinMode(ledVerde2, OUTPUT);
}

void loop() {
  // Semáforo 1: verde (espera 3 segundos), luego parpadea 3 veces
  digitalWrite(ledRojo1, LOW);
  digitalWrite(ledAmarillo1, LOW);
  digitalWrite(ledVerde1, HIGH);
  digitalWrite(ledRojo2, HIGH);   // Semáforo 2 rojo
  digitalWrite(ledAmarillo2, LOW);
  digitalWrite(ledVerde2, LOW);

  delay(3000);  // 3 segundos de espera con verde encendido antes de parpadear

  // Parpadeo del verde 3 veces
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledVerde1, HIGH); // Verde encendido
    delay(500);                    // Esperar 500 ms (medio segundo)
    digitalWrite(ledVerde1, LOW);  // Verde apagado
    delay(500);                    // Esperar 500 ms (medio segundo)
  }

  // Cambio a amarillo en Semáforo 1
  digitalWrite(ledVerde1, LOW);
  digitalWrite(ledAmarillo1, HIGH);
  delay(2000); // 2 segundos con amarillo

  // Semáforo 1 rojo y Semáforo 2 verde parpadeando
  digitalWrite(ledAmarillo1, LOW);
  digitalWrite(ledRojo1, HIGH);

  digitalWrite(ledVerde2, HIGH);  // Verde parpadea en Semáforo 2
  digitalWrite(ledRojo2, LOW);
  digitalWrite(ledAmarillo2, LOW);

  delay(3000);  // 3 segundos de espera con verde encendido antes de parpadear

  // Parpadeo del verde 3 veces en Semáforo 2
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledVerde2, HIGH); // Verde encendido
    delay(500);                    // Esperar 500 ms (medio segundo)
    digitalWrite(ledVerde2, LOW);  // Verde apagado
    delay(500);                    // Esperar 500 ms (medio segundo)
  }

  // Cambio a amarillo en Semáforo 2
  digitalWrite(ledVerde2, LOW);
  digitalWrite(ledAmarillo2, HIGH);
  delay(2000); // 2 segundos con amarillo

  // Semáforo 2 rojo
  digitalWrite(ledAmarillo2, LOW);
  digitalWrite(ledRojo2, HIGH);
  
}
