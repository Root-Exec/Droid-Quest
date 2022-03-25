#include "droidFunctions.h"
#include <time.h>

#define DROIDFUNCTIONS_H
#define MAXROW 30
#define MAXCOL 30
#define PATH_TYPE 1


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
extern int timer;

int prevDoor[2];
int nextDoor[2];
int nextMap;
int prevMap;
int startX = 27;
int startY = 24;
int c = 0;

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

void printInstructions(void) {
	struct timespec timeToContinue;
	timeToContinue.tv_sec = 0;
	timeToContinue.tv_nsec = 50000000;
	
	char* message = " Welcome to Droid Quest! You play as an astromech droid trying to escape the deathstar! \n"
					" Your job is to collect as much data on your escape from datapads denoted as 'D' on the map. \n"
					" But watch out for Imperial Soldiers on patrol! They are denoted as 'E' on the map. \n"
					" You can fight back though! Attempting to move onto an enemy space will initiate a battle. \n"
					" Be mindful of your enemies! \n" 
					" They get stronger with each level and reinforcements arrive when you leave that area! \n"
					" Between each level you can use the datapads to upgrade your different components: \n"
					" 1. Power Supply Module \n"
					" 2. Armor Regeneration Tech \n"
					" 3. Software Version (must be equivalent to the next level) \n"
					" 4. Weapons \n"
					" Don't overspend though! \n" 
					" Your score is based on how many datapads you have collected through the game! \n"
					" To move around, enter 'w' for up, 'a' for left, \n"
					" 's' for down, and 'd' for right into the command line. \n"
					" To execute command press enter. \n\n"
					" Good luck, we are counting on the data you are bringing back to command! \n";
	int a;
	for (int start = 0, end = 0; end < strlen(message); end++) {

		for (a = start; a <= end; a++) {
			printf("%c", message[a]);
			if (message[a] == '\n') break;
		}

		if (message[a] != '\n') printf("|");
		if (message[a] == '\n') {
			sleep(1);
			printf("\n");
			start = a + 1;
			end = start;
		} else {
			nanosleep(&timeToContinue, NULL);	
			printf("\r");
		}
		fflush(stdout);
	}
	__CLEARBUFFER;

	return;
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
	printf("\nPress 'c' to continue to game\n");
	printf("Press i to view game instructions\n");

	int c = getchar();
	while (c != 'c') {
		if (c == 'i') {
			printInstructions();
		}
		printf("\nPress 'c' to continue onto your mission!");
		c = getchar();
	}
	__CLEARBUFFER;
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
	system("Clear");
	fflush(stdout);
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
	printf("Time elapsed: %d\n", timer);
	printf("Current Coordinates: x: %d, y: %d\n", Character.Droid.y, MAXROW - Character.Droid.x - 1);
	printf("Software Version: %d\n", Character.Droid.softwareVersion);
	printf("Power Level: %d\n", Character.Droid.powerLevel);
	printf("Health: %d\n", Character.Droid.health);
	printf("Enemy health: %d  Enemy attack: %d\n", Enemy.Droid.health, Enemy.Droid.attack);
	printf("Datapads collected:  %d\n", datapads);
	printf("Command: \n");
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
		upgradeCharacter(Character, &datapads);
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
	__CLEARBUFFER;
	for (int i = 0; i < MAXROW; i++) printf("\r");
	return;

}

void moveEnemy(Node** map, Node* Character, Node* Enemy, int current_x, int current_y) {
	int tempX;
	int tempY;

	for (int x = current_x - 4; x < current_x + 4; x++) {

		if (x < 0) continue;
		if (x >= MAXROW) break;

		for (int y = current_y - 4; y < current_y + 4; y++) {
			if (y < 0) continue;
			if (y >= MAXCOL) break;

			if (map[x][y].Droid.icon == 'E' &&
			    map[x][y].Droid.moved == 0) {
				tempX = 0;
				tempY = 0;

				if (current_x - x < 0) tempX = -1;
				else if (current_x - x > 0) tempX = 1;

				if (current_y - y < 0) tempY = -1;
				else if (current_y - y > 0) tempY = 1;

				if (map[x + tempX][y + tempY].Tile.icon == ' ') {
					map[x][y] = path;
					map[x + tempX][y + tempY] = *Enemy;
					map[x + tempX][y + tempY].Droid.moved = 1;

				} else if (map[x + tempX][y].Tile.icon == ' ') {
					map[x][y] = path;
					map[x + tempX][y] = *Enemy;
					map[x + tempX][y].Droid.moved = 1;

				} else if (map[x][y + tempY].Tile.icon == ' ') {
					map[x][y] = path;
					map[x][y + tempY] = *Enemy;
					map[x][y + tempY].Droid.moved = 1;

				} else if (map[x + tempX][y + tempY].Droid.icon == 'O') {
					Character->Droid.health -= Enemy->Droid.attack;

				} else if (map[x + tempX][y].Droid.icon == 'O') {
					Character->Droid.health -= Enemy->Droid.attack;

				} else if (map[x][y + tempY].Droid.icon == 'O') {
					Character->Droid.health -= Enemy->Droid.attack;
				}
			}
		}
	}
	for (int x = current_x - 5; x < current_x + 5; x++) {
		if (x < 0) continue;
		if (x >= MAXROW) break;
	
		for (int y = current_y - 5; y < current_y + 5; y++) {
			if (y < 0) continue;
			if (y >= MAXCOL) break;

			if (map[x][y].Droid.icon == 'E') map[x][y].Droid.moved = 0;
		}
	}
	return;
}