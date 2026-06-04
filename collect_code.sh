#!/bin/bash

# --- Настройки ---
# Имя выходного файла
OUTPUT_FILE="full_code.txt"
# Разделитель между файлами (строка из 80 символов '=')
SEPARATOR="================================================================================"
# Временный файл для сортировки списка исходников (необязательно, но для порядка)
TEMP_FILE_LIST="temp_files_list.txt"

# Очищаем выходной файл перед записью (если он существует)
> "$OUTPUT_FILE"

# --- Функция для добавления содержимого файла в выходной ---
add_file() {
    local filepath="$1"
    local filename=$(basename "$filepath")

    # Добавляем разделитель и заголовок
    echo "$SEPARATOR" >> "$OUTPUT_FILE"
    echo "// Файл: $filepath" >> "$OUTPUT_FILE"
    echo "$SEPARATOR" >> "$OUTPUT_FILE"
    echo "" >> "$OUTPUT_FILE"

    # Добавляем содержимое файла
    cat "$filepath" >> "$OUTPUT_FILE"

    # Добавляем пустую строку для разделения между файлами
    echo "" >> "$OUTPUT_FILE"
    echo "" >> "$OUTPUT_FILE"
}

# Добавляем главный файл main.cpp, если он лежит в корне
if [ -f "./main.cpp" ]; then
    echo "./main.cpp" >> "$TEMP_FILE_LIST"
fi
# --- Сбор списка всех исходников и заголовков ---
# Находим все .cpp файлы в папке src
find ./src -type f -name "*.cpp" > "$TEMP_FILE_LIST"
# Находим все .h файлы в папке include
find ./include -type f -name "*.h" >> "$TEMP_FILE_LIST"

# Сортируем список для наглядности
sort -o "$TEMP_FILE_LIST" "$TEMP_FILE_LIST"

# --- Основной цикл по файлам ---
echo "Начинаю сборку файлов в $OUTPUT_FILE..."
while IFS= read -r filepath; do
    if [ -f "$filepath" ]; then
        echo "Добавляю: $filepath"
        add_file "$filepath"
    else
        echo "Ошибка: Файл не найден - $filepath"
    fi
done < "$TEMP_FILE_LIST"

# --- Убираем временный файл ---
rm -f "$TEMP_FILE_LIST"

echo "Готово! Результат сохранён в $OUTPUT_FILE"