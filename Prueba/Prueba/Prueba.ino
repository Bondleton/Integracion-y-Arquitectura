#include <WiFi.h>

// 🔹 Cambia estos datos por los de tu red WiFi
const char* ssid     = "TU_RED_WIFI";
const char* password = "TU_CONTRASEÑA_WIFI";

WiFiServer server(80);

// Pin del LED
const int ledPin = 32;
String estadoLED = "APAGADO";

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Conexión WiFi
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado a WiFi!");
  Serial.print("IP de la ESP32: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available(); 
  if (!client) {
    return;
  }

  while(!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Control del LED
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    estadoLED = "ENCENDIDO";
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    estadoLED = "APAGADO";
  }

  // Página web con estilo bonito
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta charset='UTF-8'><title>Control LED ESP32</title>";
  html += "<style>";
  html += "body { background: linear-gradient(135deg, #74ebd5, #9face6); font-family: Arial, sans-serif; text-align: center; color: #333; }";
  html += "h1 { margin-top: 40px; font-size: 36px; color: #222; }";
  html += "p { font-size: 22px; margin: 20px; }";
  html += ".btn { display: inline-block; padding: 20px 40px; margin: 20px; font-size: 20px; font-weight: bold; color: white; border: none; border-radius: 15px; cursor: pointer; transition: 0.3s; }";
  html += ".on { background-color: #28a745; }";
  html += ".off { background-color: #dc3545; }";
  html += ".btn:hover { transform: scale(1.1); opacity: 0.85; }";
  html += ".estado { font-size: 26px; font-weight: bold; margin-top: 30px; }";
  html += "</style></head>";
  html += "<body>";
  html += "<h1>💡 Control del LED con ESP32</h1>";
  html += "<p class='estado'>Estado del LED: <strong>" + estadoLED + "</strong></p>";
  html += "<a href='/LED=ON'><button class='btn on'>ENCENDER</button></a>";
  html += "<a href='/LED=OFF'><button class='btn off'>APAGAR</button></a>";
  html += "</body></html>";

  client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
  client.print(html);
  delay(1);
}
