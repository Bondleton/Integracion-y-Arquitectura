import tkinter as tk
import time
import threading

class LedSimulator:
    def __init__(self, root):
        self.root = root
        self.root.title("Simulación de LEDs con Imágenes Escaladas")

        # Canvas
        self.canvas = tk.Canvas(root, width=600, height=200, bg="black")
        self.canvas.pack(pady=20)

        # Cargar imágenes y reducir tamaño con subsample
        self.images_on = {
            "red": tk.PhotoImage(file="led_red_on.png").subsample(2, 2),
            "green": tk.PhotoImage(file="led_green_on.png").subsample(2, 2),
            "blue": tk.PhotoImage(file="led_blue_on.png").subsample(2, 2),
        }
        self.images_off = {
            "red": tk.PhotoImage(file="led_red_off.png").subsample(2, 2),
            "green": tk.PhotoImage(file="led_green_off.png").subsample(2, 2),
            "blue": tk.PhotoImage(file="led_blue_off.png").subsample(2, 2),
        }

        # Secuencia de LEDs (5 en total, repitiendo verde y azul)
        self.sequence = ["red", "green", "blue", "green", "blue"]

        # Crear LEDs como imágenes
        self.leds = []
        for i, color in enumerate(self.sequence):
            x = 70 + i * 100
            led = self.canvas.create_image(x, 80, image=self.images_off[color])
            self.leds.append((led, color))

        # Botones
        self.start_button = tk.Button(root, text="Iniciar Secuencia", command=self.start_sequence)
        self.start_button.pack(side="left", padx=20, pady=10)

        self.stop_button = tk.Button(root, text="Detener Secuencia", command=self.stop_sequence)
        self.stop_button.pack(side="right", padx=20, pady=10)

        self.running = False

    def start_sequence(self):
        if not self.running:
            self.running = True
            threading.Thread(target=self.run_sequence, daemon=True).start()

    def stop_sequence(self):
        self.running = False
        # Apagar todos
        for led, color in self.leds:
            self.canvas.itemconfig(led, image=self.images_off[color])

    def run_sequence(self):
        while self.running:
            for led, color in self.leds:
                if not self.running:
                    break
                # Apagar todos primero
                for led_off, color_off in self.leds:
                    self.canvas.itemconfig(led_off, image=self.images_off[color_off])

                # Encender el actual
                self.canvas.itemconfig(led, image=self.images_on[color])
                time.sleep(0.5)


if __name__ == "__main__":
    root = tk.Tk()
    app = LedSimulator(root)
    root.mainloop()
