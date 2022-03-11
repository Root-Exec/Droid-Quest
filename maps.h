#include "droidFunctions.h"
#include <stdio.h>
#include <unistd.h>

#define STDIO_H
#define MAXROW 30
#define MAXCOL 30
#define PATH_TYPE 1
#define __CLEARBUFFER while ((c = getchar()) != '\n');

Node wall;
Node vert_door;
Node hori_door;
Node path;
Node power;
Node datapad;

extern Node Character;
extern Node Enemy;
extern int enemyLevel;
extern int datapads;

int prevDoor[2];
int nextDoor[2];
int nextMap;
int prevMap;
int startX = 27;
int startY = 24;
int c;

char mapList[][20] = {"map1_template.txt", "map2_template.txt", "map3_template.txt"};


void initializeTiles(Node* wall, Node* vert_door, Node* hori_door, Node* path, Node* power, Node* datapad) {

	wall->Tile.icon = '*';
	strcpy(wall->Tile.name, "Wall");
	wall->Tile.type = 1;

	vert_door->Tile.icon = '|';
	strcpy(vert_door->Tile.name, "Vertical Door");
	vert_door->Tile.type = 1;

	hori_door->Tile.icon = '-';
	strcpy(vert_door->Tile.name, "Horizontal Door");
	hori_door->Tile.type =1;

	path->Tile.icon = ' ';
	strcpy(path->Tile.name, "Path");
	path->Tile.type = 1;

	power->Tile.icon = 'p';
	strcpy(power->Tile.name, "Power Port");
	power->Tile.type = 1;

	datapad->Tile.icon = 'D';
	strcpy(datapad->Tile.name, "Datapad");
	datapad->Tile.type = 1;
}

void printWelcomeScreen(){

	printf("  _____    ___   ___       ____    \n");
	printf("  |     \\ |   \\ /   \\  |  |    \\   \n");
	printf("  |     | |   | |   |  |  |    |   \n");
	printf("  |     | |___/ |   |  |  |    |   \n");
	printf("  |____/  |   \\ \\___/  |  |____/   \n");

	printf("  ____           _____  _____  ______    \n");
	printf(" /    \\   |    | |     |          |      \n");
	printf("|     |   |    | |____ |_____     |      \n");
	printf("|     |   |    | |           |    |      \n");
	printf(" \\____/\\  \\____/ |____  _____|    |      \n");

	printf("        _______             \n");
	printf("       |  |*|  |            \n");
	printf("      _|_______|_           \n");
	printf("     | | _____ | |          \n");
	printf("     | | _____ | |          \n");
	printf("     |_|_______|_|          \n");
	printf("_____|_|__|_|__|_|______    \n");
	printf("                            \n");

	printf("Controls: \n");
	printf("w: move character up\n");
	printf("s: move character down\n");
	printf("a: move character left\n");
	printf("d: move character right\n");
	printf("\nPress 'c' to continue...");

	int c = getchar();
	while (c != 'c') {
		c = getchar();
	}
	while ((c = getchar()) != '\n');
	system("clear");
	return;

}

Node** allocateMap(void) {

	Node** map = calloc(MAXROW, sizeof(Node*));
	for (int i = 0; i < MAXROW; i++) {
		map[i] = calloc(MAXCOL, sizeof(Node));
	}

	if (map == NULL) {
		printf("Map failed to load\n");
		return NULL;
	}
	return map;
}

void readHeader(FILE* fp, int* nextMap, int* prevMap, int* nextDoor, int* prevDoor, int* enemyLevel){
	char icon;
	char buff[5];
	int i = 0;

	//nextmap data
	while ((icon = fgetc(fp)) != '\n') {
		buff[i++] = icon;
	}
	buff[i] = '\0';
	i = 0;
	*nextMap = atoi(buff);
	memset(buff, 0, sizeof(char) * 5);

	//prevmap data
	while ((icon = fgetc(fp)) != '\n') {
		buff[i++] = icon;
	}
	buff[i] = '\0';
	i = 0;
	*prevMap = atoi(buff);
	memset(buff, 0, sizeof(char) * 5);

	//nextDoor data
	while ((icon = fgetc(fp)) != '\n') {
		buff[i++] = icon;
	}
	buff[i] = '\0';
	i = 0;
	nextDoor[0] = atoi(buff);
	memset(buff, 0, sizeof(char) * 5);

	while ((icon = fgetc(fp)) != '\n') {
		buff[i++] = icon;
	}
	buff[i] = '\0';
	i = 0;
	nextDoor[1] = atoi(buff);
	memset(buff, 0, sizeof(char) * 5);

	//prevDoor data
	while ((icon = fgetc(fp)) != '\n') {
		buff[i++] = icon;
	}
	buff[i] = '\0';
	i = 0;
	prevDoor[0] = atoi(buff);
	memset(buff, 0, sizeof(char) * 5);

	while ((icon = fgetc(fp)) != '\n') {
		buff[i++] = icon;
	}
	buff[i] = '\0';
	i = 0;
	prevDoor[1] = atoi(buff);
	memset(buff, 0, sizeof(char) * 5);

	//enemyLevel data
	while ((icon = fgetc(fp)) != '\n') {
		buff[i++] = icon;
	}
	buff[i] = '\0';
	i = 0;
	*enemyLevel = atoi(buff);
	memset(buff, 0, sizeof(char) * 5);

	return;
}

