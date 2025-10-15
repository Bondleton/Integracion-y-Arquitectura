/*
  ESP32 - Control de 5 LEDs sincronizados con animación web
  Orden de LEDs físicos: Verde, Amarillo, Rojo, Blanco, Azul
  - Respuesta inmediata a los botones de dirección
  - Animación web reflejando el LED encendido en tiempo real
*/

#include <WiFi.h>

// --- Configuración WiFi ---
const char* ssid = "BUAP_Trabajadores";
const char* password = "BuaPW0rk.2017";

// --- Pines de LEDs físicos ---
const int leds[] = {18, 19, 21, 22, 23}; // Verde, Amarillo, Rojo, Blanco, Azul
const int numLeds = 5;

// --- Variables de control ---
bool direccionNormal = true;
int ledActual = 0;
unsigned long anteriorMillis = 0;
const unsigned long intervalo = 2000; // 2 segundos entre LEDs

// --- Servidor Web ---
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Configurar LEDs como salida
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  // Conexión WiFi
  Serial.print("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Conectado!");
  Serial.print("🌐 IP del servidor: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  // --- Servidor web ---
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Cambiar dirección según botón
    if (request.indexOf("/izquierda") != -1) direccionNormal = true;
    if (request.indexOf("/derecha") != -1) direccionNormal = false;

    // Petición para estado actual
    if (request.indexOf("/estado") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.println(ledActual);
      client.stop();
      return;
    }

    // Página principal con animación
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE html><html><head>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
    client.println("<title>Control LEDs ESP32</title>");
    client.println("<style>");
    client.println("body{font-family:Arial;text-align:center;background:#222;color:white;}");
    client.println(".leds{display:flex;justify-content:center;margin:20px;}");
    client.println(".led{width:40px;height:40px;border-radius:50%;margin:10px;background:#555;transition:opacity 0.2s;}");
    client.println("button{font-size:18px;padding:10px 20px;margin:10px;border:none;border-radius:8px;}");
    client.println(".izq{background:#3c8d40;color:white;} .der{background:#a93a3a;color:white;}");
    client.println("</style></head><body>");
    client.println("<h1>Secuencia de LEDs ESP32</h1>");
    client.println("<div class='leds'>");
    client.println("<div id='led0' class='led' style='background:green;'></div>");
    client.println("<div id='led1' class='led' style='background:yellow;'></div>");
    client.println("<div id='led2' class='led' style='background:red;'></div>");
    client.println("<div id='led3' class='led' style='background:white;'></div>");
    client.println("<div id='led4' class='led' style='background:blue;'></div>");
    client.println("</div>");
    client.println("<button class='izq' onclick=\"fetch('/izquierda')\">Izquierda a Derecha</button>");
    client.println("<button class='der' onclick=\"fetch('/derecha')\">Derecha a Izquierda</button>");
    client.println("<script>");
    client.println("let numLeds=5;");
    client.println("function actualizar(){fetch('/estado').then(r=>r.text()).then(i=>{");
    client.println("for(let j=0;j<numLeds;j++){document.getElementById('led'+j).style.opacity=(j==i?1:0.2);}");
    client.println("setTimeout(actualizar,1100);"); // actualización rápida
    client.println("});}");
    client.println("actualizar();");
    client.println("</script>");
    client.println("</body></html>");
    client.stop();
  }

  // --- Secuencia no bloqueante sincronizada ---
  unsigned long actualMillis = millis();
  if (actualMillis - anteriorMillis >= intervalo) {
    anteriorMillis = actualMillis;

    // Apagar todos los LEDs
    for (int i = 0; i < numLeds; i++) digitalWrite(leds[i], LOW);

    // Encender LED actual
    digitalWrite(leds[ledActual], HIGH);

    // Actualizar LED siguiente
    if (direccionNormal) {
      ledActual++;
      if (ledActual >= numLeds) ledActual = 0;
    } else {
      ledActual--;
      if (ledActual < 0) ledActual = numLeds - 1;
    }
  }
}
