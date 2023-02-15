#define STRING_H
#define UNISTD_H
#define STDLIB_H
#define STDIO_H
#define MAPS_H

#define NEW 0
#define OPEN 1
#define CLOSED 2

#include "maps.h"

int initializeCharacter(Node* Character);
int initializeEnemy(Node* Enemy, int level);

int powerUp(Node* Character);
int upgradeCharacter(Node* Character, int* datapads);
void moveDroid(Node** map, Node* Character, int changeX, int changeY, int* current_x, int* current_y);
void attack(Node* Character, Node** map, int x, int y);
void collectDatapad(Node* Character, int* datapads);
int compare(const void* a, const void* b);
Point* AIpath(Node** map, int startX, int startY, int* nextDoor, int* botX, int* botY);
