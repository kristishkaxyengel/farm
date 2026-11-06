#include <stdio.h>
#include <locale.h> 
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//имя пользователя
char username[32];

// объявляем глобальные пременные
int current_day = 1;
int current_hour = 8;

// размеры инвентаря и длины названий я доверюсо чату гпт и сделаю так))) это типо макрос типо круче чем просто переменной объявлять 
#define MAX_ITEMS    10
#define MAX_NAME_LEN 32

// двумерный массив под названия предметов как в задании
char item_names[MAX_ITEMS][MAX_NAME_LEN];

// сам инвентарь — в каждой ячейке храним ID предмета
int inventory[MAX_ITEMS] = { 0, 2, 8, 5, 1, 8, 3, 5, 4, 2 };

// штука с дефолтными значениями для инвентаря если все поломается
void fillDefaultItems()
{
	const char* defaults[MAX_ITEMS] = {
		"Пусто", "Дерево", "Камень", "Семена пшеницы", "Конфетка",
		"Клубника", "Сено", "Кукуруза", "Мясо", "Вода"
	};
	for (int i = 0; i < MAX_ITEMS; i++)
		strcpy_s(item_names[i], MAX_NAME_LEN, defaults[i]); // заполняет инвентарь по дефолту
}

// читаем названия предметов из файла (по строчке на ID)
void loadItemsFromFile(const char* filename)
{
	fillDefaultItems();   // сначала заменяем массив дефолтными значениями на случай если в файле чего то будет не хватать

	FILE* f = NULL;
	if (fopen_s(&f, filename, "r") != 0 || f == NULL)
	{
		printf("[!] Файл %s не найден. Используются названия по умолчанию.\n", filename);
		return;
	}

	char line[MAX_NAME_LEN];
	int i = 0;
	while (i < MAX_ITEMS && fgets(line, sizeof(line), f) != NULL)
	{
		line[strcspn(line, "\r\n")] = '\0';   // убираем \n и \r
		if (line[0] != '\0')                  // пустые строки пропускаем
		{
			strcpy_s(item_names[i], MAX_NAME_LEN, line);
			i++;
		}
	}
	fclose(f);
	printf("[+] Загружено предметов из %s: %d\n", filename, i);
}

//проверрка ввода пользователя
int readInt()
{
	int value;
	while (scanf_s("%d", &value) != 1)
	{
		while (getchar() != '\n');
		printf("Неверный ввод, попробуйте снова: ");
	}
	while (getchar() != '\n');
	return value;
}

// запрос имени фермера
void askName()
{
	printf("Как тебя зовут, фермер?: ");
	fgets(username, sizeof(username), stdin); //sizeof это типо сколько максимум можно записать а stdin это стандартный ввод (клавиатура)
	username[strcspn(username, "\r\n")] = '\0';   // убираем \n и \r в конце
	if (username[0] == '\0') //если вообще ничего не написали то присваиваем дефолтное имя
		strcpy_s(username, sizeof(username), "Фермер");
}

//красота (чисто для красивого выделения времени)
void setColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//считаем время работы
void timeToWork()
{
	int workTime = 0;
	printf("Сколько часов выхотите поработать: \n");
	workTime = readInt();

	if (workTime < 0)
	{
		printf("Нельзя работать отрицательное количество часов.\n");
		return;
	}
	current_hour += workTime;
	//проверка и добавление часов/дней
	if (current_hour >= 24)
	{
		current_day += current_hour / 24;
		current_hour = current_hour % 24;
	}
	setColor(10);
	printf("Текущее время: день %d, время %d:00\n", current_day, current_hour);
	setColor(7);
}

void checkInventory()
{
	//перебираем инвентарь и выводим

	for (int i = 0; i < MAX_ITEMS; i++)
	{
		if (inventory[i] == 0)
			printf("Слот %d: пусто\n", i);
		else
			printf("Слот %d: [%d] (%s)\n", i, inventory[i], item_names[inventory[i]]);
	}
}

