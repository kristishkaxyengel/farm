//реализация функций игрока
#include "player.h"

//веса предметов по их ID (для Варианта 9)
int item_weights[MAX_ITEMS] = { 0, 5, 10, 1, 1, 2, 3, 2, 4, 2 };

void updateWeight(Player* player) { //пересчет веса
    player->backpack.current_weight = 0; //обнуление
    for (int i = 0; i < MAX_ITEMS; i++) {
        //берем, смотрим вес в массиве и прибавляем к общему
        player->backpack.current_weight += item_weights[player->backpack.slots[i]];
    }

    if (player->backpack.current_weight > player->backpack.max_weight) { //а если больше?
        player->backpack.state = LOAD_STATE_OVERWEIGHT; //перегруз!
    }
    else if (player->backpack.current_weight > player->backpack.max_weight * 0.7) { // больше 70%?
        player->backpack.state = LOAD_STATE_NORMAL; //нормис
    }
    else {
        player->backpack.state = LOAD_STATE_LIGHT; //или налегке
    }
}

//создание игрока (динамическая память)
Player* createPlayer(void) {
    Player* p = (Player*)malloc(sizeof(Player)); //malloc выделяет память
    if (!p) return NULL; //если нет-выход

    printf("Как тебя зовут, фермер?: ");
    fgets(p->name, MAX_NAME_LEN, stdin); //читаем
    p->name[strcspn(p->name, "\r\n")] = '\0'; //убиарем энтер
    if (p->name[0] == '\0') strcpy_s(p->name, MAX_NAME_LEN, "Фермер");

    //стартовые характеристики
    p->hp = 100; p->max_hp = 100;
    p->stamina = 50; p->max_stamina = 50;
    p->gold = 0;
    p->x = 0; p->y = 0;

    //стартовый набор инвентаря
    int start_inv[MAX_ITEMS] = { 0, 2, 8, 5, 1, 8, 3, 5, 4, 2 };
    for (int i = 0; i < MAX_ITEMS; i++) p->backpack.slots[i] = start_inv[i];

    p->backpack.max_weight = 30; //стартовый лимит веса
    updateWeight(p); //считаем вес старта

    printf("[+] Персонаж %s создан! (Адрес в памяти: %p)\n", p->name, (void*)p);
    return p;
}

//удаление игрока
void freePlayer(Player* player) {
    if (player) { //если указатель не пустой
        printf("Память структуры по адресу %p освобождена\n", (void*)player);
        free(player); //освободили память
    }
}

//показать инвентарь
void checkInventory(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    const char* loadStr[] = { "Налегке", "Норма", "ПЕРЕГРУЗ!" };
    printf("Вес: %d/%d [%s]\n", player->backpack.current_weight, player->backpack.max_weight, loadStr[player->backpack.state]);

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (player->backpack.slots[i] == 0) printf("Слот %d: пусто\n", i);
        else printf("Слот %d: [%d] (%s)\n", i, player->backpack.slots[i], item_names[player->backpack.slots[i]]);
    }
}

//замена предметов
void swapItem(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    checkInventory(player, item_names); //показываем что имеем
    int slot, id;
    printf("Введите номер слота (0-9): "); slot = readInt();
    if (slot < 0 || slot >= MAX_ITEMS) { printf("Ошибка!\n"); return; } //проверка границ

    for (int i = 0; i < MAX_ITEMS; i++) printf("ID %d: %s\n", i, item_names[i]); //показываем id
    printf("Введите ID предмета (0-9): "); id = readInt();
    if (id < 0 || id >= MAX_ITEMS) { printf("Ошибка!\n"); return; }

    player->backpack.slots[slot] = id; //кладем предмет в слот
    updateWeight(player); //пересчитываем вес
}

//выброс предмета
void dropItem(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    checkInventory(player, item_names);
    int slot;
    printf("Введите номер слота для удаления (0-9): "); slot = readInt();
    if (slot < 0 || slot >= MAX_ITEMS) return;
    player->backpack.slots[slot] = 0;
    updateWeight(player);
}

//поиск по имени
void searchItemByName(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    char query[MAX_NAME_LEN];
    printf("Введите название: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\r\n")] = '\0'; //убрали энтер

    int count = 0;
    for (int i = 0; i < MAX_ITEMS; i++) {
        //если слот не пустой и имя совпадает
        if (player->backpack.slots[i] != 0 && strcmp(item_names[player->backpack.slots[i]], query) == 0) {
            printf("Найдено в слоте: %d\n", i); count++;
        }
    }
    if (count == 0) printf("В рюкзаке этого предмета нет.\n");
}

//реверс карманов
void reverseInventory(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    printf("Инвентарь до:\n");
    checkInventory(player, item_names);
    //меняем местами
    for (int i = 0; i < MAX_ITEMS / 2; i++) {
        int temp = player->backpack.slots[i];
        player->backpack.slots[i] = player->backpack.slots[MAX_ITEMS - 1 - i];
        player->backpack.slots[MAX_ITEMS - 1 - i] = temp;
    }
    printf("\nИнвентарь после:\n");
    checkInventory(player, item_names);
}

//дневник
void saveDiary(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN], int day, int hour) {
    FILE* file = NULL;
    if (fopen_s(&file, "diary.txt", "w") != 0 || file == NULL) { //открыли файл для записи
        printf("[!] Не удалось открыть diary.txt\n");
        return;
    }
    printf("\n========== ДНЕВНИК ==========\n");
    fprintf(file, "========== ДНЕВНИК ==========\n");
    fprintf(file, "Фермер: %s | День: %d | Время: %02d:00\n", player->name, day, hour);
    fprintf(file, "Золото: %d | HP: %d/%d\n", player->gold, player->hp, player->max_hp);
    for (int i = 0; i < MAX_ITEMS; i++) {
        fprintf(file, "Слот %d: %s\n", i, item_names[player->backpack.slots[i]]);
    }
    fclose(file);
    printf("[+] Состояние сохранено в diary.txt\n");
}

//доска объявлений
void boardOfQuests(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN], int current_day) {
    FILE* fin = NULL, * fout = NULL;
    if (fopen_s(&fin, "input.txt", "r") != 0 || !fin) {
        printf("[!] input.txt не найден\n");
        return;
    }
    if (fopen_s(&fout, "output.txt", "w") != 0 || !fout) {
        fclose(fin); return;
    }
    printf("=== Доска (день %d) ===\n\n", current_day);
    char line[256];
    while (fgets(line, sizeof(line), fin)) { //читаем построчно
        int q_day, reward;
        char customer[64], item[32];
        //разбираем строку по шаблону число/строка/строка/числл
        if (sscanf_s(line, "%d|%63[^|]|%31[^|]|%d", &q_day, customer, (unsigned)sizeof(customer), item, (unsigned)sizeof(item), &reward) != 4)
            continue;
        if (q_day > current_day) continue; //если день квеста не наступил-пропускаем

        int ready = 0;
        for (int i = 0; i < MAX_ITEMS; i++) {
            if (player->backpack.slots[i] != 0 && strcmp(item_names[player->backpack.slots[i]], item) == 0) {
                ready = 1; break; //нашли нужный
            }
        }
        const char* status = ready ? "[ГОТОВО]" : "[В ПРОЦЕССЕ]"; //тернарный оператор
        printf("День %d | %s | %s | %d | %s\n", q_day, customer, item, reward, status);
        fprintf(fout, "День %d | %s | %s | %d | %s\n", q_day, customer, item, reward, status);
    }
    fclose(fin); fclose(fout);
    printf("\n[+] Результат сохранён в output.txt\n");
}