Node** loadMap(char* mapName, int* enemyLevel) {

	FILE* fp = fopen(mapName, "r");
	if (fp == NULL) {
		printf("Unable to load map template\n");
		return NULL;
	}

	int a = 0;
	int b = 0;
	int icon;

	Node** map = calloc(30, sizeof(Node*));
	for (int i = 0; i < 30; i++) {
		map[i] = calloc(30, sizeof(Node));
	}

	if (map == NULL) {
		printf("Map failed to load\n");
		return NULL;
	}

	readHeader(fp, &nextMap, &prevMap, nextDoor, prevDoor, enemyLevel);

	while ((icon = fgetc(fp)) != EOF) {

		switch (icon) {

			case '*':
				map[a][b] = wall;
				b++;
				break;

			case '|':
				map[a][b] = vert_door;
				b++;
				break;

			case '_':
				map[a][b] = hori_door;
				b++;
				break;

			case ' ':
				map[a][b] = path;
				b++;
				break;

			case 'p':
				map[a][b] = power;
				b++;
				break;

			case 'E':
				map[a][b] = Enemy;
				b++;
				break;

			case 'D':
				map[a][b] = datapad;
				b++;
				break;

			default:
				a++;
				b = 0;
				break;
		}
	}
	fclose(fp);
	fp = NULL;
	return map;
}

void drawMap(Node** map) {
	for (int a = 0; a < MAXROW; a++) {
		for (int b = 0; b < MAXCOL; b++) {
			if (map[a][b].Tile.type == 1)
				printf("%c ", map[a][b].Tile.icon);
			else
				printf("%c ", map[a][b].Droid.icon);
		}
		printf("\n");
	}

	printf("\nDroid Data: \n");
	printf("Current Coordinates: x: %d, y: %d\n", Character.Droid.y, MAXROW - Character.Droid.x - 1);
	printf("Software Version: %d\n", Character.Droid.softwareVersion);
	printf("Power Level: %d\n", Character.Droid.powerLevel);
	printf("Health: %d\n", Character.Droid.health);
	printf("Enemy health: %d  Enemy attack: %d\n", Enemy.Droid.health, Enemy.Droid.attack);
	printf("Datapads collected:  %d\n", datapads);
	printf("Command: ");
}

int doorTransition(Node*** map, Node* Character, int* current_x, int* current_y, int* enemyLevel) {

	if (*current_x == prevDoor[0] && *current_y == prevDoor[1]){

		*map = loadMap(mapList[prevMap], enemyLevel);
		*current_x = nextDoor[0];
		*current_y = nextDoor[1];

		if (*current_x == MAXROW - 1)       (*current_x)--;
		else if(*current_x == 0)            (*current_x)++;
		else if(*current_y == MAXCOL - 1)   (*current_y)--;
		else if(*current_y == 0)            (*current_y)++;
		return 1;

	} else if (*current_x == nextDoor[0] && *current_y == nextDoor[1]) {
		if (Character->Droid.softwareVersion < *enemyLevel) {
			printf("Insufficient software version to continue! Collect more datapads for software upgrades.\n");
			sleep(3);
			return 0;
		}
		*map = loadMap(mapList[nextMap], enemyLevel);
		*current_x = prevDoor[0];
		*current_y = prevDoor[1];

		if (*current_x == MAXROW - 1)        (*current_x)--;
		else if(*current_x == 0)             (*current_x)++;
		else if(*current_y == MAXCOL - 1)    (*current_y)--;
		else if(*current_y == 0)             (*current_y)++;
		return 1;
	}

	return 0;
}

void statusCheck(Node**map, int x, int y) {
	if (map[x][y].Droid.health <= 0) {
		map[x][y] = path;
		//initializeEnemy(Enemy, enemyLevel);
	}
	return;
}

void unloadMap(Node** map) {
	for (int i = 0; i < MAXROW; i++) {
		free(map[i]);
	}
}

void cleanup(Node** map, Node* Character, int* current_x, int* current_y) {
	int c;
	Character->Droid.x = *current_x;
	Character->Droid.y = *current_y;
	map[*current_x][*current_y] = *Character;
	map[*current_x][*current_y].Droid.icon = 'O';
	__CLEARBUFFER;
	system("clear");
	return;

}