//замена предметов
void svapItem()
{
	int slot, id;
	checkInventory();
	printf("Введите номер слота который хотите заменить (0-9): ");
	slot = readInt();
	if (slot < 0 || slot > 9) { printf("Неверный номер слота!\n"); return; }

	//добавляем вывод предметов для удобства
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		printf("Предмет %d: (%s)\n", i, item_names[i]);
	}

	printf("Введите ID предмета (0-9): ");
	id = readInt();
	if (id < 0 || id > 9) { printf("Неверный ID предмета!\n"); return; }

	inventory[slot] = id;
	printf("В слот %d положен предмет [%d] (%s)\n", slot, id, item_names[id]);
}

//удаление предмета
void dropItem()
{
	//выводим весь список для удобства выбора
	checkInventory();
	int slot;
	printf("Введите номер слота который хотите выбросить  (0-9): ");
	slot = readInt();
	if (slot < 0 || slot > 9) { printf("Неверный номер слота!\n"); return; }

	inventory[slot] = 0;
	printf("Слот %d очищен.\n", slot);
}

void reverseInventory()
{
	printf("Инвентарь до:\n");
	checkInventory();

	// меняем первый и последний слот типо по умному, ну хоть не в файле менять
	for (int i = 0; i < MAX_ITEMS / 2; i++)
	{
		int temp = inventory[i];
		inventory[i] = inventory[MAX_ITEMS - 1 - i];
		inventory[MAX_ITEMS - 1 - i] = temp;
	}

	printf("\nИнвентарь после:\n");
	checkInventory();
}


// Поиск предмета в рюкзаке по названию
void searchItemByName()
{
	char query[MAX_NAME_LEN];

	printf("Введите название предмета для поиска: ");
	fgets(query, sizeof(query), stdin);
	query[strcspn(query, "\r\n")] = '\0';

	for (int i = 1; i < MAX_ITEMS; i++)
	{
		if (strcmp(item_names[i], query) == 0)
		{
			int count = 0;

			printf("Предмет [%d] (%s) найден в слотах: ", i, item_names[i]);

			for (int j = 0; j < MAX_ITEMS; j++)
			{
				if (inventory[j] == i)
				{
					printf("%d ", j);
					count++;
				}
			}

			if (count == 0)
				printf("\nВ рюкзаке этого предмета нет.\n");
			else
				printf("\nВсего штук: %d\n", count);

			return;
		}
	}

	printf("Предмет \"%s\" не найден в каталоге.\n", query);
}

// Запись состояния фермы в дневник
void saveDiary()
{
	FILE* file = NULL;

	// Открываем файл для перезаписи
	if (fopen_s(&file, "diary.txt", "w") != 0 || file == NULL)
	{
		printf("[!] Не удалось открыть diary.txt\n");
		return;
	}

	printf("\n========== ДНЕВНИК ФЕРМЕРА ==========\n");
	fprintf(file, "========== ДНЕВНИК ФЕРМЕРА ==========\n");

	printf("Фермер: %s\n", username);
	fprintf(file, "Фермер: %s\n", username);

	printf("День: %d\n", current_day);
	fprintf(file, "День: %d\n", current_day);

	printf("Время: %02d:00\n", current_hour);
	fprintf(file, "Время: %02d:00\n", current_hour);

	printf("\nИнвентарь:\n");
	fprintf(file, "\nИнвентарь:\n");

	for (int i = 0; i < MAX_ITEMS; i++)
	{
		char* itemName = item_names[inventory[i]];

		printf("Слот %d: %s\n", i, itemName);
		fprintf(file, "Слот %d: %s\n", i, itemName);
	}

	printf("=====================================\n");
	fprintf(file, "=====================================\n");

	fclose(file);

	printf("\n[+] Состояние фермы сохранено в diary.txt\n");
}

