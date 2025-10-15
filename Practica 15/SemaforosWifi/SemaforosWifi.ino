#include <WiFi.h>
#include <WebServer.h>

// Configuración de WiFi
const char* ssid = "FLORESTOTALPLAY_2.4Gnormal";
const char* password = "Christian13";

WebServer server(80);

// Pines de los Semaforos
const int ledRojo1 = 21;
const int ledAmarillo1 = 19;
const int ledVerde1 = 18;

const int ledRojo2 = 25;
const int ledAmarillo2 = 23;
const int ledVerde2 = 22;

// Variables de control
bool sistemaActivo = false;
bool amarilloManual = false;
unsigned long tiempoAnterior = 0;
unsigned long tiempoAmarilloAnterior = 0;
int estadoSemaforo = 0;
bool estadoAmarillo = false;

void setup() {
  Serial.begin(115200);
  
  // Configuración de pines
  pinMode(ledRojo1, OUTPUT);
  pinMode(ledAmarillo1, OUTPUT);
  pinMode(ledVerde1, OUTPUT);
  pinMode(ledRojo2, OUTPUT);
  pinMode(ledAmarillo2, OUTPUT);
  pinMode(ledVerde2, OUTPUT);
  
  // Apagar todos los LEDs inicialmente
  apagarTodos();
  
  // Conectar a WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Configurar rutas del servidor
  server.on("/", handleRoot);
  server.on("/iniciar", handleIniciar);
  server.on("/detener", handleDetener);
  server.on("/amarillo-on", handleAmarilloOn);
  server.on("/amarillo-off", handleAmarilloOff);
  server.on("/estado", handleEstado);
  
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
  
  if (sistemaActivo && !amarilloManual) {
    controlarSemaforos();
  } else if (amarilloManual) {
    controlarAmarilloIntermitente();
  }
}

void apagarTodos() {
  digitalWrite(ledRojo1, LOW);
  digitalWrite(ledAmarillo1, LOW);
  digitalWrite(ledVerde1, LOW);
  digitalWrite(ledRojo2, LOW);
  digitalWrite(ledAmarillo2, LOW);
  digitalWrite(ledVerde2, LOW);
}

void controlarAmarilloIntermitente() {
  unsigned long tiempoActual = millis();
  
  // Cambiar estado cada 500ms (1 segundo por ciclo completo)
  if (tiempoActual - tiempoAmarilloAnterior >= 800) {
    tiempoAmarilloAnterior = tiempoActual;
    estadoAmarillo = !estadoAmarillo;
    
    // Aplicar el estado a ambos semáforos
    digitalWrite(ledAmarillo1, estadoAmarillo ? HIGH : LOW);
    digitalWrite(ledAmarillo2, estadoAmarillo ? HIGH : LOW);
    
    // Asegurar que los otros LEDs estén apagados
    digitalWrite(ledRojo1, LOW);
    digitalWrite(ledVerde1, LOW);
    digitalWrite(ledRojo2, LOW);
    digitalWrite(ledVerde2, LOW);
  }
}

