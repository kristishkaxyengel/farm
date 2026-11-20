//файл с реализацией функций
#include "utils.h" //подключаем наш файл

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
} //меняет цвет текста

void clearScreen(void) { //очищает экран терминала
    system("cls");
}

int readInt(void) { //безопасный ввод целого числа
    int value; //если получается прочитать то круто, если нет то в цикл
    while (scanf_s("%d", &value) != 1) {
        while (getchar() != '\n'); //вычитываем мусор
        printf("Неверный ввод, попробуйте снова: ");
    }
    while (getchar() != '\n'); //убираем enter чтоб он не сломал следующий ввод
    return value;
}

//загружает названия предметов из файла
void loadItemsFromFile(char item_names[MAX_ITEMS][MAX_NAME_LEN], const char* filename) {
    const char* defaults[MAX_ITEMS] = {
        "Пусто", "Дерево", "Камень", "Семена пшеницы", "Конфетка",
        "Клубника", "Сено", "Кукуруза", "Мясо", "Вода"
    };
    for (int i = 0; i < MAX_ITEMS; i++) //копирует дефолтные значения в наш массив
        strcpy_s(item_names[i], MAX_NAME_LEN, defaults[i]); //безопасная копия строки

    FILE* f = NULL; //указатель на файл
    if (fopen_s(&f, filename, "r") != 0 || f == NULL) {
        printf("[!] Файл %s не найден. Используются названия по умолчанию.\n", filename);
        return;
    }

    char line[MAX_NAME_LEN]; //буфер для чтения одной строки
    int i = 0; //счетчик предметов
    while (i < MAX_ITEMS && fgets(line, sizeof(line), f) != NULL) { //читаем пока не закончится или 10 предметов
        line[strcspn(line, "\r\n")] = '\0'; //убираем символ переноса
        if (line[0] != '\0') { //если не пустая
            strcpy_s(item_names[i], MAX_NAME_LEN, line); //копируем в массив
            i++;
        }
    }
    fclose(f);
}

void showTime(int day, int hour) { //время
    setColor(10);
    printf("Текущее время: день %d, время %02d:00\n", day, hour);
    setColor(7);
}