//========================================================
//доска объявлений
void boardOfQuests()
{
	FILE* fin = NULL, * fout = NULL; //file это для работы с файлами fin начальный файл 
	if (fopen_s(&fin, "input.txt", "r") != 0 || !fin) // открывае файл через fopen_s и вводим 
	{
		printf("[!] input.txt не найден\n");
		return;
	}
	if (fopen_s(&fout, "output.txt", "w") != 0 || !fout)
	{
		printf("[!] output.txt не создать\n");
		fclose(fin);

		return;
	}

	// заголовок — и на экран, и в файл
	printf("=== Доска объявлений (день %d) ===\n\n", current_day); //выводит в консоль
	fprintf(fout, "=== Доска объявлений (день %d) ===\n\n", current_day); // записывает в сам в файл

	char line[256]; // символы для записи из input в код
	int total = 0, ready_count = 0;

	while (fgets(line, sizeof(line), fin))
	{
		int day, reward;
		char customer[64], item[32];

		if (sscanf_s(line, "%d|%63[^|]|%31[^|]|%d", //читаем и записываем информацию условно число/первый масив с заказчикм/второй масив с предметом/награда
			&day, customer, (unsigned)sizeof(customer),
			item, (unsigned)sizeof(item), &reward) != 4)
			continue;

		if (day > current_day) continue;   // квест ещё не доступен типо проверка на день
		total++;

		// ищем предмет в рюкзаке
		int ready = 0;
		for (int i = 0; i < MAX_ITEMS; i++)
		{
			if (inventory[i] != 0 &&
				strcmp(item_names[inventory[i]], item) == 0) // сравниваем предмет в задании с предметами в инвентаре
			{
				ready = 1;
				break;
			}
		}
		if (ready) ready_count++;

		const char* status = ready ? "[ГОТОВО К СДАЧЕ]" : "[В ПРОЦЕССЕ]"; // если статус готов рейди то к сдаче иначк в процесс

		// одна и та же строка — на экран и в файл
		printf("День %d | %s | %s | %d | %s\n",
			day, customer, item, reward, status);
		fprintf(fout, "День %d | %s | %s | %d | %s\n",
			day, customer, item, reward, status);
	}

	fclose(fin);

	// итог — тоже в оба места
	printf("\nИтого: доступно %d, готово к сдаче %d\n", total, ready_count);
	fprintf(fout, "\nИтого: доступно %d, готово к сдаче %d\n", total, ready_count);

	fclose(fout);
	printf("\n[+] Результат сохранён в output.txt\n");
}

///////////////////////////////////////////////

#define USERNAME_LEN 32
/////размер карты выберет от 5 до 30
#define MAP_MIN_SIZE 5
#define MAP_MAX_SIZE 30
//////////символы на карте
#define MAP_WALL '#'
#define MAP_FLOOR '.'
#define MAP_TREE 'T'
#define MAP_ROCK 'R'
#define MAP_HERO '@'
///////вероятность препяытствий на карте от 15 до 30%
#define OBSTACLE_PERCENT_MIN 15
#define OBSTACLE_PERCENT_MAX 30


////////очистка экрана
void clearScreen(void)
{
	system("cls");
}

//////////защита от дурака. читает одну команду движения: W, A,S,D или Q все остальное не принимает и переспрашивает 

char readDirection(void)
{
	char line[16]; ////маленький буфер
	char cmd;

	while (1)
	{
		if (fgets(line, sizeof(line), stdin) == NULL)
			continue; ////не прочиталось, новая попытка

		/////пропускаем пробелы и табы в начале строки
		int i = 0;
		while (line[i] == ' ' || line[i] == '\t') i++;

		/////toupper переводит "w" в "W", защита от отрицательных значений
		cmd = (char)toupper((unsigned char)line[i]);

		/////проверяем что это одна из разрешенных команд
		if (cmd == 'W' || cmd == 'A' || cmd == 'S' || cmd == 'D' || cmd == 'Q' || cmd == 'E')
			return cmd;

		printf("Неверная команда. Введите W/A/S/D, Q или E: ");
	}
}

/////выделение памяти под карту
////map[0] - указатель на первую строку
//////map[1] - указатель на вторую строку
/////ксли на каком то шаге память кончилась обязательно нужно освободить

