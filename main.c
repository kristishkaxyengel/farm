#include <stdio.h>
#include <locale.h> 
#include <windows.h>

// объявляем глобальные пременные
int current_day = 1;
int current_hour = 8;
// объявляем два массива для хранения предметов
char* itemNames[10] = {
 "Пусто",
 "Дерево",
 "Камень",
 "Семена",
 "Конфетка",
 "Клубника",
 "Сено",
 "Кукуруза",
 "Мясо",
 "Вода"
};
int inventory[10] = { 0, 2, 8, 5, 1, 8, 3, 5, 4, 2 };

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
	printf("Текущее время: день %d, время %d:00\n", current_day, current_hour);

}

void checkInventory()
{
	//перебираем инвентарь и выводим
	for (int i = 0; i < 10; i++)
	{
		if (inventory[i] == 0)
			printf("Слот %d: пусто\n", i);
		else
			printf("Слот %d: [%d] (%s)\n", i, inventory[i], itemNames[inventory[i]]);

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
	for (int i = 0; i < 10; i++)
	{
		if (itemNames[i] == 0)
			printf("Слот %d: пусто\n", i);
		else
			printf("Предмет %d: (%s)\n", i, itemNames[i]);

	}

	printf("Введите ID предмета (0-9): ");
	id = readInt();
	if (id < 0 || id > 9) { printf("Неверный ID предмета!\n"); return; }

	inventory[slot] = id;
	printf("В слот %d положен предмет [%d] (%s)\n", slot, id, itemNames[id]);
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

int main()
{
	SetConsoleOutputCP(65001);
	while (1)
	{
		printf("Меню \n [0] Выход \n [1] Посмотреть на часы \n [2] Промотать время (Поработать) \n [3] Посмотреть инвентарь \n [4] Положить предмет в слот \n [5] Выбросить предмет \n [6] Выполнить задание по варианту \n");
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
			printf("Текущее время: день %d, время %d:00\n", current_day, current_hour);
			break;
		}

		case(2):
		{
			timeToWork();
			break;
		}
		case(3):
		{
			checkInventory();
			break;
		}

		case(4):
		{
			svapItem();
			break;
		}

		case(5):
		{
			dropItem();

			break;
		}

		case(6):
		{
			printf("Выбор5\n");

			break;
		}

		default: printf("Неверный пункт меню\n"); break;
		}

	}
	return 0;
}