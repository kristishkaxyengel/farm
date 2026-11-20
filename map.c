//реализаци приключения
#include "map.h"
#include <time.h>

//создание карты
Map* createMap(int height, int width) {
    Map* m = (Map*)malloc(sizeof(Map)); //выделение памяти под структуру
    if (!m) return NULL;
    m->height = height; m->width = width; m->floors_passed = 0; //задаем размеры

    //выделяем память под массив указателей
    m->grid = (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        m->grid[i] = (char*)malloc(width * sizeof(char)); //память под одну строку
        for (int x = 0; x < width; x++) {
            int roll = rand() % 100;
            m->grid[i][x] = (roll < 20) ? ((rand() % 2) ? MAP_TREE : MAP_ROCK) : MAP_FLOOR;
        }
    }
    return m;
}

void freeMap(Map* map) {
    if (map) {
        for (int i = 0; i < map->height; i++) free(map->grid[i]); //удаляем строки
        free(map->grid); //удаляем массив
        printf("Память структуры по адресу %p освобождена\n", (void*)map);
        free(map); //удаляем структуру
    }
}

//чтение команды игрока
char readDirection(void) {
    char line[16]; char cmd;
    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL) continue;
        cmd = (char)toupper((unsigned char)line[0]); //первый символ делаем заглавным
        if (cmd == 'W' || cmd == 'A' || cmd == 'S' || cmd == 'D' || cmd == 'Q') return cmd;
        printf("Неверная команда. Введите W/A/S/D или Q: ");
    }
}

//проверка шага
int heroMove(Map* map, Player* player, char cmd) {
    int dx = 0, dy = 0;
    switch (cmd) {
    case 'W': dy = -1; break;
    case 'S': dy = 1; break;
    case 'A': dx = -1; break;
    case 'D': dx = 1; break;
    default: return 0;
    }
    int nx = player->x + dx, ny = player->y + dy; //новые координаты
    //сигнал для портала если вышли за границы
    if (nx < 0 || nx >= map->width || ny < 0 || ny >= map->height) return -1;

    char target = map->grid[ny][nx];
    if (target != MAP_FLOOR && target != MAP_PORTAL) return 0;

    map->grid[player->y][player->x] = MAP_FLOOR;
    player->x = nx; player->y = ny;
    map->grid[player->y][player->x] = MAP_HERO;

    if (target == MAP_PORTAL) return 2;
    return 1;
}

void adventureRun(Player* player, Map** map_ptr) {
    Map* map = *map_ptr;
    int wanderCount = 0;
    int running = 1;

    do {
        player->x = 1 + rand() % (map->width - 2);
        player->y = 1 + rand() % (map->height - 2);
    } while (map->grid[player->y][player->x] != MAP_FLOOR);
    map->grid[player->y][player->x] = MAP_HERO;

    while (running) {
        clearScreen();
        printf("HP: %d/%d | Выносливость: %d/%d | Золото: %d | Этаж: %d\n\n",
            player->hp, player->max_hp, player->stamina, player->max_stamina, player->gold, map->floors_passed + 1);

        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                char c = map->grid[y][x];
                if (c == MAP_HERO) setColor(14);
                else if (c == MAP_TREE) setColor(2);
                else if (c == MAP_ROCK) setColor(7);
                else if (c == MAP_PORTAL) setColor(13);
                else setColor(15);
                putchar(c);
            }
            putchar('\n');
        }

        printf("\nКуда идём (W/A/S/D, Q - выйти)? > ");
        char cmd = readDirection();

        if (cmd == 'Q') { running = 0; continue; }

        int res = heroMove(map, player, cmd);

        if (res == 1 || res == -1 || res == 2) {
            int stamina_cost = (player->backpack.state == LOAD_STATE_OVERWEIGHT) ? 3 : 1;
            player->stamina -= stamina_cost;

            if (player->stamina <= 0) {
                printf("\n[!] Вы потеряли сознание! Эвакуация... Половина золота потеряна.\n");
                player->gold /= 2;
                player->stamina = 10;
                running = 0;
                Sleep(2000);
                continue;
            }
        }

        if (res == -1) {
            map->grid[player->y][player->x] = MAP_FLOOR;
            if (cmd == 'W') player->y = map->height - 1;
            if (cmd == 'S') player->y = 0;
            if (cmd == 'A') player->x = map->width - 1;
            if (cmd == 'D') player->x = 0;
            map->grid[player->y][player->x] = MAP_HERO;

            wanderCount++;
            if (wanderCount >= WANDER_TO_PORTAL) {
                int px = rand() % map->width, py = rand() % map->height;
                if (map->grid[py][px] == MAP_FLOOR) map->grid[py][px] = MAP_PORTAL;
            }
            printf("[~] Вас занесло на другой край!\n");
        }
        else if (res == 2) {
            printf("\n[*] Вы вошли в портал! Генерируем новый лес...\n");
            Sleep(1000);

            int reward = 10 * (map->floors_passed + 1);
            player->gold += reward;
            player->stamina -= 5;

            freeMap(map);
            map = createMap(MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1),
                MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1));
            map->floors_passed = (*map_ptr)->floors_passed + 1;

            do {
                player->x = 1 + rand() % (map->width - 2);
                player->y = 1 + rand() % (map->height - 2);
            } while (map->grid[player->y][player->x] != MAP_FLOOR);
            map->grid[player->y][player->x] = MAP_HERO;
            wanderCount = 0;

            printf("[+] Этаж %d! Награда: %d золота.\n", map->floors_passed + 1, reward);
        }
        Sleep(SLEEP_BAD_MOVE);
    }

    freeMap(map);
    *map_ptr = NULL;
}