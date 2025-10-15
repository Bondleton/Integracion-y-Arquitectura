import tkinter as tk
from tkinter import ttk, messagebox
import requests
import threading
import time

class ControlSemaforos:
    def __init__(self, root):
        self.root = root
        self.root.title("Control de Semáforos Cruzados - Cliente")
        self.root.geometry("800x600")
        self.root.configure(bg='#f0f0f0')
        
        # Variables
        self.ip_esp32 = ""
        self.sistema_activo = False
        
        # Crear interfaz
        self.crear_interfaz()
        
    def crear_interfaz(self):
        # Frame principal
        main_frame = ttk.Frame(self.root, padding="20")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # Configurar grid
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)
        main_frame.columnconfigure(1, weight=1)
        
        # Configuración de IP
        ttk.Label(main_frame, text="IP de la ESP32:").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.ip_entry = ttk.Entry(main_frame, width=15)
        self.ip_entry.grid(row=0, column=1, sticky=(tk.W, tk.E), pady=5, padx=(5, 0))
        self.ip_entry.insert(0, "192.168.1.100")  # IP por defecto
        
        ttk.Button(main_frame, text="Conectar", 
                  command=self.conectar).grid(row=0, column=2, padx=5, pady=5)
        
        # Estado del sistema
        self.estado_label = ttk.Label(main_frame, text="Estado: No conectado", 
                                     foreground="red", font=('Arial', 12, 'bold'))
        self.estado_label.grid(row=1, column=0, columnspan=3, pady=10)
        
        # Botones de control
        btn_frame = ttk.Frame(main_frame)
        btn_frame.grid(row=2, column=0, columnspan=3, pady=20)
        
        self.btn_iniciar = ttk.Button(btn_frame, text="Iniciar Semáforos", 
                                     command=self.iniciar_sistema, state='disabled')
        self.btn_iniciar.pack(side=tk.LEFT, padx=10)
        
        self.btn_detener = ttk.Button(btn_frame, text="Detener Semáforos", 
                                     command=self.detener_sistema, state='disabled')
        self.btn_detener.pack(side=tk.LEFT, padx=10)
        
        # Frame para los semáforos
        semaforos_frame = ttk.Frame(main_frame)
        semaforos_frame.grid(row=3, column=0, columnspan=3, pady=20)
        
        # Semáforo 1
        semaforo1_frame = ttk.LabelFrame(semaforos_frame, text="Semáforo 1", padding="10")
        semaforo1_frame.grid(row=0, column=0, padx=20, sticky=tk.N)
        
        self.crear_semaforo(semaforo1_frame, "semaforo1")
        
        # Semáforo 2
        semaforo2_frame = ttk.LabelFrame(semaforos_frame, text="Semáforo 2", padding="10")
        semaforo2_frame.grid(row=0, column=1, padx=20, sticky=tk.N)
        
        self.crear_semaforo(semaforo2_frame, "semaforo2")
        
        # Iniciar hilo de actualización
        self.iniciar_actualizacion()
        
    def crear_semaforo(self, parent, nombre):
        # Marco del semáforo
        marco = ttk.Frame(parent)
        marco.pack()
        
        # Crear luces
        luz_roja = tk.Canvas(marco, width=60, height=60, bg='white', highlightthickness=0)
        luz_roja.create_oval(5, 5, 55, 55, fill='gray', outline='black', width=2, tags='luz')
        luz_roja.grid(row=0, column=0, pady=5)
        
        luz_amarilla = tk.Canvas(marco, width=60, height=60, bg='white', highlightthickness=0)
        luz_amarilla.create_oval(5, 5, 55, 55, fill='gray', outline='black', width=2, tags='luz')
        luz_amarilla.grid(row=1, column=0, pady=5)
        
        luz_verde = tk.Canvas(marco, width=60, height=60, bg='white', highlightthickness=0)
        luz_verde.create_oval(5, 5, 55, 55, fill='gray', outline='black', width=2, tags='luz')
        luz_verde.grid(row=2, column=0, pady=5)
        
        # Etiquetas
        ttk.Label(marco, text="Rojo").grid(row=0, column=1, padx=5, sticky=tk.W)
        ttk.Label(marco, text="Amarillo").grid(row=1, column=1, padx=5, sticky=tk.W)
        ttk.Label(marco, text="Verde").grid(row=2, column=1, padx=5, sticky=tk.W)
        
        # Guardar referencias
        if nombre == "semaforo1":
            self.luz_roja1 = luz_roja
            self.luz_amarilla1 = luz_amarilla
            self.luz_verde1 = luz_verde
        else:
            self.luz_roja2 = luz_roja
            self.luz_amarilla2 = luz_amarilla
            self.luz_verde2 = luz_verde
    
    def actualizar_luz(self, canvas, encendida, color):
        color_fill = color if encendida else 'gray'
        canvas.itemconfig('luz', fill=color_fill)
    
    def conectar(self):
        self.ip_esp32 = self.ip_entry.get().strip()
        if not self.ip_esp32:
            messagebox.showerror("Error", "Por favor ingresa la IP de la ESP32")
            return
            
        try:
            response = requests.get(f"http://{self.ip_esp32}/", timeout=5)
            if response.status_code == 200:
                self.estado_label.config(text="Estado: Conectado", foreground="green")
                self.btn_iniciar.config(state='normal')
                self.btn_detener.config(state='normal')
                messagebox.showinfo("Conexión Exitosa", "Conectado a la ESP32 correctamente")
            else:
                messagebox.showerror("Error", "No se pudo conectar a la ESP32")
        except Exception as e:
            messagebox.showerror("Error", f"No se pudo conectar: {str(e)}")
    
    def iniciar_sistema(self):
        try:
            response = requests.get(f"http://{self.ip_esp32}/iniciar", timeout=5)
            if response.status_code == 200:
                self.sistema_activo = True
        except Exception as e:
            messagebox.showerror("Error", f"Error al iniciar: {str(e)}")
    
    def detener_sistema(self):
        try:
            response = requests.get(f"http://{self.ip_esp32}/detener", timeout=5)
            if response.status_code == 200:
                self.sistema_activo = False
        except Exception as e:
            messagebox.showerror("Error", f"Error al detener: {str(e)}")
    
    def actualizar_estado(self):
        if not self.ip_esp32:
            return
            
        try:
            response = requests.get(f"http://{self.ip_esp32}/estado", timeout=5)
            if response.status_code == 200:
                data = response.json()
                
                # Actualizar estado
                estado_texto = "Estado: Activo" if data['activo'] else "Estado: Inactivo"
                color_estado = "green" if data['activo'] else "red"
                self.estado_label.config(text=estado_texto, foreground=color_estado)
                
                # Actualizar semáforo 1
                self.actualizar_luz(self.luz_roja1, data['rojo1'], 'red')
                self.actualizar_luz(self.luz_amarilla1, data['amarillo1'], 'yellow')
                self.actualizar_luz(self.luz_verde1, data['verde1'], 'green')
                
                # Actualizar semáforo 2
                self.actualizar_luz(self.luz_roja2, data['rojo2'], 'red')
                self.actualizar_luz(self.luz_amarilla2, data['amarillo2'], 'yellow')
                self.actualizar_luz(self.luz_verde2, data['verde2'], 'green')
                
        except Exception:
            # Silenciar errores de conexión durante la actualización
            pass
    
    def iniciar_actualizacion(self):
        def loop_actualizacion():
            while True:
                self.actualizar_estado()
                time.sleep(1)  # Actualizar cada segundo
                
        thread = threading.Thread(target=loop_actualizacion, daemon=True)
        thread.start()

if __name__ == "__main__":
    root = tk.Tk()
    app = ControlSemaforos(root)
    root.mainloop()