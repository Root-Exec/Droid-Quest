#define STRING_H
#define UNISTD_H
#define STDLIB_H
#define STDIO_H

#define __CLEARBUFFER while ((c = getchar()) != '\n' && c != EOF);
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

short powerSupplyCost = 1;
short attackUpgradeCost = 1;
short softwareUpgradeCost = 1; 
short armorRegenUpgradeCost = 1;
int c;

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

int initializeCharacter(Node* Character) {
	Character->Droid.type = 0;
	Character->Droid.softwareVersion = 0;
	Character->Droid.health = 100;
	Character->Droid.attack = 20;
	Character->Droid.batteryLimit = 100;
	Character->Droid.powerLevel = Character->Droid.batteryLimit;
	Character->Droid.icon = 'O';
	Character->Droid.powerSupplyValue = 25;
	Character->Droid.armorRegenValue = 25;
	return 0;
}

int initializeEnemy(Node* Enemy, int level) {
	Enemy->Droid.type = 0;
	Enemy->Droid.softwareVersion = level;
	Enemy->Droid.health = level * 10;
	Enemy->Droid.attack = level * 5;
	Enemy->Droid.powerLevel = 100;
	Enemy->Droid.moved = 0;
	Enemy->Droid.icon = 'E';
	return 0;
}

int powerUp(Node* Character) {
	Character->Droid.powerLevel += Character->Droid.powerSupplyValue;
	if (Character->Droid.powerLevel > 100) Character->Droid.powerLevel = 102;
	Character->Droid.health += Character->Droid.armorRegenValue;
	if (Character->Droid.health >= 100) Character->Droid.health = 100;
	return 0;
}

int upgradeCharacter(Node* Character, int* datapads) {
	short usr = 0;

	while (usr != 'c' && *datapads > 0) {
		
		system("clear");
		printf("        _______             \n");
		printf("       |  |*|  |            \n");
		printf("      _|_______|_           \n");
		printf("     | | _____ | |          \n");
		printf("     | | _____ | |          \n");
		printf("     |_|_______|_|          \n");
		printf("_____|_|__|_|__|_|______    \n");
		printf("      Droid Upgrade!        \n");
		printf("  Spend datapads to upgrade  \n");
		printf("   Datapads available:        %d\n", *datapads);
		printf("1. Upgrade Power Supply Port: %d\n", powerSupplyCost);
		printf("2. Upgrade Attack Module:     %d\n", attackUpgradeCost);
		printf("3. Upgrade Droid Software:    %d\n", softwareUpgradeCost);
		printf("4. Upgrade Amor Regeneration: %d\n", armorRegenUpgradeCost);
		printf("Press 'c' when ready to continue...\n");
		printf("Input upgrade:  \n");

		usr = getchar();

		switch (usr) {

			case '1':
				Character->Droid.powerSupplyValue += 5;
				*datapads -= powerSupplyCost;
				powerSupplyCost++;
				break;

			case '2':
				Character->Droid.attack += 10;
				*datapads -= attackUpgradeCost;
				attackUpgradeCost++;
				break;

			case '3':
				Character->Droid.softwareVersion++;
				*datapads -= softwareUpgradeCost;
				softwareUpgradeCost++;
				break;

			case '4':
				Character->Droid.armorRegenValue += 5;
				*datapads -= armorRegenUpgradeCost;
				armorRegenUpgradeCost++;
				break;
		}
		
	}
	return 0;
}

void moveDroid(Node** map, Node* Character, int changeX, int changeY, int* current_x, int* current_y) {
	Character->Droid.powerLevel -= 2;
	map[*current_x][*current_y] = path;
	*current_x += changeX;
	*current_y += changeY;
	map[*current_x][*current_y] = *Character;
	Character->Droid.x = *current_x;
	Character->Droid.y = *current_y;
	return;
}

void attack(Node* Character, Node** map, int x, int y) {

	Character->Droid.health -= map[x][y].Droid.attack;
	map[x][y].Droid.health -= Character->Droid.attack;
	return;
}

void collectDatapad(Node* Character, int* datapads) {
		(*datapads)++;
		return;
}
