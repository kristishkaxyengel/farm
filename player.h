// все что связано с персонажем
#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"

//структура рюкзака
typedef struct {
    int slots[MAX_ITEMS]; //массив(хранения предметов)
} Backpack;

//структура игрока
typedef struct {
    char name[MAX_NAME_LEN];
    int hp;
    int max_hp;
    int stamina; //выносливость
    int max_stamina;
    int gold;
    int x, y;
    Backpack backpack; //вложенная структура рюкзака
} Player;

//прототипы функций
Player* createPlayer(void); //создать игрока(выделить память под него)
void freePlayer(Player* player); //удалить игрока(очистить)
void checkInventory(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN]);
void swapItem(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN]);
void dropItem(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN]);
void searchItemByName(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN]); //поиск по названию
void reverseInventory(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN]); //реверс карманов
void saveDiary(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN], int day, int hour); //сохранить в файл
void boardOfQuests(Player* player, char item_names[MAX_ITEMS][MAX_NAME_LEN], int day); //доска объявлений

#endif