#include "droidFunctions.h"

short powerSupplyCost = 1;
short attackUpgradeCost = 1;
short softwareUpgradeCost = 1; 
short armorRegenUpgradeCost = 1;
int c;


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

int compare(const void* a, const void* b) {
	Point* pt_a = (Point*)a;
	Point* pt_b = (Point*)b;

	return pt_a->moveValue - pt_b->moveValue;
}


Point* AIpath(Node** map, int startX, int startY, int* nextDoor, int* botX, int* botY) {
	
	Point paths[30][30] = {0};
	Point current;
	Point* neighbor;
	Point openList[30 * 30];
	Point* endPoint = NULL;

	int open_index = 0;

	memset(&current, 0, sizeof(Point));
	
	//set up grid of Points
	for (int a = 0; a < 30; a++) {
		
		for (int b = 0; b < 30; b++) {
			paths[a][b].status = NEW;
			if (map[a][b].Tile.type == 1)
				paths[a][b].icon = map[a][b].Tile.icon;
			else
				paths[a][b].icon = map[a][b].Droid.icon;
			paths[a][b].x = a;
			paths[a][b].y = b;
			paths[a][b].distanceTravelled = -1;
			paths[a][b].predictedDistance = -1;
			paths[a][b].moveValue = -1;
			paths[a][b].parent = NULL;
			paths[a][b].origin = 0;
		}
		
	}

	//set starting Point
	paths[startX][startY].origin = 1;
	current.status = OPEN;
	current.x = startX;
	current.y = startY;
	current.distanceTravelled = 0;
	current.predictedDistance = abs(startX - nextDoor[0]) + abs(startY - nextDoor[1]);
	current.moveValue = current.distanceTravelled + current.predictedDistance;
	current.parent = NULL;
	current.origin = 1;
	openList[open_index++] = current;

	while (open_index >= 0) {

		memset(&current, 0, sizeof(Point));

		memcpy(&current, &openList[0], sizeof(Point));
	
		if (current.x == nextDoor[0] && current.y == nextDoor[1]) {
			endPoint = &paths[current.x][current.y];
			break;
		}

		neighbor = &paths[current.x - 1][current.y];
		if (neighbor->icon != '*' && current.x - 1 >= 0 && current.status != CLOSED) {
			//valid point found

			if (neighbor->status == OPEN && neighbor->origin == 0) {
				if (neighbor->distanceTravelled >= current.distanceTravelled + 1) {
					neighbor->parent = &paths[current.x][current.y];
					neighbor->distanceTravelled = current.distanceTravelled + 1;
					neighbor->predictedDistance = abs(current.x - nextDoor[0]) + abs(current.y - nextDoor[1]);
					neighbor->moveValue = neighbor->distanceTravelled + neighbor->predictedDistance;
				}
			} else if (neighbor->status == CLOSED) {
				if (neighbor->distanceTravelled >= current.distanceTravelled + 1) {
					neighbor->parent = &paths[current.x][current.y];
					neighbor->distanceTravelled = current.distanceTravelled + 1;
					neighbor->predictedDistance = abs(current.x - nextDoor[0]) + abs(current.y - nextDoor[1]);
					neighbor->moveValue = neighbor->distanceTravelled + neighbor->predictedDistance;
					neighbor->status = OPEN;
				}
			}

			if (neighbor->status == NEW) {
				neighbor->status = OPEN;
				neighbor->x = current.x - 1;
				neighbor->y = current.y;
				neighbor->distanceTravelled = current.distanceTravelled++;
				neighbor->predictedDistance = abs(neighbor->x - nextDoor[0]) + abs(neighbor->y - nextDoor[1]);
				neighbor->moveValue = neighbor->distanceTravelled + neighbor->predictedDistance;
				neighbor->parent = &paths[current.x][current.y];
				openList[open_index++] = *neighbor;
			} 
		}


		neighbor = &paths[current.x + 1][current.y];
		if (neighbor->icon != '*' && current.x + 1 <= 29 && current.status != CLOSED) {
			//valid point found

			if (neighbor->status == OPEN && neighbor->origin == 0) {
				if (neighbor->distanceTravelled >= current.distanceTravelled + 1) {
					neighbor->parent = &paths[current.x][current.y];
					neighbor->distanceTravelled = current.distanceTravelled + 1;
					neighbor->predictedDistance = abs(current.x - nextDoor[0]) + abs(current.y - nextDoor[1]);
					neighbor->moveValue = neighbor->distanceTravelled + neighbor->predictedDistance;
				}
			} else if (neighbor->status == CLOSED) {
				if (neighbor->distanceTravelled >= current.distanceTravelled + 1) {
					neighbor->parent = &paths[current.x][current.y];
					neighbor->distanceTravelled = current.distanceTravelled + 1;
					neighbor->predictedDistance = abs(current.x - nextDoor[0]) + abs(current.y - nextDoor[1]);
					neighbor->moveValue = neighbor->distanceTravelled + neighbor->predictedDistance;
					neighbor->status = OPEN;
					openList[open_index++] = *neighbor;
				}
			}
			

			if (neighbor->status == NEW) {
				neighbor->status = OPEN;
				neighbor->x = current.x + 1;
				neighbor->y = current.y;
				neighbor->distanceTravelled = current.distanceTravelled++;
				neighbor->predictedDistance = abs(neighbor->x - nextDoor[0]) + abs(neighbor->y - nextDoor[1]);
				neighbor->moveValue = neighbor->distanceTravelled + neighbor->predictedDistance;
				neighbor->parent = &paths[current.x][current.y];
				openList[open_index++] = *neighbor;
			}
		}
		

		neighbor = &paths[current.x][current.y + 1];
		if (neighbor->icon != '*' && current.y + 1 <= 29) {
			//valid point found

			if (neighbor->status == OPEN && neighbor->origin == 0) {
				if (neighbor->distanceTravelled >= current.distanceTravelled + 1) {
					neighbor->parent = &paths[current.x][current.y];
					neighbor->distanceTravelled = current.distanceTravelled + 1;
					neighbor->predictedDistance = abs(current.x - nextDoor[0]) + abs(current.y - nextDoor[1]);
					neighbor->moveValue = neighbor->distanceTravelled + neighbor->predictedDistance;
				}
			} else if (neighbor->status == CLOSED) {
				if (neighbor->distanceTravelled >= current.distanceTravelled + 1) {
					neighbor->parent = &paths[current.x][current.y];
					neighbor->distanceTravelled = current.distanceTravelled + 1;
					neighbor->predictedDistance = abs(current.x - nextDoor[0]) + abs(current.y - nextDoor[1]);
					neighbor->moveValue = neighbor->distanceTravelled + neighbor->predictedDistance;
					neighbor->status = OPEN;
					openList[open_index++] = *neighbor;
				}
			}

			if (neighbor->status == NEW) {
				neighbor->status = OPEN;
				neighbor->x = current.x;
				neighbor->y = current.y + 1;
				neighbor->distanceTravelled = current.distanceTravelled++;
				neighbor->predictedDistance = abs(neighbor->x - nextDoor[0]) + abs(neighbor->y - nextDoor[1]);
				neighbor->moveValue = neighbor->distanceTravelled + neighbor->predictedDistance;
				neighbor->parent = &paths[current.x][current.y];
				openList[open_index++] = *neighbor;
			}
		}	


		neighbor = &paths[current.x][current.y - 1];
		if (neighbor->icon != '*' && current.y - 1 >= 0) {
			//valid point found

			if (neighbor->status == OPEN && neighbor->origin == 0) {
				if (neighbor->distanceTravelled >= current.distanceTravelled + 1) {
					neighbor->parent = &paths[current.x][current.y];
					neighbor->distanceTravelled = current.distanceTravelled + 1;
					neighbor->predictedDistance = abs(current.x - nextDoor[0]) + abs(current.y - nextDoor[1]);
					neighbor->moveValue = neighbor->distanceTravelled + neighbor->predictedDistance;
				}
			} else if (neighbor->status == CLOSED) {
				if (neighbor->distanceTravelled >= current.distanceTravelled + 1) {
					neighbor->parent = &paths[current.x][current.y];
					neighbor->distanceTravelled = current.distanceTravelled + 1;
					neighbor->predictedDistance = abs(current.x - nextDoor[0]) + abs(current.y - nextDoor[1]);
					neighbor->moveValue = neighbor->distanceTravelled + neighbor->predictedDistance;
					neighbor->status = OPEN;
					openList[open_index++] = *neighbor;
				}
			}

			if (neighbor->status == NEW) {
				neighbor->status = OPEN;
				neighbor->x = current.x;
				neighbor->y = current.y - 1;
				neighbor->distanceTravelled = current.distanceTravelled++;
				neighbor->predictedDistance = abs(neighbor->x - nextDoor[0]) + abs(neighbor->y - nextDoor[1]);
				neighbor->moveValue = neighbor->distanceTravelled + neighbor->predictedDistance;
				neighbor->parent = &paths[current.x][current.y];
				openList[open_index++] = *neighbor;
			}
		}

		//pop first element off list
		for (int i = 0; i < 30 * 30; i++) {
			if (i < open_index) {
				openList[i] = openList[i + 1];
			}
			else memset(&openList[i], 0, sizeof(Point));
		}

		paths[current.x][current.y].status = CLOSED;
		open_index--;
		qsort(&openList, open_index, sizeof(Point), compare);
		
	}
	

	if (endPoint != NULL) {
		while (endPoint->origin == 0) {	
			endPoint->parent->next = endPoint;
			endPoint = endPoint->parent;
			map[endPoint->x][endPoint->y].Tile.icon = '#';
		}

		*botX = endPoint->x;
		*botY = endPoint->y;

		return endPoint;	
	}
	return NULL;
}