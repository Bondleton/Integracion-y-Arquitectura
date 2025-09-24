import tkinter as tk
from PIL import ImageTk, Image

# Configuración inicial
estado_led = False  # Estado inicial (apagado)

# Paleta de colores personalizada (puedes modificarla)

COLORES = {
    "fondo": "#2E3440",       # Color de fondo
    "texto": "#E5E9F0",       # Color de texto general
    "boton_encender": "#A3BE8C",  # Botón en modo "encender"
    "boton_apagar": "#BF616A",    # Botón en modo "apagar"
    "estado_encendido": "#8FBCBB", # Color texto "ENCENDIDO"
    "estado_apagado": "#D8DEE9"   # Color texto "APAGADO"
}

# Función para cambiar el estado del LED y el foco
def toggle_led_foco():
    global estado_led, img_led, img_foco, label_led, label_foco
    estado_led = not estado_led  # Cambia el estado
    
    if estado_led:
        boton_control.config(
            text="APAGAR LED Y FOCO",
            bg=COLORES["boton_apagar"],
            fg=COLORES["texto"],
            activebackground="#d08770"  # Color al hacer clic
        )
        img_led = ImageTk.PhotoImage(Image.open("led_on.png").resize((80, 80)))
        img_foco = ImageTk.PhotoImage(Image.open("foco_on.png").resize((150, 150)))
        etiqueta_estado.config(
            text="Estado: ENCENDIDOS",
            fg=COLORES["estado_encendido"]
        )
    else:
        boton_control.config(
            text="ENCENDER LED Y FOCO",
            bg=COLORES["boton_encender"],
            fg=COLORES["texto"],
            activebackground="#a3be8c"  # Color al hacer clic
        )
        img_led = ImageTk.PhotoImage(Image.open("led_off.png").resize((80, 80)))
        img_foco = ImageTk.PhotoImage(Image.open("foco_off.png").resize((150, 150)))
        etiqueta_estado.config(
            text="Estado: APAGADOS",
            fg=COLORES["estado_apagado"]
        )
    
    # Actualizar imágenes
    label_led.config(image=img_led)
    label_foco.config(image=img_foco)

# Crear la ventana principal
ventana = tk.Tk()
ventana.title("Control de LED y Foco")
ventana.geometry("450x350")
ventana.configure(bg=COLORES["fondo"])  # Fondo personalizado

# Cargar imágenes iniciales (apagadas)
img_led = ImageTk.PhotoImage(Image.open("led_off.png").resize((80, 80)))
img_foco = ImageTk.PhotoImage(Image.open("foco_off.png").resize((150, 150)))

# Frame para contener las imágenes
frame_imagenes = tk.Frame(ventana, bg=COLORES["fondo"])
frame_imagenes.pack(pady=20)

# Labels para las imágenes
label_led = tk.Label(
    frame_imagenes,
    image=img_led,
    bg=COLORES["fondo"]
)
label_led.grid(row=0, column=0, padx=20)

label_foco = tk.Label(
    frame_imagenes,
    image=img_foco,
    bg=COLORES["fondo"]
)
label_foco.grid(row=0, column=1, padx=20)

# Etiqueta de estado
etiqueta_estado = tk.Label(
    ventana,
    text="Estado: APAGADOS",
    font=("Arial", 12, "bold"),
    bg=COLORES["fondo"],
    fg=COLORES["estado_apagado"]
)
etiqueta_estado.pack(pady=10)

# Botón de control
boton_control = tk.Button(
    ventana,
    text="ENCENDER LED Y FOCO",
    font=("Arial", 12, "bold"),
    bg=COLORES["boton_encender"],
    fg=COLORES["texto"],
    activebackground="#a3be8c",
    activeforeground=COLORES["texto"],
    command=toggle_led_foco,
    width=30,
    relief="flat",  # Estilo plano sin bordes 3D
    borderwidth=0
)
boton_control.pack(pady=20)

# Ejecutar la aplicación
ventana.mainloop()