char** mapCreate(int height, int width)
{
	char** map = (char**)malloc(height * sizeof(char*));
	if (!map) return NULL; ///память кончилась вернем NULL

	for (int i = 0; i < height; i++)
	{
		map[i] = (char*)malloc(width * sizeof(char));
		if (!map[i]) ////не выделилось открываем все что успели
		{
			for (int j = 0; j < i; j++) free(map[j]);///освободить строки 0...i-1
			free(map);////освободить массив указателей
			return NULL;
		}
	}
	return map;
}


/////освобождение памяти сначала строки потом массив, если наоборот то потреяем указатели на строки и получимутечку

void mapFree(char** map, int height)
{
	if (!map) return; ///NULL освобождать не надо
	for (int i = 0; i < height; i++) free(map[i]);
	free(map);
}

////генерируем карту и ставим героя на случайную клетку
void mapGenerate(char** map, int height, int width, int* heroX, int* heroY)
{
	///////случайный процент генерации от 15 до 30
	int obstaclePercent = OBSTACLE_PERCENT_MIN +
		rand() % (OBSTACLE_PERCENT_MAX - OBSTACLE_PERCENT_MIN + 1);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//////края это стены внутри свободно
			if (y == 0 || y == height - 1 || x == 0 || x == width - 1)
			{
				map[y][x] = MAP_WALL;
				continue; //////к следующей клетке
			}
			///////rand() % 100 дает 0..99 если меньше процента то припятствие
			int roll = rand() % 100;
			if (roll < obstaclePercent)
				map[y][x] = (rand() % 2 == 0) ? MAP_TREE : MAP_ROCK;
			///////если rand четный то будет дерево если нечет то камень примерно 50/50
			else
				map[y][x] = MAP_FLOOR;
		}
	}
	////////ищем свободную клетку для героя
	int hx, hy;
	do {
		hx = 1 + rand() % (width - 2); ////от 1 до width-2 не настенку
		hy = 1 + rand() % (height - 2);
	} while (map[hy][hx] != MAP_FLOOR);

	map[hy][hx] = MAP_HERO; ////поставили героя
	*heroX = hx; ///отдали координаты наружу через указатели
	*heroY = hy;
}

/////рисует карту каждому символу дается свой цвет
void mapPrint(char** map, int height, int width)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			char c = map[y][x];
			switch (c)
			{
			case MAP_HERO: setColor(14); break;///желтый егерой
			case MAP_WALL: setColor(8);  break;///серые стены
			case MAP_TREE: setColor(2);  break;///зеленые деревья
			case MAP_ROCK: setColor(7);  break;///белые камни
			default:       setColor(15); break;///все остальное белое
			}
			putchar(c); ///вывести один символ быстрее
		}
		putchar('\n');
	}
	setColor(7);
}

//////попытка шага 1-шаг сделан 0-нельзя препятствие -1-выход за край
int heroTryMove(char** map, int height, int width,
	int* heroX, int* heroY, char cmd)
{
	//////dx/dy смещение по осям
	int dx = 0, dy = 0;
	switch (cmd)
	{
	case 'W': dy = -1; break;
	case 'S': dy = 1; break;
	case 'A': dx = -1; break;
	case 'D': dx = 1; break;
	default:  return 0;////неизвестная команда значит ничего не происходит
	}

	int nx = *heroX + dx; ///новые кординаты
	int ny = *heroY + dy;
	//////проверяем обязательно границы
	if (nx < 0 || nx >= width || ny < 0 || ny >= height) return 0;
	/////проходимость только на пол
	if (map[ny][nx] != MAP_FLOOR) return 0;
	/////если все круто то перемещаем героя на новую клетку стирая с прошлой
	map[*heroY][*heroX] = MAP_FLOOR;
	*heroX = nx;
	*heroY = ny;
	map[*heroY][*heroX] = MAP_HERO;
	return 1;
}
////базовое приключение оставляю если пригодится, а так не используется
void adventureRun(void)
{
	int height = MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1);
	int width = MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1);

	char** map = mapCreate(height, width);
	if (!map)
	{
		printf("[!] Не удалось выделить память под карту.\n");
		return;
	}

	int heroX = 0, heroY = 0;
	mapGenerate(map, height, width, &heroX, &heroY);

	printf("\n=== ПРИКЛЮЧЕНИЕ ===\n");
	printf("Размер карты: %d x %d\n", height, width);
	printf("Управление: W-вверх A-влево S-вниз D-вправо Q-выйти\n\n");

	int running = 1;
	while (running)
	{
		clearScreen();
		mapPrint(map, height, width);
		printf("\nГерой в (%d, %d)\n", heroX, heroY);
		printf("Куда идём? > ");

		char cmd = readDirection();

		if (cmd == 'Q')
		{
			running = 0;
			continue;
		}

		if (!heroTryMove(map, height, width, &heroX, &heroY, cmd))
		{
			printf("[!] Туда нельзя.\n");
			Sleep(400);
		}
	}

	mapFree(map, height);
	printf("Возврат в ферму.\n");
}


