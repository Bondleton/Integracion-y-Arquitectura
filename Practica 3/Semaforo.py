import tkinter as tk
from PIL import ImageTk, Image

# Paleta de colores personalizada
COLORES = {
    "fondo": "#435c6f",          
    "fondo_semaforo": "#2E3440", 
    "poste": "#4F4F4F",          
    "texto": "#E5E9F0",
    "estado_encendido": "#8FBCBB",
    "estado_apagado": "#D8DEE9"
}

# Secuencia del semáforo (verde → amarillo → rojo)
secuencia = ["verde", "amarillo", "rojo"]
tiempos = {
    "verde": 3000,    # 3 segundos (incluye parpadeo)
    "amarillo": 2000, # 2 segundos
    "rojo": 4000      # 4 segundos
}

# Estado actual
indice = 0  
parpadeos_restantes = 0  
verde_encendido = False  

def actualizar_semaforo():
    global indice, parpadeos_restantes

    color_actual = secuencia[indice]

    # Apagar todos primero
    for c in secuencia:
        labels[c].config(image=imagenes_apagadas[c])

    if color_actual == "verde":
        # Iniciar parpadeo en verde
        parpadeos_restantes = 4  # 4 cambios = 2 parpadeos
        parpadear_verde()
    else:
        # Encender color fijo
        labels[color_actual].config(image=imagenes_encendidas[color_actual])
        etiqueta_estado.config(
            text=f"Estado: {color_actual.upper()} ENCENDIDO",
            fg=COLORES["estado_encendido"]
        )
        # Pasar al siguiente color después de su tiempo
        indice = (indice + 1) % len(secuencia)
        ventana.after(tiempos[color_actual], actualizar_semaforo)

def parpadear_verde():
    global parpadeos_restantes, verde_encendido, indice

    if parpadeos_restantes > 0:
        if verde_encendido:
            labels["verde"].config(image=imagenes_apagadas["verde"])
            etiqueta_estado.config(text="Estado: VERDE APAGADO", fg=COLORES["estado_apagado"])
        else:
            labels["verde"].config(image=imagenes_encendidas["verde"])
            etiqueta_estado.config(text="Estado: VERDE ENCENDIDO", fg=COLORES["estado_encendido"])

        verde_encendido = not verde_encendido
        parpadeos_restantes -= 1
        ventana.after(500, parpadear_verde)  # cada medio segundo cambia
    else:
        # Terminar en verde encendido fijo un rato
        labels["verde"].config(image=imagenes_encendidas["verde"])
        etiqueta_estado.config(text="Estado: VERDE ENCENDIDO", fg=COLORES["estado_encendido"])
        indice = (indice + 1) % len(secuencia)
        ventana.after(1000, actualizar_semaforo)  # 1 segundo más en verde fijo

# Crear ventana principal
ventana = tk.Tk()
ventana.title("Semáforo con Parpadeo Verde")
ventana.geometry("250x800")
ventana.configure(bg=COLORES["fondo"])

# Cargar imágenes de focos apagados y encendidos
imagenes_apagadas = {
    "verde": ImageTk.PhotoImage(Image.open(r"Practica 3\verde_off.png").resize((100, 100))),
    "amarillo": ImageTk.PhotoImage(Image.open(r"Practica 3\amarillo_off.png").resize((100, 100))),
    "rojo": ImageTk.PhotoImage(Image.open(r"Practica 3\rojo_off.png").resize((100, 100)))
}

imagenes_encendidas = {
    "verde": ImageTk.PhotoImage(Image.open(r"Practica 3\verde_on.png").resize((100, 100))),
    "amarillo": ImageTk.PhotoImage(Image.open(r"Practica 3\amarillo_on.png").resize((100, 100))),
    "rojo": ImageTk.PhotoImage(Image.open(r"Practica 3\rojo_on.png").resize((100, 100)))
}

# Frame simulando la caja del semáforo
frame_semaforo = tk.Frame(ventana, bg=COLORES["fondo_semaforo"], bd=5, relief="ridge")
frame_semaforo.pack(pady=5)

# Labels para los focos en vertical
labels = {}
for i, color in enumerate(secuencia):
    labels[color] = tk.Label(frame_semaforo, image=imagenes_apagadas[color], bg=COLORES["fondo_semaforo"])
    labels[color].grid(row=i, column=0, pady=10)

# Poste del semáforo (Canvas con rectángulo gris)
canvas_poste = tk.Canvas(ventana, width=30, height=200, bg=COLORES["fondo"], highlightthickness=0)
canvas_poste.pack()
canvas_poste.create_rectangle(10, 0, 20, 200, fill=COLORES["poste"], outline=COLORES["poste"])

# Etiqueta de estado
etiqueta_estado = tk.Label(
    ventana,
    text="Estado: TODOS APAGADOS",
    font=("Arial", 12, "bold"),
    bg=COLORES["fondo"],
    fg=COLORES["estado_apagado"]
)
etiqueta_estado.pack(pady=10)

# Iniciar la secuencia automática
ventana.after(1000, actualizar_semaforo)

# Ejecutar aplicación
ventana.mainloop()
