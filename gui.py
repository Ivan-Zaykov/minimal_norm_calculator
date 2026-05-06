#!/usr/bin/env python3
import tkinter as tk
from tkinter import ttk, messagebox, filedialog
import subprocess
import threading
import os

class LebesgueGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Lebesgue Constant Calculator")
        self.root.geometry("900x700")

        # Путь к исполняемому файлу
        self.executable = "./build/LebesgueOptimizer"  # Linux
        if os.name == 'nt':  # Windows
            self.executable = ".\\build\\LebesgueOptimizer.exe"

        # Переменные для ввода
        self.degree = tk.IntVar(value=5)
        self.samples = tk.IntVar(value=1000)
        self.domain_type = tk.StringVar(value="interval")
        self.dimension = tk.IntVar(value=1)
        self.node_type = tk.StringVar(value="uniform")
        self.node_file = tk.StringVar(value="")

        self.create_widgets()
        self.update_domain_combo()  # начальная настройка

    def update_domain_combo(self):
        """Обновляет список доступных типов областей в зависимости от размерности"""
        dim = self.dimension.get()

        if dim == 1:
            available = ["interval"]
            # Если текущий тип не подходит, меняем на interval
            if self.domain_type.get() not in available:
                self.domain_type.set("interval")
        else:
            available = ["cube", "simplex"]
            # Если текущий тип не подходит (например, interval), меняем на cube
            if self.domain_type.get() not in available:
                self.domain_type.set("cube")

        # Обновляем выпадающий список
        self.domain_combo['values'] = available

    def on_dimension_change(self, event=None):
        """Обработчик изменения размерности"""
        dim = self.dimension.get()
        if dim < 1:
            self.dimension.set(1)
            dim = 1
        if dim > 10:
            self.dimension.set(10)
            dim = 10

        # Обновляем доступные типы областей
        self.update_domain_combo()

    def on_domain_change(self, event=None):
        """Обработчик изменения типа области"""
        domain = self.domain_type.get()
        if domain == "interval":
            # Для interval размерность всегда 1
            self.dimension.set(1)
            self.update_domain_combo()

    def on_node_type_change(self):
        """Показываем/скрываем поле для файла"""
        if self.node_type.get() == "file":
            self.file_frame.grid()
        else:
            self.file_frame.grid_remove()

    def browse_file(self):
        """Диалог выбора файла с узлами"""
        filename = filedialog.askopenfilename(
            title="Select nodes file",
            filetypes=[("Text files", "*.txt"), ("All files", "*.*")]
        )
        if filename:
            self.node_file.set(filename)

    def create_widgets(self):
        # Основная рамка
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))

        # Заголовок
        ttk.Label(main_frame, text="Lebesgue Constant Calculator",
                  font=('Arial', 16, 'bold')).grid(row=0, column=0, columnspan=4, pady=10)

        # Параметры
        row = 1
        ttk.Label(main_frame, text="Degree:").grid(row=row, column=0, sticky=tk.W, pady=5)
        ttk.Entry(main_frame, textvariable=self.degree, width=10).grid(row=row, column=1, sticky=tk.W)
        ttk.Label(main_frame, text="(polynomial degree)").grid(row=row, column=2, columnspan=2, sticky=tk.W, padx=10)

        row += 1
        ttk.Label(main_frame, text="Sample points:").grid(row=row, column=0, sticky=tk.W, pady=5)
        ttk.Entry(main_frame, textvariable=self.samples, width=10).grid(row=row, column=1, sticky=tk.W)
        ttk.Label(main_frame, text="(more = more accurate)").grid(row=row, column=2, columnspan=2, sticky=tk.W, padx=10)

        row += 1
        ttk.Label(main_frame, text="Dimension:").grid(row=row, column=0, sticky=tk.W, pady=10)
        self.dim_spinbox = ttk.Spinbox(main_frame, from_=1, to=10, textvariable=self.dimension, width=8, command=self.on_dimension_change)
        self.dim_spinbox.grid(row=row, column=1, sticky=tk.W)
        self.dim_spinbox.bind('<KeyRelease>', lambda e: self.on_dimension_change())
        ttk.Label(main_frame, text="(1..10)").grid(row=row, column=2, columnspan=2, sticky=tk.W, padx=10)

        row += 1
        ttk.Label(main_frame, text="Domain type:").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.domain_combo = ttk.Combobox(main_frame, textvariable=self.domain_type, state="readonly")
        self.domain_combo.grid(row=row, column=1, sticky=tk.W)
        self.domain_combo.bind("<<ComboboxSelected>>", self.on_domain_change)

        ttk.Label(main_frame, text="(interval only for dim=1; cube/simplex for dim≥2)").grid(
            row=row, column=2, columnspan=2, sticky=tk.W, padx=10)

        row += 1
        ttk.Label(main_frame, text="Node type:").grid(row=row, column=0, sticky=tk.W, pady=5)

        node_frame = ttk.Frame(main_frame)
        node_frame.grid(row=row, column=1, columnspan=3, sticky=tk.W)

        ttk.Radiobutton(node_frame, text="Uniform", variable=self.node_type,
                        value="uniform", command=self.on_node_type_change).pack(side=tk.LEFT, padx=5)
        ttk.Radiobutton(node_frame, text="Chebyshev", variable=self.node_type,
                        value="chebyshev", command=self.on_node_type_change).pack(side=tk.LEFT, padx=5)
        ttk.Radiobutton(node_frame, text="From file", variable=self.node_type,
                        value="file", command=self.on_node_type_change).pack(side=tk.LEFT, padx=5)

        # Рамка для выбора файла
        self.file_frame = ttk.Frame(main_frame)
        self.file_frame.grid(row=row+1, column=0, columnspan=4, pady=5, sticky=tk.W)

        ttk.Label(self.file_frame, text="Nodes file:").pack(side=tk.LEFT)
        self.file_entry = ttk.Entry(self.file_frame, textvariable=self.node_file, width=40)
        self.file_entry.pack(side=tk.LEFT, padx=5)
        ttk.Button(self.file_frame, text="Browse...", command=self.browse_file).pack(side=tk.LEFT)

        self.file_frame.grid_remove()

        # Кнопки действий
        row += 2
        button_frame = ttk.Frame(main_frame)
        button_frame.grid(row=row, column=0, columnspan=4, pady=20)

        ttk.Button(button_frame, text="Compute", command=self.run_compute).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="Optimize", command=self.run_optimize).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="Clear Output", command=self.clear_output).pack(side=tk.LEFT, padx=5)

        # Область вывода
        row += 1
        ttk.Label(main_frame, text="Output:").grid(row=row, column=0, columnspan=4, sticky=tk.W)

        row += 1
        self.output_text = tk.Text(main_frame, wrap=tk.WORD, height=25, width=90)
        self.output_text.grid(row=row, column=0, columnspan=4, pady=5)

        scrollbar = ttk.Scrollbar(main_frame, orient=tk.VERTICAL, command=self.output_text.yview)
        scrollbar.grid(row=row, column=4, sticky=(tk.N, tk.S))
        self.output_text['yscrollcommand'] = scrollbar.set

        # Статус
        row += 1
        self.status_label = ttk.Label(main_frame, text="Ready", relief=tk.SUNKEN, anchor=tk.W)
        self.status_label.grid(row=row, column=0, columnspan=4, sticky=(tk.W, tk.E), pady=5)

    def run_compute(self):
        """Запуск вычисления константы Лебега"""
        dim = self.dimension.get()
        domain = self.domain_type.get()

        # Проверка совместимости
        if dim == 1 and domain != "interval":
            messagebox.showerror("Error", "Dimension 1 only supports 'interval' domain.\nPlease change domain type to 'interval'.")
            return
        if dim >= 2 and domain == "interval":
            messagebox.showerror("Error", "Domain 'interval' only works for dimension 1.\nPlease change dimension to 1 or change domain type.")
            return

        args = [
            self.executable,
            "compute",
            "-d", str(self.degree.get()),
            "-s", str(self.samples.get()),
            "-t", domain,
            "-dim", str(dim),
        ]

        # Добавляем параметры для узлов
        node_type = self.node_type.get()
        if node_type == "uniform" or node_type == "chebyshev":
            args.extend(["-n", node_type])
        elif node_type == "file":
            node_file = self.node_file.get().strip()
            if not node_file:
                messagebox.showerror("Error", "Please select a nodes file")
                return
            if not os.path.exists(node_file):
                messagebox.showerror("Error", f"File not found: {node_file}")
                return
            args.extend(["--nodes", node_file])

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
                    timeout=300
                )

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