////===================ВАРИАНТ 5 *ЛЕС КОЛЬЦО*============================
#define MAP_PORTAL 'P' ///портал
#define WANDER_TO_PORTAL 3 ///сколько блужданий до появления портала
#define MAP_GEN_ATTEMPTS 1000 ///лимит попыток найти свободную клетку
#define SLEEP_BAD_MOVE 400 ////пауза после "ттуда нельзя"
#define SLEEP_RETURN 800 ///пауза перед возвратом в меню

/////ищет случайную пустую клетку возращает 1 еесли нашел а 0 если не смог
///лиммит попыток чтобы не зациклиться если карта забита
int findFreeCell5(char** map, int height, int width, int* outX, int* outY)
{
	for (int attempt = 0; attempt < MAP_GEN_ATTEMPTS; attempt++)
	{
		int x = 1 + rand() % (width - 2);
		int y = 1 + rand() % (height - 2);
		if (map[y][x] == MAP_FLOOR)
		{
			*outX = x; ///вернуть координаты через указатели
			*outY = y;
			return 1;
		}
	}
	return 0; ///не нашли печально
}

/////генерация леса без стен по краям, край это выход за карту
void mapGenerate5(char** map, int height, int width, int* heroX, int* heroY)
{
	int obstaclePercent = OBSTACLE_PERCENT_MIN +
		rand() % (OBSTACLE_PERCENT_MAX - OBSTACLE_PERCENT_MIN + 1);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			////края заполняются как и все остальное
			int roll = rand() % 100;
			if (roll < obstaclePercent)
				map[y][x] = (rand() % 2 == 0) ? MAP_TREE : MAP_ROCK;
			else
				map[y][x] = MAP_FLOOR;
		}
	}
	////ставим героя на свободную клетку если не нашли за 1000 попыток
	//освобождаем центр и ставим туда
	int hx = 1, hy = 1;
	if (!findFreeCell5(map, height, width, &hx, &hy))
	{
		hx = width / 2;
		hy = height / 2;
		map[hy][hx] = MAP_FLOOR; ///освободили центр
	}
	map[hy][hx] = MAP_HERO;
	*heroX = hx;
	*heroY = hy;
}
//////отрисовка с цветом для портала
void mapPrint5(char** map, int height, int width)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			char c = map[y][x];
			switch (c)
			{
			case MAP_HERO:   setColor(14); break;
			case MAP_TREE:   setColor(2);  break;
			case MAP_ROCK:   setColor(7);  break;
			case MAP_PORTAL: setColor(13); break;//фиолетовый портал
			default:         setColor(15); break;
			}
			putchar(c);
		}
		putchar('\n');
	}
	setColor(7);
}


//////возращает 2 если шагнули на портал, запоминаем target до того как герой шагнул в портал и возращаем 2

