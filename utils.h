///файл для вспомогательных функций
#ifndef UTILS_H //защита от двойного приключения
#define UTILS_H

#include <stdio.h>
#include <stdlib.h> //для работы с памятью
#include <string.h>
#include <windows.h>

#define MAX_ITEMS 10 //макс кол-во слотов вв рюкзаке
#define MAX_NAME_LEN 32 //макс длина имени игрока или предмета

//прототипы функций,что другие файлы знали что такие функцииесть
int readInt(void); //ввод целого числа с клавиатуры
void setColor(int color); //смена цвета
void clearScreen(void); //очистка экрана
void loadItemsFromFile(char item_names[MAX_ITEMS][MAX_NAME_LEN], const char* filename); //загрузка названий предметов из файла
void showTime(int day, int hour); //вывод текущего времени

#endif //конец проверки