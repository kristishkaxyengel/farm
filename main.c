#include <stdio.h>
#include <locale.h> 
#include <windows.h>
#include <string.h>

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

int main()
{
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	loadItemsFromFile("items.txt");   // грузим каталог предметов
	askName();
	while (1)
	{
		printf("=== Фермер: %s ===\n", username);   //  шапка с именем
		printf("Меню \n [0] Выход \n [1] Посмотреть на часы \n [2] Промотать время (Поработать) \n [3] Посмотреть инвентарь \n [4] Положить предмет в слот \n [5] Выбросить предмет \n [6] Инверсия карманов \n");
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

		default: printf("Неверный пункт меню\n"); break;
		}
	}
	return 0;
}
//пришлось блин менять все чтобы из файла читалось мне не нрав это (дизлайк)
// а еще коментов так много потому что я не хочу забыть что и как работает, а то потом буду ломать голову