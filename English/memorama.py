import tkinter as tk
from tkinter import messagebox, ttk
import math
import random
import time
import re

# ----------------------------
# Utilidades
# ----------------------------
SPLIT_PATTERNS = [r"\s*-\s*", r"\s*=\s*", r"\s*:\s*"]

def parse_pairs(raw_text):
    """
    Recibe texto con líneas tipo:
      hello - hola
      house = casa
      computer: computadora
    Devuelve:
      pair_map (bidireccional), cards (lista con el texto de cada carta)
    """
    lines = [ln.strip() for ln in raw_text.splitlines() if ln.strip()]
    pairs = []
    for ln in lines:
        left, right = None, None
        for pat in SPLIT_PATTERNS:
            parts = re.split(pat, ln, maxsplit=1)
            if len(parts) == 2:
                left, right = parts[0].strip(), parts[1].strip()
                break
        if not left or not right:
            raise ValueError(f"Línea no válida: «{ln}». Use '-', '=' o ':' para separar.")
        pairs.append((left, right))

    # Validaciones
    if len(pairs) < 2:
        raise ValueError("Ingrese al menos 2 pares para crear el memorama.")
    flat = [x for pr in pairs for x in pr]
    if len(set(flat)) != len(flat):
        raise ValueError("Hay palabras repetidas. Verifique que cada término aparezca una sola vez.")

    # Mapa bidireccional
    pair_map = {}
    for a, b in pairs:
        pair_map[a] = b
        pair_map[b] = a

    cards = flat[:]  # cada término es una carta
    random.shuffle(cards)
    return pair_map, cards

def best_grid(n):
    """
    Devuelve (rows, cols) para n cartas, aproximando un rectángulo equilibrado.
    """
    root = int(math.sqrt(n))
    rows = root
    cols = math.ceil(n / rows)
    while rows * cols < n:
        rows += 1
        cols = math.ceil(n / rows)
    if cols < rows:
        rows, cols = cols, rows
    return rows, cols

