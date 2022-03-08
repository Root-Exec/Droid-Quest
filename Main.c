#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include "maps.h"

#ifndef DROIDFUNCTIONS_H
#include "droidFunctions.h"
#endif

#ifndef STDIO_H
#include <stdio.h>
#endif

extern Node wall;
extern Node vert_door;
extern Node hori_door;
extern Node path;
extern Node datapad;

Node Character;
Node Enemy;

extern int startX;
extern int startY;

int enemyLevel = 1;
int datapads = 0;

int main (void) {
	int current_x = startX;
	int current_y = startY;
	char command;

	printWelcomeScreen();
	initializeCharacter(&Character);
	initializeEnemy(&Enemy, enemyLevel);
	initializeTiles(&wall, &vert_door, &hori_door, &path, &power, &datapad);

	Node** map = loadMap("map1_template.txt", &enemyLevel);
	initializeEnemy(&Enemy, enemyLevel);

	map[current_x][current_y] = Character;
	map[current_x][current_y].Droid.icon = 'O';
	if (map == NULL) {
		return 1;
	}

	drawMap(map);

	while (1) {

		command = getchar();

		switch(command) {
			//up move
			case 'w':
				if (map[current_x - 1][current_y].Tile.icon != '*') {

					if (map[current_x - 1][current_y].Tile.icon == 'p') {
						powerUp(&Character);
					}

					if (map[current_x - 1][current_y].Tile.type == 0 &&
						map[current_x - 1][current_y].Droid.icon == 'E') {
						attack(&Character, map, current_x - 1, current_y);
						statusCheck(map, current_x - 1, current_y);
						break;
					}

					moveDroidUp(map, &Character, &current_x, &current_y);

					if (doorTransition(&map, &current_x, &current_y, &enemyLevel)) {
						initializeEnemy(&Enemy, enemyLevel);
					} else if (map[current_x][current_y].Tile.icon == 'D') {
						collectDatapad(map, &datapads, current_x, current_y);
					}
				}
				break;
			//left move
			case  'a':
				if (map[current_x][current_y - 1].Tile.icon != '*') {

					if (map[current_x][current_y - 1].Tile.icon == 'p') {
						powerUp(&Character);
					}

					if (map[current_x][current_y - 1].Tile.type == 0 &&
						map[current_x][current_y - 1].Droid.icon == 'E') {
						attack(&Character, map, current_x, current_y - 1);
						statusCheck(map, current_x, current_y - 1);
						break;
					}

					moveDroidLeft(map, &Character, &current_x, &current_y);

					if (doorTransition(&map, &current_x, &current_y, &enemyLevel)) {
						initializeEnemy(&Enemy, enemyLevel);
					}  else if (map[current_x][current_y].Tile.icon == 'D') {
						collectDatapad(map, &datapads, current_x, current_y);
					}

				}
				break;
			//down move
			case 's':
				if (map[current_x + 1][current_y].Tile.icon != '*') {

					if (map[current_x + 1][current_y].Tile.icon == 'p') {
						powerUp(&Character);
					}
					if (map[current_x + 1][current_y].Tile.type == 0 &&
						map[current_x + 1][current_y].Droid.icon == 'E') {
						attack(&Character, map, current_x + 1, current_y);
						statusCheck(map, current_x + 1, current_y);
						break;
					}

					moveDroidDown(map, &Character, &current_x, &current_y);
					if (doorTransition(&map, &current_x, &current_y, &enemyLevel)) {
						initializeEnemy(&Enemy, enemyLevel);
					}  else if (map[current_x][current_y].Tile.icon == 'D') {
						collectDatapad(map, &datapads, current_x, current_y);
					}
				}
				break;
			//right move
			case 'd':

				if (map[current_x][current_y + 1].Tile.icon != '*') {

					if (map[current_x][current_y + 1].Tile.icon == 'p') {
						powerUp(&Character);
					}

					if (map[current_x][current_y + 1].Tile.type == 0 &&
						map[current_x][current_y + 1].Droid.icon == 'E') {
						attack(&Character, map, current_x, current_y + 1);
						statusCheck(map, current_x, current_y + 1);
						break;
					}

					moveDroidRight(map, &Character, &current_x, &current_y);
					if (doorTransition(&map, &current_x, &current_y, &enemyLevel)) {
						initializeEnemy(&Enemy, enemyLevel);
					}  else if (map[current_x][current_y].Tile.icon == 'D') {
						collectDatapad(map, &datapads, current_x, current_y);
					}
				}
				break;

			case 'l':
				unloadMap(map);
				return 0;

		};

		cleanup(map, &Character, &current_x, &current_y);
		drawMap(map);

		if (Character.Droid.powerLevel <= 0) {
			printf("Droid powering down, out of power!\nYou lose!\n");
			unloadMap(map);
			return 0;
		}
		if (Character.Droid.health <= 0) {
			printf("Droid structural integrity down to zero!\nYou lose!\n");
			unloadMap(map);
			return 0;
		}

	}

	return 0;
}


