//
// Created by Nick Bonner on 2/15/23.
//

#ifndef DROID_QUEST_REPO_MAPS_H
#define DROID_QUEST_REPO_MAPS_H
#endif

#define MAPS_H
#define MAXROW 30
#define MAXCOL 30
#define PATH_TYPE 1

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

union node {

    struct droid {
        int type;
        int softwareVersion;
        int health;
        int attack;
        int batteryLimit;
        int powerLevel;
        int moved;
        int x, y;
        short powerSupplyValue;
        short armorRegenValue;
        char icon;
    } Droid;

    struct tile {
        int type;
        char name[20];
        char icon;
    } Tile;
};

typedef union node Node;
extern Node path;

typedef struct point {
    int status;                    //NEW, OPEN, CLOSED
    char icon;
    int x;
    int y;
    int distanceTravelled;         //Known distance travelled. = Parent Node dist + 1
    int predictedDistance;         //Heuristic value. Weighted value
    int moveValue; 				   //distanceTraveled + predictedDistance
    struct point* parent;		   //pointer to the point's parent
    int origin;
    int datapads;
    struct point* next;
} Point;

void initializeTiles(Node* wall, Node* vert_door, Node* hori_door, Node* path, Node* power, Node* datapad);
void printInstructions(void);
void printWelcomeScreen(void);

Node** allocateMap(void);

Node** loadMap(char* mapName, int* enemyLevel);
void unloadMap(Node** map);
void drawMap(Node** map);

int doorTransition(Node*** map, Node* Character, int* current_x, int* current_y, int* enemyLevel);
void readHeader(FILE* fp, int* nextMap, int* prevMap, int* nextDoor, int* prevDoor, int* enemyLevel);

void statusCheck(Node**map, int x, int y);
void moveEnemy(Node** map, Node* Character, Node* Enemy, int current_x, int current_y);