# ----------------------------
# Clase del Juego
# ----------------------------
class MemoramaApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Memorama Inglés–Español")
        self.geometry("900x650")
        self.minsize(820, 580)

        # Estilo ttk
        self.style = ttk.Style(self)
        try:
            if "clam" in self.style.theme_names():
                self.style.theme_use("clam")
        except Exception:
            pass

        # Estado del juego
        self.pair_map = {}
        self.cards = []
        self.buttons = []
        self.revealed = set()
        self.first_pick = None
        self.second_pick = None
        self.moves = 0
        self.start_time = None
        self.timer_id = None

        # Frames
        self.setup_frame = ttk.Frame(self, padding=16)
        self.game_frame = ttk.Frame(self, padding=10)

        self._build_setup_ui()
        self.setup_frame.pack(fill="both", expand=True)

    # ------------------------
    # UI de configuración
    # ------------------------
    def _build_setup_ui(self):
        title = ttk.Label(
            self.setup_frame,
            text="Memorama de Vocabulario (Inglés–Español)",
            font=("Segoe UI", 18, "bold")
        )
        title.pack(pady=(0, 10))

        instructions = (
            "Pegue sus pares (una línea por par):\n"
            "  palabra_en_inglés - significado_en_español\n"
            "También se aceptan '=' o ':'. Ejemplos:\n"
            "  apple - manzana\n"
            "  house = casa\n"
            "  computer: computadora"
        )
        ttk.Label(self.setup_frame, text=instructions, justify="left").pack(anchor="w", pady=(0, 8))

        self.text_input = tk.Text(self.setup_frame, height=12, wrap="word")
        self.text_input.pack(fill="both", expand=True)

        example = (
            "apple - manzana\n"
            "house - casa\n"
            "computer - computadora\n"
            "book - libro\n"
            "car - coche\n"
            "dog - perro\n"
            "window - ventana\n"
            "tree - árbol\n"
        )
        self.text_input.insert("1.0", example)

        bottom_bar = ttk.Frame(self.setup_frame)
        bottom_bar.pack(fill="x", pady=10)

        ttk.Label(bottom_bar, text="Tamaño de fuente de cartas:").pack(side="left")
        self.font_size_var = tk.IntVar(value=12)
        spin = ttk.Spinbox(bottom_bar, from_=8, to=24, textvariable=self.font_size_var, width=5)
        spin.pack(side="left", padx=(6, 16))

        ttk.Button(bottom_bar, text="Iniciar juego", command=self.start_game).pack(side="right")

    # ------------------------
    # Inicio del juego
    # ------------------------
    def start_game(self):
        raw = self.text_input.get("1.0", "end").strip()
        try:
            self.pair_map, self.cards = parse_pairs(raw)
        except Exception as e:
            messagebox.showerror("Entrada no válida", str(e), parent=self)
            return

        # Reset de estado
        self.revealed.clear()
        self.first_pick = None
        self.second_pick = None
        self.moves = 0
        self.start_time = time.time()

        if self.timer_id:
            self.after_cancel(self.timer_id)
            self.timer_id = None

        # Cambiar a frame de juego
        self.setup_frame.pack_forget()
        self._build_game_ui()
        self.game_frame.pack(fill="both", expand=True)
        self._tick_timer()

    # ------------------------
    # UI del juego
    # ------------------------
    def _build_game_ui(self):
        for w in self.game_frame.winfo_children():
            w.destroy()

        header = ttk.Frame(self.game_frame)
        header.pack(fill="x", pady=(0, 8))

        ttk.Button(header, text="⟵ Volver a configuración", command=self._back_to_setup).pack(side="left")

        self.moves_var = tk.StringVar(value="Movimientos: 0")
        self.time_var = tk.StringVar(value="Tiempo: 00:00")
        ttk.Label(header, textvariable=self.moves_var, font=("Segoe UI", 11, "bold")).pack(side="right", padx=(8, 0))
        ttk.Label(header, textvariable=self.time_var, font=("Segoe UI", 11)).pack(side="right")

        board_container = ttk.Frame(self.game_frame)
        board_container.pack(fill="both", expand=True)

        n = len(self.cards)
        rows, cols = best_grid(n)

        self.buttons = []
        font_size = self.font_size_var.get()
        btn_font = ("Segoe UI", font_size)

        # Estilo: DEBE finalizar en 'TButton' para heredar el layout del botón
        style_name = f"Card{font_size}.TButton"
        self._ensure_button_style(style_name, btn_font)

        idx = 0
        for r in range(rows):
            row_frame = ttk.Frame(board_container)
            row_frame.pack(fill="x", expand=True)
            for c in range(cols):
                if idx < n:
                    txt = self.cards[idx]
                    btn = ttk.Button(row_frame, text="?", style=style_name)
                    btn.card_text = txt
                    btn.is_revealed = False
                    btn["command"] = lambda b=btn: self.on_card_click(b)
                    btn.pack(side="left", expand=True, fill="both", padx=4, pady=4)
                    self.buttons.append(btn)
                    idx += 1
                else:
                    filler = ttk.Label(row_frame, text="")
                    filler.pack(side="left", expand=True, fill="both", padx=4, pady=4)

        info = (
            "Regla: forme pares correctos (Inglés ↔ Español). Cada palabra tiene exactamente una pareja.\n"
            "Consejo: reduzca el tamaño de fuente en la configuración si el texto no cabe."
        )
        ttk.Label(self.game_frame, text=info, justify="left").pack(fill="x", pady=(6, 0))

    def _ensure_button_style(self, style_name, font_tuple):
        # Configurar el estilo; como el nombre termina en 'TButton', hereda su layout
        try:
            self.style.configure(style_name, padding=10, font=font_tuple, anchor="center")
        except Exception:
            pass

    # ------------------------
    # Lógica de clic en carta
    # ------------------------
    def on_card_click(self, btn):
        if btn.is_revealed:
            return
        if self.second_pick is not None:
            return

        # Revelar carta
        btn.config(text=btn.card_text)
        btn.is_revealed = True

        if self.first_pick is None:
            self.first_pick = (btn, btn.card_text)
            return

        if self.first_pick and self.second_pick is None:
            self.second_pick = (btn, btn.card_text)
            self.moves += 1
            self.moves_var.set(f"Movimientos: {self.moves}")

            a_txt = self.first_pick[1]
            b_txt = self.second_pick[1]
            is_match = (self.pair_map.get(a_txt) == b_txt)

            if is_match:
                self.revealed.add(a_txt)
                self.revealed.add(b_txt)
                self.first_pick[0].state(["disabled"])
                self.second_pick[0].state(["disabled"])
                self.first_pick = None
                self.second_pick = None
                self._check_victory()
            else:
                self.after(800, self._hide_mismatch)

    def _hide_mismatch(self):
        if self.first_pick:
            self.first_pick[0].config(text="?")
            self.first_pick[0].is_revealed = False
        if self.second_pick:
            self.second_pick[0].config(text="?")
            self.second_pick[0].is_revealed = False
        self.first_pick = None
        self.second_pick = None

    def _check_victory(self):
        if len(self.revealed) == len(self.cards):
            elapsed = int(time.time() - self.start_time)
            mm = elapsed // 60
            ss = elapsed % 60
            if self.timer_id:
                self.after_cancel(self.timer_id)
                self.timer_id = None
            messagebox.showinfo(
                "¡Completado!",
                f"Ha resuelto el memorama.\nTiempo: {mm:02d}:{ss:02d}\nMovimientos: {self.moves}",
                parent=self
            )

    # ------------------------
    # Timer
    # ------------------------
    def _tick_timer(self):
        elapsed = int(time.time() - self.start_time) if self.start_time else 0
        mm = elapsed // 60
        ss = elapsed % 60
        self.time_var.set(f"Tiempo: {mm:02d}:{ss:02d}")
        self.timer_id = self.after(500, self._tick_timer)

    # ------------------------
    # Volver a configuración
    # ------------------------
    def _back_to_setup(self):
        if self.timer_id:
            self.after_cancel(self.timer_id)
            self.timer_id = None
        self.game_frame.pack_forget()
        self.setup_frame.pack(fill="both", expand=True)

# ----------------------------
# Main
# ----------------------------
if __name__ == "__main__":
    app = MemoramaApp()
    app.mainloop()