int heroTryMove5(char** map, int height, int width,
	int* heroX, int* heroY, char cmd)
{
	int dx = 0, dy = 0;
	switch (cmd)
	{
	case 'W': dy = -1; break;
	case 'S': dy = 1; break;
	case 'A': dx = -1; break;
	case 'D': dx = 1; break;
	default:  return 0;
	}

	int nx = *heroX + dx;
	int ny = *heroY + dy;
	////выход за край это сигнал телепорта
	if (nx < 0 || nx >= width || ny < 0 || ny >= height)
		return -1;
	////запоминаем что на клетке до шага героя туда
	char target = map[ny][nx];


	if (map[ny][nx] != MAP_FLOOR && map[ny][nx] != MAP_PORTAL)
		return 0;
	///двигаем героя
	map[*heroY][*heroX] = MAP_FLOOR;
	*heroX = nx;
	*heroY = ny;
	map[*heroY][*heroX] = MAP_HERO;
	////если шагнули на портал сообщаем об это отдельным кодом 2
	if (target == MAP_PORTAL)
		return 2;

	return 1;
}
//////ставит портал P в случайную пустую клетку
///вызывается после 3 перехода за край карты
void mapPlacePortal(char** map, int height, int width)
{
	int px = 1, py = 1;
	if (findFreeCell5(map, height, width, &px, &py))
		map[py][px] = MAP_PORTAL; ///поставили портал
}

//////само приключение логика простая карта лес, герой ходит с помощью
//заданных клавиш, уходит за край-перемещается на другой край
//наступил на портал появляется новый лес
//Q домой только с портала
void adventureRun5(void)

{
	///случайный размер
	int height = MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1);
	int width = MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1);

	char** map = mapCreate(height, width);
	if (!map)
	{
		printf("[!] Не удалось выделить память под карту.\n");
		return;
	}

	int heroX = 0, heroY = 0;
	mapGenerate5(map, height, width, &heroX, &heroY);

	int wanderCount = 0; //сколько раз ушел за край на этой карте
	int distanceFromHome = 0;//сколько карт пройдено(дальность от дома)
	int onPortal = 0;//стоим на портале для Q

	printf("\n=== ПРИКЛЮЧЕНИЕ: ЛЕС-КОЛЬЦО (вариант 5) ===\n");
	printf("Размер карты: %d x %d\n", height, width);
	printf("Управление: W-вверх A-влево S-вниз D-вправо Q-домой\n");
	printf("Q - домой, но только с портала");
	printf("E - уйти в новый лес, но только с потрала");
	printf("Правила:\n");
	printf("Шаг за край -> телепорт на другой край этой же карты.\n");
	printf("После %d телепортов появится портал 'P'.\n", WANDER_TO_PORTAL);
	printf("Домой 'Q' можно только с портала 'P', E - дальше.\n\n");

	//////пауза чтобы успеть прочитать правила
	printf("Нажмите Enter,  чтобы начать");
	while (getchar() != '\n');

	int running = 1;
	while (running)
	{
		//отрисовка кадра
		clearScreen();
		printf("Дальность от дома: %d   Блужданий: %d/%d\n\n",
			distanceFromHome, wanderCount, WANDER_TO_PORTAL);
		mapPrint5(map, height, width);
		printf("\nГерой в (%d, %d)\n", heroX, heroY);

		if (onPortal)
			printf("Вы на портале! Q — отправиться домой, E - в новый лес, шаг - сойти.\n");

		printf("Куда идём? > ");
		char cmd = readDirection();

		////Q домой только с портала
		if (cmd == 'Q')
		{
			if (onPortal)
			{
				running = 0;
				continue;
			}
			printf("[!] Домой можно только с портала 'P'.\n");
			Sleep(SLEEP_BAD_MOVE);
			continue;
		}
		if (cmd == 'E')
		{
			if (!onPortal)
			{
				printf("[!] В новый лес можно только с портала.\n");
				Sleep(SLEEP_BAD_MOVE);
				continue;
			}
			printf("\n[*] Вы вошли в портал и ушли в новый лес...\n");
			Sleep(SLEEP_BAD_MOVE);
			///////ОБЯЗАТЕЛЬНО освободить старую карту
			mapFree(map, height);

			/////новый случайный размер
			height = MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1);
			width = MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1);

			//////новая карта
			map = mapCreate(height, width);
			if (!map)
			{
				printf("[!] Не удалось выделить память под новую карту.\n");
				return;
			}
			mapGenerate5(map, height, width, &heroX, &heroY);

			//////обновляем счётчики
			distanceFromHome++;
			wanderCount = 0;
			onPortal = 0;

			printf("[+] Новая карта: %d x %d. Дальность от дома: %d\n",
				height, width, distanceFromHome);
			Sleep(SLEEP_BAD_MOVE);
			continue;
		}

		//////снчала сходим с портала потом шагаем
		if (onPortal)
			onPortal = 0; ////сошли с портала


		//пробуем шагнуть
		int result = heroTryMove5(map, height, width, &heroX, &heroY, cmd);

		if (result == 0)//туда нельзя
		{
			printf("[!] Туда нельзя.\n");
			Sleep(SLEEP_BAD_MOVE);
			continue;
		}

		if (result == -1)//вышли за край-телепорт
		{
			map[heroY][heroX] = MAP_FLOOR;//без этой строки на карте будет две @

			///телепорт по модулю
			switch (cmd)
			{
			case 'W': heroY = height - 1; break;//верх-низ
			case 'S': heroY = 0;          break;//низ-верх
			case 'A': heroX = width - 1;  break;//лево-право
			case 'D': heroX = 0;          break;//право-лево
			}
			map[heroY][heroX] = MAP_HERO;

			wanderCount++;
			onPortal = 0;//сошли с портала если были на нем

			printf("[~] Вас занесло на другой край! Блужданий: %d\n", wanderCount);

			//////после 3 блужданий появляется портал
			if (wanderCount >= WANDER_TO_PORTAL)
				mapPlacePortal(map, height, width);

			Sleep(SLEEP_BAD_MOVE);
			continue;
		}

		if (result == 2)//наступили на портал - перешли на новую карту
		{
			onPortal = 1;
			printf("\n[*] Вы встали на портал \n");
			printf("    Q — домой, E — в новый лес, шаг — сойти с портала.\n");
			Sleep(SLEEP_BAD_MOVE);
			continue;
		}
		if (result == 1)
			onPortal = 0;
	}

	//обязательно освободили прошлую карту
	mapFree(map, height);
	printf("Возврат в ферму. Дальность от дома: %d\n", distanceFromHome);
	Sleep(SLEEP_RETURN);
}



