#include <stdio.h>
#include <locale.h> 
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"  //вспомогательные функции (readInt, setColor, clearScreen, loadItemsFromFile)
#include "player.h" //структура Player и функции инвентаря/дневника/доски
#include "map.h"    //структура Map, приключение

// объявляем глобальные пременные (время пока живёт здесь)
int current_day = 1;
int current_hour = 8;

//========================================================
//печать меню (вынесено из main)
void printMenu()
{
	printf("\n=== МЕНЮ ФЕРМЕРА ===\n");
	printf("[1] Создать/Пересоздать персонажа\n");
	printf("[2] Карточка персонажа (время, инвентарь)\n");
	printf("[3] Время (Посмотреть, Ждать, Спать)\n");
	printf("[4] Инвентарь (Посмотреть)\n");
	printf("[5] Положить предмет в слот\n");
	printf("[6] Выбросить предмет\n");
	printf("[7] Инверсия карманов\n");
	printf("[8] Поиск предмета по названию\n");
	printf("[9] Записать состояние в дневник\n");
	printf("[10] Доска объявлений (квесты)\n");
	printf("[11] Отправиться в приключение\n");
	printf("[0] Выход\n");
	printf("Ваш выбор: ");
}

int main()
{
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	srand((unsigned)time(NULL));

	char item_names[MAX_ITEMS][MAX_NAME_LEN];
	loadItemsFromFile(item_names, "items.txt");   // грузим каталог предметов

	Player* player = NULL;   // персонаж создаётся через меню, изначально пусто
	int running = 1;

	while (running)
	{
		printMenu();
		int choice = readInt();

		switch (choice)
		{
		case 0:
			running = 0;
			break;

		case 1:
			//пересоздание с подтверждением
			if (player != NULL) {
				printf("Персонаж уже существует. Удалить старого? (1 - Да, 0 - Нет): ");
				if (readInt() == 1) freePlayer(player);
				else continue;
			}
			player = createPlayer();
			break;

		case 2:
			if (!player) { printf("Сначала создайте персонажа!\n"); break; }
			printf("\n--- КАРТОЧКА ---\nИмя: %s\nHP: %d/%d\nВыносливость: %d/%d\nЗолото: %d\n",
				player->name, player->hp, player->max_hp, player->stamina, player->max_stamina, player->gold);
			showTime(current_day, current_hour);
			checkInventory(player, item_names);
			break;

		case 3:
			//защита нельзя управлять временем, если нет персонажа
			if (!player) { printf("Сначала создайте персонажа!\n"); break; }

			printf("[1] Посмотреть часы\n[2] Подождать\n[3] Лечь спать\n");
			int t = readInt();

			if (t == 1) {
				showTime(current_day, current_hour);
			}
			else if (t == 2) {
				printf("Сколько часов ждать? ");
				int h = readInt();
				if (h < 1) { printf("Нельзя ждать меньше часа!\n"); break; }

				//вычисляем сколько время после ожидания
				int new_hour = current_hour + h;
				int new_day = current_day + (new_hour / 24);
				new_hour = new_hour % 24;

				//проверка на 2 часа ночи
				int crossed_2am = 0;
				for (int i = 1; i <= h; i++) {
					int check_hour = (current_hour + i) % 24;
					if (check_hour == 2) crossed_2am = 1;
				}

				if (crossed_2am) {
					printf("\n[!] Фермер начинает засыпать на ходу. Нельзя ждать дольше 02:00 ночи!\n");
					printf("    Идите спать (пункт 3).\n\n");
				}
				else {
					current_day = new_day;
					current_hour = new_hour;
					printf("Вы подождали %d ч. Текущее время: день %d, %02d:00\n", h, current_day, current_hour);
				}
			}
			else if (t == 3) {
				current_day++; current_hour = 8;
				player->hp = player->max_hp;
				player->stamina = player->max_stamina;
				printf("Вы поспали. Утро! HP и стамина восстановлены.\n");
			}
			break;

		case 4:
			if (!player) { printf("Сначала создайте персонажа!\n"); break; }
			checkInventory(player, item_names);
			break;

		case 5:
			if (!player) { printf("Сначала создайте персонажа!\n"); break; }
			swapItem(player, item_names);
			break;

		case 6:
			if (!player) { printf("Сначала создайте персонажа!\n"); break; }
			dropItem(player, item_names);
			break;

		case 7:
			if (!player) { printf("Сначала создайте персонажа!\n"); break; }
			reverseInventory(player, item_names);
			break;

		case 8:
			if (!player) { printf("Сначала создайте персонажа!\n"); break; }
			searchItemByName(player, item_names);
			break;

		case 9:
			if (!player) { printf("Сначала создайте персонажа!\n"); break; }
			saveDiary(player, item_names, current_day, current_hour);
			break;

		case 10:
			if (!player) { printf("Сначала создайте персонажа!\n"); break; }
			boardOfQuests(player, item_names, current_day);
			break;

		case 11:
			//защита: без персонажа и без стамины в поход нельзя
			if (!player) { printf("Сначала создайте персонажа!\n"); break; }
			if (player->stamina <= 0) { printf("У вас нет сил идти!\n"); break; }

			printf("Генерация леса...\n");
			Map* map = createMap(15, 15);
			adventureRun(player, &map);
			break;

		default:
			printf("Неверный пункт!\n");
			break;
		}
	}

	if (player) freePlayer(player);   // очистка при выходе
	return 0;
}