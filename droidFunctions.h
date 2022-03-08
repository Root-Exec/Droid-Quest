#define STRING_H
#define DROIDFUNCTIONS_H
#define DROID_TYPE 0
#include <string.h>
#include <stdlib.h>


union node {

	struct droid {
		int type;
		char softwareVersion[10];
		int health;
		int attack;
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

int initializeCharacter(Node* Character) {
	Character->Droid.type = 0;
	strcpy(Character->Droid.softwareVersion, "1.0.0");
	Character->Droid.health = 100;
	Character->Droid.attack = 10;
	Character->Droid.powerLevel = 100;
	Character->Droid.icon = 'O';
	return 0;
}

int initializeEnemy(Node* Enemy, int level) {

	Enemy->Droid.type = 0;
	strcpy(Enemy->Droid.softwareVersion, "Enemy");
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

int upgradeCharacter(char* item, int value, Node* Character) {


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

void collectDatapad(Node** map, int* datapads, int x, int y) {
		(*datapads)++;
		map[x][y] = path;
		return;
}
