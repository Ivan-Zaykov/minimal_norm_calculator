#!/bin/bash

# Базовый URL
BASE_URL="https://neilsloane.com/hadamard/had.28."

# Выходной файл
OUTPUT_FILE="hadamard_28_all.txt"

# Очищаем выходной файл перед записью (если существует)
> "$OUTPUT_FILE"

# Проходим по номерам от 1 до 60
for i in $(seq 1 500); do
    URL="${BASE_URL}${i}.txt"
    echo "Скачивание: $URL"

    # Скачиваем файл
    wget -q "$URL" -O "temp_hadamard_${i}.txt"

    if [ $? -eq 0 ] && [ -s "temp_hadamard_${i}.txt" ]; then
        # Добавляем содержимое файла в выходной
        cat "temp_hadamard_${i}.txt" >> "$OUTPUT_FILE"
        # Добавляем пустую строку
        echo "" >> "$OUTPUT_FILE"
        echo "  -> Добавлена матрица #${i}"
    else
        echo "  -> ОШИБКА: не удалось скачать $URL"
    fi

    # Удаляем временный файл
    rm -f "temp_hadamard_${i}.txt"
done

echo "Готово! Все матрицы сохранены в: $OUTPUT_FILE"