void controlarSemaforos() {
  unsigned long tiempoActual = millis();
  
  switch(estadoSemaforo) {
    case 0: // Semáforo 1 verde, Semáforo 2 rojo
      digitalWrite(ledRojo1, LOW);
      digitalWrite(ledAmarillo1, LOW);
      digitalWrite(ledVerde1, HIGH);
      digitalWrite(ledRojo2, HIGH);
      digitalWrite(ledAmarillo2, LOW);
      digitalWrite(ledVerde2, LOW);
      
      if (tiempoActual - tiempoAnterior >= 3000) {
        tiempoAnterior = tiempoActual;
        estadoSemaforo = 1;
      }
      break;
      
    case 1: // Parpadeo verde Semáforo 1
      if (tiempoActual - tiempoAnterior >= 500) {
        tiempoAnterior = tiempoActual;
        static int parpadeos = 0;
        static bool estadoVerde = true;
        
        digitalWrite(ledVerde1, estadoVerde ? HIGH : LOW);
        estadoVerde = !estadoVerde;
        parpadeos++;
        
        if (parpadeos >= 6) { // 3 parpadeos completos (on-off)
          parpadeos = 0;
          estadoSemaforo = 2;
          digitalWrite(ledVerde1, LOW);
        }
      }
      break;
      
    case 2: // Semáforo 1 amarillo
      digitalWrite(ledAmarillo1, HIGH);
      if (tiempoActual - tiempoAnterior >= 3000) {
        tiempoAnterior = tiempoActual;
        estadoSemaforo = 3;
        digitalWrite(ledAmarillo1, LOW);
        digitalWrite(ledRojo1, HIGH);
      }
      break;
      
    case 3: // Semáforo 2 verde, Semáforo 1 rojo
      digitalWrite(ledVerde2, HIGH);
      digitalWrite(ledRojo2, LOW);
      
      if (tiempoActual - tiempoAnterior >= 3000) {
        tiempoAnterior = tiempoActual;
        estadoSemaforo = 4;
      }
      break;
      
    case 4: // Parpadeo verde Semáforo 2
      if (tiempoActual - tiempoAnterior >= 500) {
        tiempoAnterior = tiempoActual;
        static int parpadeos = 0;
        static bool estadoVerde = true;
        
        digitalWrite(ledVerde2, estadoVerde ? HIGH : LOW);
        estadoVerde = !estadoVerde;
        parpadeos++;
        
        if (parpadeos >= 6) { // 3 parpadeos completos
          parpadeos = 0;
          estadoSemaforo = 5;
          digitalWrite(ledVerde2, LOW);
        }
      }
      break;
      
    case 5: // Semáforo 2 amarillo
      digitalWrite(ledAmarillo2, HIGH);
      if (tiempoActual - tiempoAnterior >= 3000) {
        tiempoAnterior = tiempoActual;
        estadoSemaforo = 0;
        digitalWrite(ledAmarillo2, LOW);
        digitalWrite(ledRojo2, HIGH);
      }
      break;
  }
}

