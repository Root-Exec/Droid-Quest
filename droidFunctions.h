#define STRING_H
#define DROIDFUNCTIONS_H
#define DROID_TYPE 0
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


union node {

	struct droid {
		int type;
		int softwareVersion;
		int health;
		int attack;
		int batteryLimit;
		int powerLevel;
		int x, y;
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

char items[][15] = {"armor", "power supply", "weapon", "software"};

int initializeCharacter(Node* Character) {
	Character->Droid.type = 0;
	Character->Droid.softwareVersion = 0;
	Character->Droid.health = 100;
	Character->Droid.attack = 10;
	Character->Droid.batteryLimit = 100;
	Character->Droid.powerLevel = Character->Droid.batteryLimit;
	Character->Droid.icon = 'O';
	return 0;
}

int initializeEnemy(Node* Enemy, int level) {
	Enemy->Droid.type = 0;
	Enemy->Droid.softwareVersion = level;
	Enemy->Droid.health = level * 10;
	Enemy->Droid.attack = level * 5;
	Enemy->Droid.powerLevel = 100;
	Enemy->Droid.icon = 'E';
	return 0;
}

int powerUp(Node* Character) {
	Character->Droid.powerLevel += 30;
	if (Character->Droid.powerLevel > 100) Character->Droid.powerLevel = 102;
	Character->Droid.health += 20;
	if (Character->Droid.health >= 100) Character->Droid.health = 100;
	return 0;
}

int upgradeCharacter(char* item, Node* Character) {
	if (strcmp(item, "power supply") == 0) Character->Droid.batteryLimit += 10;
	if (strcmp(item, "weapon") == 0) Character->Droid.attack += 5;
	return 0;
}

void moveDroidUp(Node** map, Node* Character, int* current_x, int* current_y) {
	Character->Droid.powerLevel -= 2;
	map[*current_x][*current_y] = path;
	map[*current_x][*current_y].Tile.icon = ' ';
	(*current_x)--;
	map[*current_x][*current_y] = *Character;
	map[*current_x][*current_y].Droid.icon = 'O';

	return;
}

void moveDroidLeft(Node** map, Node* Character, int* current_x, int* current_y) {
	Character->Droid.powerLevel -= 2;
	map[*current_x][*current_y] = path;
	map[*current_x][*current_y].Tile.icon = ' ';
	(*current_y)--;
	map[*current_x][*current_y] = *Character;
	map[*current_x][*current_y].Droid.icon = 'O';

	return;
}

void moveDroidDown(Node** map, Node* Character, int* current_x, int* current_y) {
	Character->Droid.powerLevel -= 2;
	map[*current_x][*current_y] = path;
	map[*current_x][*current_y].Tile.icon = ' ';
	(*current_x)++;
	map[*current_x][*current_y] = *Character;
	map[*current_x][*current_y].Droid.icon = 'O';

	return;
}

void moveDroidRight(Node** map, Node* Character, int* current_x, int* current_y) {
	Character->Droid.powerLevel -= 2;
	map[*current_x][*current_y] = path;
	map[*current_x][*current_y].Tile.icon = ' ';
	(*current_y)++;
	map[*current_x][*current_y] = *Character;
	map[*current_x][*current_y].Droid.icon = 'O';

	return;
}

void attack(Node* Character, Node** map, int x, int y) {

	Character->Droid.health -= map[x][y].Droid.attack;
	map[x][y].Droid.health -= Character->Droid.attack;

	return;
}

void collectDatapad(Node* Character, int* datapads) {
		int value = arc4random_uniform(4);
		if (value <= 10) Character->Droid.softwareVersion += 1;
		(*datapads)++;
		return;
}
