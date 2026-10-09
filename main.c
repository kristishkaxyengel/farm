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


int main()
{
	SetConsoleOutputCP(65001);
	while (1)
	{
		printf("Меню \n [0] Выход \n [1] Посмотреть на часы \n [2] Промотать время (Поработать) \n [3] Посмотреть инвентарь \n [4] Положить предмет в слот \n [5] Выбросить предмет \n [6] Выполнить задание по варианту \n");
		int choice;
		scanf_s("%d", &choice);
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
			printf("Выбор1\n");
			break;
		}
		case(3):
		{
			printf("Выбор2\n");

			break;
		}

		case(4):
		{
			printf("Выбор3\n");

			break;
		}

		case(5):
		{
			printf("Выбор4\n");

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