void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>Control de Semaforos</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { 
        font-family: Arial, sans-serif; 
        text-align: center; 
        margin: 20px;
        background-color: #f0f0f0;
      }
      .container { 
        max-width: 800px; 
        margin: 0 auto; 
        background: white;
        padding: 20px;
        border-radius: 10px;
        box-shadow: 0 0 10px rgba(0,0,0,0.1);
      }
      .semaforo { 
        display: inline-block; 
        margin: 20px; 
        padding: 15px;
        background: #333;
        border-radius: 10px;
      }
      .luz { 
        width: 50px; 
        height: 50px; 
        border-radius: 50%; 
        margin: 10px;
        background: #555;
        border: 2px solid #000;
        transition: opacity 0.2s ease;
      }
      .rojo { background: red; }
      .amarillo { background: yellow; }
      .verde { background: green; }
      .apagado { background: #555; }
      .luz-apagada {
        opacity: 0.3;
      }
      button { 
        padding: 10px 20px; 
        margin: 10px; 
        font-size: 16px;
        border: none;
        border-radius: 5px;
        cursor: pointer;
        transition: all 0.3s;
      }
      button:hover {
        opacity: 0.8;
        transform: scale(1.05);
      }
      .iniciar { background: #4CAF50; color: white; }
      .detener { background: #f44336; color: white; }
      .amarillo-btn { background: #FFC107; color: black; }
      .amarillo-on { background: #FF9800; color: white; }
      .estado { 
        padding: 10px; 
        margin: 10px; 
        font-weight: bold;
        border-radius: 5px;
      }
      .activo { background: #d4edda; color: #155724; }
      .inactivo { background: #f8d7da; color: #721c24; }
      .manual { background: #fff3cd; color: #856404; }
      .control-section {
        margin: 20px 10px;
        padding: 15px;
        background: #f8f9fa;
        border-radius: 8px;
        border: 1px solid #dee2e6;
        display: inline-block;
        width: 45%;
        vertical-align: top;
        min-height: 150px;
        box-sizing: border-box;
      }
      .controls-container {
        display: flex;
        justify-content: center;
        align-items: stretch;
        flex-wrap: wrap;
        gap: 20px;
        margin: 20px 0;
      }
      @media (max-width: 768px) {
        .control-section {
          width: 100%;
          margin: 10px 0;
        }
        .controls-container {
          flex-direction: column;
          align-items: center;
        }
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h1>Control de Semaforos Cruzados</h1>
      
      <div class="estado" id="estado">Sistema Inactivo</div>
      
      <div class="controls-container">
        <div class="control-section">
          <h3>Control Automatico</h3>
          <button class="iniciar" onclick="iniciarSistema()">Iniciar Semaforos</button>
          <button class="detener" onclick="detenerSistema()">Detener Semaforos</button>
        </div>
        
        <div class="control-section">
          <h3>Control Manual</h3>
          <button class="amarillo-btn" id="btnAmarillo" onclick="toggleAmarillo()">Activar Amarillo Intermitente</button>
        </div>
      </div>
      
      <div>
        <div class="semaforo">
          <h3>Semaforo</h3>
          <h3>1</h3>
          <div class="luz rojo" id="rojo1"></div>
          <div class="luz amarillo" id="amarillo1"></div>
          <div class="luz verde" id="verde1"></div>
        </div>
        
        <div class="semaforo">
          <h3>Semaforo</h3>
          <h3>2</h3>
          <div class="luz rojo" id="rojo2"></div>
          <div class="luz amarillo" id="amarillo2"></div>
          <div class="luz verde" id="verde2"></div>
        </div>
      </div>
    </div>

    <script>
      let amarilloActivo = false;
      
      function actualizarEstado(estado) {
        const elementoEstado = document.getElementById('estado');
        const btnAmarillo = document.getElementById('btnAmarillo');
        const amarillo1 = document.getElementById('amarillo1');
        const amarillo2 = document.getElementById('amarillo2');
        
        if (estado.amarilloManual) {
          elementoEstado.textContent = 'Modo Madrugada - Amarillo Intermitente';
          elementoEstado.className = 'estado manual';
          btnAmarillo.textContent = 'Desactivar Amarillo Intermitente';
          btnAmarillo.className = 'amarillo-on';
          amarilloActivo = true;
          
          // En modo manual, mostrar el estado real del parpadeo
          if (estado.amarillo1) {
            amarillo1.className = 'luz amarillo';
            amarillo2.className = 'luz amarillo';
            amarillo1.classList.remove('luz-apagada');
            amarillo2.classList.remove('luz-apagada');
          } else {
            amarillo1.className = 'luz amarillo luz-apagada';
            amarillo2.className = 'luz amarillo luz-apagada';
          }
          
          // Forzar que los otros LEDs estén apagados
          document.getElementById('rojo1').className = 'luz apagado';
          document.getElementById('verde1').className = 'luz apagado';
          document.getElementById('rojo2').className = 'luz apagado';
          document.getElementById('verde2').className = 'luz apagado';
          
        } else if (estado.activo) {
          elementoEstado.textContent = 'Sistema Activo - Modo Normal';
          elementoEstado.className = 'estado activo';
          btnAmarillo.textContent = 'Activar Amarillo Intermitente';
          btnAmarillo.className = 'amarillo-btn';
          amarilloActivo = false;
          
          // Remover clases de apagado
          amarillo1.classList.remove('luz-apagada');
          amarillo2.classList.remove('luz-apagada');
          
          // Actualizar luces normalmente
          document.getElementById('rojo1').className = estado.rojo1 ? 'luz rojo' : 'luz apagado';
          document.getElementById('amarillo1').className = estado.amarillo1 ? 'luz amarillo' : 'luz apagado';
          document.getElementById('verde1').className = estado.verde1 ? 'luz verde' : 'luz apagado';
          document.getElementById('rojo2').className = estado.rojo2 ? 'luz rojo' : 'luz apagado';
          document.getElementById('amarillo2').className = estado.amarillo2 ? 'luz amarillo' : 'luz apagado';
          document.getElementById('verde2').className = estado.verde2 ? 'luz verde' : 'luz apagado';
          
        } else {
          elementoEstado.textContent = 'Sistema Inactivo';
          elementoEstado.className = 'estado inactivo';
          btnAmarillo.textContent = 'Activar Amarillo Intermitente';
          btnAmarillo.className = 'amarillo-btn';
          amarilloActivo = false;
          
          // Remover clases de apagado
          amarillo1.classList.remove('luz-apagada');
          amarillo2.classList.remove('luz-apagada');
          
          // Apagar todas las luces
          document.getElementById('rojo1').className = 'luz apagado';
          document.getElementById('amarillo1').className = 'luz apagado';
          document.getElementById('verde1').className = 'luz apagado';
          document.getElementById('rojo2').className = 'luz apagado';
          document.getElementById('amarillo2').className = 'luz apagado';
          document.getElementById('verde2').className = 'luz apagado';
        }
      }
      
      function iniciarSistema() {
        fetch('/iniciar')
          .then(response => response.json())
          .then(data => actualizarEstado(data));
      }
      
      function detenerSistema() {
        fetch('/detener')
          .then(response => response.json())
          .then(data => actualizarEstado(data));
      }
      
      function toggleAmarillo() {
        if (amarilloActivo) {
          // Apagar amarillos
          fetch('/amarillo-off')
            .then(response => response.json())
            .then(data => actualizarEstado(data));
        } else {
          // Encender amarillos intermitentes
          fetch('/amarillo-on')
            .then(response => response.json())
            .then(data => actualizarEstado(data));
        }
      }
      
      function actualizarDatos() {
        fetch('/estado')
          .then(response => response.json())
          .then(data => actualizarEstado(data));
      }
      
      // Actualizar cada 300ms para mejor sincronización con el parpadeo
      setInterval(actualizarDatos, 100);
      
      // Cargar estado inicial
      actualizarDatos();
    </script>
  </body>
  </html>
  )rawliteral";
  
  server.send(200, "text/html", html);
}

void handleIniciar() {
  sistemaActivo = true;
  amarilloManual = false;
  tiempoAnterior = millis();
  estadoSemaforo = 0;
  enviarEstado();
}

void handleDetener() {
  sistemaActivo = false;
  amarilloManual = false;
  apagarTodos();
  enviarEstado();
}

void handleAmarilloOn() {
  amarilloManual = true;
  sistemaActivo = false;
  tiempoAmarilloAnterior = millis();
  estadoAmarillo = true;
  // Encender amarillos inicialmente
  digitalWrite(ledAmarillo1, HIGH);
  digitalWrite(ledAmarillo2, HIGH);
  // Apagar otros LEDs
  digitalWrite(ledRojo1, LOW);
  digitalWrite(ledVerde1, LOW);
  digitalWrite(ledRojo2, LOW);
  digitalWrite(ledVerde2, LOW);
  enviarEstado();
}

void handleAmarilloOff() {
  amarilloManual = false;
  apagarTodos();
  enviarEstado();
}

void handleEstado() {
  enviarEstado();
}

void enviarEstado() {
  String json = "{";
  json += "\"activo\":" + String(sistemaActivo ? "true" : "false") + ",";
  json += "\"amarilloManual\":" + String(amarilloManual ? "true" : "false") + ",";
  json += "\"rojo1\":" + String(digitalRead(ledRojo1) ? "true" : "false") + ",";
  json += "\"amarillo1\":" + String(digitalRead(ledAmarillo1) ? "true" : "false") + ",";
  json += "\"verde1\":" + String(digitalRead(ledVerde1) ? "true" : "false") + ",";
  json += "\"rojo2\":" + String(digitalRead(ledRojo2) ? "true" : "false") + ",";
  json += "\"amarillo2\":" + String(digitalRead(ledAmarillo2) ? "true" : "false") + ",";
  json += "\"verde2\":" + String(digitalRead(ledVerde2) ? "true" : "false");
  json += "}";
  
  server.send(200, "application/json", json);
}