//========================================================
int main()
{
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	loadItemsFromFile("items.txt");   // грузим каталог предметов
	askName();
	while (1)
	{
		printf("=== Фермер: %s ===\n", username);   //  шапка с именем
		printf("Меню \n [0] Выход \n [1] Посмотреть на часы \n [2] Промотать время (Поработать) \n [3] Посмотреть инвентарь \n [4] Положить предмет в слот \n [5] Выбросить предмет \n [6] Инверсия карманов \n [7] Поиск предмета в рюкзаке по названию \n [8] Записать состояние в дневник \n [9] Доска объявлений \n [10] Отправиться в приключение (лес-кольцо) \n");
		int choice = readInt();
		//меню с выбором действий
		switch (choice)
		{
		case(0):
		{
			return 0;
		}

		case(1):
		{
			system("cls");
			setColor(10);
			printf("Текущее время: день %d, время %d:00\n", current_day, current_hour);
			setColor(7);
			break;
		}

		case(2):
		{
			system("cls");
			timeToWork();
			break;
		}
		case(3):
		{
			system("cls");
			checkInventory();
			break;
		}

		case(4):
		{
			system("cls");
			svapItem();
			break;
		}

		case(5):
		{
			system("cls");
			dropItem();
			break;
		}

		case(6):
		{
			system("cls");
			reverseInventory();
			break;
		}
		case(7):
		{

			system("cls");
			checkInventory();
			searchItemByName();
			break;
		}
		case(8):
		{
			system("cls");
			saveDiary();
			break;
		}
		case(9):
		{
			system("cls");
			boardOfQuests();
			break;
		}
		case(10):
		{
			system("cls");
			adventureRun5();    // ВАРИАНТ 5
			break;
		}
		default:
			printf("Неверный пункт меню\n");
			break;
		}
	}
	return 0;
}
