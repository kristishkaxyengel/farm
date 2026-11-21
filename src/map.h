//заголовочный файл для игрового мира
#ifndef MAP_H
#define MAP_H

#include "player.h" //нужен игрок

#define MAP_MIN_SIZE 5 //мин размер карты
#define MAP_MAX_SIZE 30 //макс размер карты
#define MAP_FLOOR '.' 
#define MAP_TREE  'T'
#define MAP_ROCK  'R'
#define MAP_HERO  '@'
#define MAP_PORTAL 'P'

#define WANDER_TO_PORTAL 3
#define SLEEP_BAD_MOVE 800 //задержка экрана

typedef struct { //структура карты
    char** grid; //двумерный динамический массив символов
    int width;
    int height;
    int floors_passed; //счетчик этажей
} Map;
//прототипы функций для работы с картой
Map* createMap(int height, int width); //создание карты(выделение памяти)
void freeMap(Map* map); //удалить(очиститть)
char readDirection(void); //прочитать нажатую клавишу
void adventureRun(Player* player, Map** map_ptr); //запуск игры

#endif