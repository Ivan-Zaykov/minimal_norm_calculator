#!/usr/bin/env python3
import tkinter as tk
from tkinter import ttk, messagebox
import subprocess
import threading
import os

class LebesgueGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Lebesgue Constant Calculator")
        self.root.geometry("800x600")

        # Путь к исполняемому файлу
        self.executable = "./build/LebesgueOptimizer"  # Linux
        if os.name == 'nt':  # Windows
            self.executable = ".\\build\\LebesgueOptimizer.exe"

        # Переменные для ввода
        self.degree = tk.IntVar(value=5)
        self.samples = tk.IntVar(value=1000)
        self.domain_type = tk.StringVar(value="interval")
        self.dimension = tk.IntVar(value=2)
        self.node_type = tk.StringVar(value="uniform")

        # Результаты
        self.result = tk.StringVar(value="")

        self.create_widgets()

    def create_widgets(self):
        # Основная рамка
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))

        # Заголовок
        ttk.Label(main_frame, text="Lebesgue Constant Calculator",
                  font=('Arial', 16, 'bold')).grid(row=0, column=0, columnspan=2, pady=10)

        # Параметры
        row = 1
        ttk.Label(main_frame, text="Degree:").grid(row=row, column=0, sticky=tk.W, pady=5)
        ttk.Entry(main_frame, textvariable=self.degree, width=10).grid(row=row, column=1, sticky=tk.W)

        row += 1
        ttk.Label(main_frame, text="Sample points:").grid(row=row, column=0, sticky=tk.W, pady=5)
        ttk.Entry(main_frame, textvariable=self.samples, width=10).grid(row=row, column=1, sticky=tk.W)

        row += 1
        ttk.Label(main_frame, text="Domain type:").grid(row=row, column=0, sticky=tk.W, pady=5)
        domain_combo = ttk.Combobox(main_frame, textvariable=self.domain_type,
                                     values=["interval", "cube", "simplex"], state="readonly")
        domain_combo.grid(row=row, column=1, sticky=tk.W)
        domain_combo.bind("<<ComboboxSelected>>", self.on_domain_change)

        row += 1
        ttk.Label(main_frame, text="Dimension:").grid(row=row, column=0, sticky=tk.W, pady=5)
        ttk.Entry(main_frame, textvariable=self.dimension, width=10).grid(row=row, column=1, sticky=tk.W)

        row += 1
        ttk.Label(main_frame, text="Node type (compute):").grid(row=row, column=0, sticky=tk.W, pady=5)
        node_combo = ttk.Combobox(main_frame, textvariable=self.node_type,
                                   values=["uniform", "chebyshev"], state="readonly")
        node_combo.grid(row=row, column=1, sticky=tk.W)

        # Кнопки
        row += 1
        button_frame = ttk.Frame(main_frame)
        button_frame.grid(row=row, column=0, columnspan=2, pady=20)

        ttk.Button(button_frame, text="Compute", command=self.run_compute).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="Optimize", command=self.run_optimize).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="Clear", command=self.clear_output).pack(side=tk.LEFT, padx=5)

        # Область вывода
        row += 1
        ttk.Label(main_frame, text="Output:").grid(row=row, column=0, columnspan=2, sticky=tk.W)

        row += 1
        self.output_text = tk.Text(main_frame, wrap=tk.WORD, height=25, width=80)
        self.output_text.grid(row=row, column=0, columnspan=2, pady=5)

        scrollbar = ttk.Scrollbar(main_frame, orient=tk.VERTICAL, command=self.output_text.yview)
        scrollbar.grid(row=row, column=2, sticky=(tk.N, tk.S))
        self.output_text['yscrollcommand'] = scrollbar.set

        # Статус
        row += 1
        self.status_label = ttk.Label(main_frame, text="Ready", relief=tk.SUNKEN, anchor=tk.W)
        self.status_label.grid(row=row, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=5)

    def on_domain_change(self, event=None):
        """При изменении типа области обновляем размерность по умолчанию"""
        if self.domain_type.get() == "interval":
            self.dimension.set(1)
            self.dimension_entry.config(state='disabled')
        else:
            self.dimension_entry.config(state='normal')

    def run_compute(self):
        """Запуск вычисления константы Лебега"""
        args = [
            self.executable,
            "compute",
            "-d", str(self.degree.get()),
            "-s", str(self.samples.get()),
            "-t", self.domain_type.get(),
            "-dim", str(self.dimension.get()),
            "-n", self.node_type.get()
        ]
        self.run_command(args, "Computing...")

    def run_optimize(self):
        """Запуск оптимизации узлов"""
        args = [
            self.executable,
            "optimize",
            "-d", str(self.degree.get()),
            "-s", str(self.samples.get()),
            "-t", self.domain_type.get(),
            "-dim", str(self.dimension.get()),
            "-m", "nelder-mead"
        ]
        self.run_command(args, "Optimizing...")

    def run_command(self, args, status_msg):
        """Запуск команды в отдельном потоке"""
        self.status_label.config(text=status_msg)
        self.output_text.delete(1.0, tk.END)

        def run():
            try:
                result = subprocess.run(
                    args,
                    capture_output=True,
                    text=True,
                    timeout=300  # 5 минут таймаут
                )

                # Показать вывод
                self.root.after(0, self.display_output, result.stdout, result.stderr)

                if result.returncode == 0:
                    self.root.after(0, lambda: self.status_label.config(text="Done"))
                else:
                    self.root.after(0, lambda: self.status_label.config(text=f"Error (code {result.returncode})"))

            except subprocess.TimeoutExpired:
                self.root.after(0, lambda: self.status_label.config(text="Timeout (5 min)"))
                self.root.after(0, lambda: self.display_output("", "Process timed out"))
            except FileNotFoundError:
                self.root.after(0, lambda: self.status_label.config(text="Executable not found"))
                self.root.after(0, lambda: self.display_output("", f"File not found: {self.executable}"))
            except Exception as e:
                self.root.after(0, lambda: self.status_label.config(text="Error"))
                self.root.after(0, lambda: self.display_output("", str(e)))

        thread = threading.Thread(target=run, daemon=True)
        thread.start()

    def display_output(self, stdout, stderr):
        """Отображение вывода в текстовое поле"""
        self.output_text.insert(tk.END, stdout)
        if stderr:
            self.output_text.insert(tk.END, "\n--- STDERR ---\n")
            self.output_text.insert(tk.END, stderr)
        self.output_text.see(tk.END)

    def clear_output(self):
        """Очистка вывода"""
        self.output_text.delete(1.0, tk.END)
        self.status_label.config(text="Ready")

def main():
    root = tk.Tk()
    app = LebesgueGUI(root)
    root.mainloop()

if __name__ == "__main__":
    main()
