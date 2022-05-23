#include <sys/select.h>
#include "maps.h"
#include <termios.h>

#ifndef DROIDFUNCTIONS_H
#include "droidFunctions.h"
#endif

#define MOVELEFT (current_y - 1)
#define MOVERIGHT (current_y + 1)
#define MOVEUP (current_x - 1)
#define MOVEDOWN (current_x + 1)

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

struct timeval timeout;

void initTerminal(struct termios* defaultTerminal, struct termios* gameTerminal) {
	tcgetattr(STDIN_FILENO, defaultTerminal);
	*gameTerminal = *defaultTerminal;
	gameTerminal->c_lflag &= (~ICANON);
	gameTerminal->c_lflag &= (~ECHO);
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, gameTerminal) == -1)
		printf("Terminal Init Failed\n");

	fflush(stdout);
}

void resetTerminal(struct termios* defaultTerminal) {
	tcsetattr(STDIN_FILENO, TCSANOW, defaultTerminal);
}


int main (void) {
	int current_x = startX;
	int current_y = startY;
	int timerExpire;
	int botX = 0;
	int botY = 0;
	char command;
	
	fd_set rfds;

	struct termios defaultTerminal, gameTerminal;
	
	initTerminal(&defaultTerminal, &gameTerminal);
	if (setvbuf(stdin, NULL, _IONBF, 0) != 0) printf("Buffer change fail\n");
	
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

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(STDIN_FILENO, &rfds);
		
		timerExpire = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &timeout);

		if (timerExpire == 0) {
			moveEnemy(map, &Character, &Enemy, current_x, current_y);
			drawMap(map);
			continue;
		} else if (timerExpire == -1) {
			system("clear");
			printf("system timer error\n");
			resetTerminal(&defaultTerminal);
			return 1;
		}
		
		command = getchar();

		switch(command) {

			//up move
			case 'w':
				if (map[MOVEUP][current_y].Tile.icon != '*') {

					if (map[MOVEUP][current_y].Tile.icon == 'p') {
						powerUp(&Character);
					}

					if (map[MOVEUP][current_y].Tile.type == 0 &&
						map[MOVEUP][current_y].Droid.icon == 'E') {
						attack(&Character, map, MOVEUP, current_y);
						statusCheck(map, MOVEUP, current_y);
						break;
					}

					if (map[MOVEUP][current_y].Tile.type == 1 &&
						map[MOVEUP][current_y].Tile.icon == 'D') {
						collectDatapad(&Character, &datapads);
					}

					moveDroid(map, &Character, -1, 0, &current_x, &current_y);

					if (doorTransition(&map, &Character, &current_x, &current_y, &enemyLevel)) {
						map[current_x][current_y] = Character;
						initializeEnemy(&Enemy, enemyLevel);
					}
				}
				break;

			//left move
			case  'a':
				if (map[current_x][MOVELEFT].Tile.icon != '*') {

					if (map[current_x][MOVELEFT].Tile.icon == 'p') {
						powerUp(&Character);
					}

					else if (map[current_x][MOVELEFT].Tile.type == 0 &&
						map[current_x][MOVELEFT].Droid.icon == 'E') {
						attack(&Character, map, current_x, MOVELEFT);
						statusCheck(map, current_x, MOVELEFT);
						break;
					}

					else if (map[current_x][MOVELEFT].Tile.type == 1 &&
						map[current_x][MOVELEFT].Tile.icon == 'D') {
						collectDatapad(&Character, &datapads);
					}

					moveDroid(map, &Character, 0, -1, &current_x, &current_y);

					if (doorTransition(&map, &Character, &current_x, &current_y, &enemyLevel)) {
						map[current_x][current_y] = Character;
						initializeEnemy(&Enemy, enemyLevel);
					}

				}
				break;

			//down move
			case 's':
				if (map[MOVEDOWN][current_y].Tile.icon != '*') {

					if (map[MOVEDOWN][current_y].Tile.icon == 'p') {
						powerUp(&Character);
					}
					if (map[MOVEDOWN][current_y].Tile.type == 0 &&
						map[MOVEDOWN][current_y].Droid.icon == 'E') {
						attack(&Character, map, MOVEDOWN, current_y);
						statusCheck(map, MOVEDOWN, current_y);
						break;
					}

					if (map[MOVEDOWN][current_y].Tile.type == 1 &&
						map[MOVEDOWN][current_y].Tile.icon == 'D') {
						collectDatapad(&Character, &datapads);
					}

					moveDroid(map, &Character, 1, 0, &current_x, &current_y);

					if (doorTransition(&map, &Character, &current_x, &current_y, &enemyLevel)) {
						map[current_x][current_y] = Character;
						initializeEnemy(&Enemy, enemyLevel);
					}
				}
				break;

			//right move
			case 'd':
				if (map[current_x][MOVERIGHT].Tile.icon != '*') {

					if (map[current_x][MOVERIGHT].Tile.icon == 'p') {
						powerUp(&Character);
					}

					if (map[current_x][MOVERIGHT].Tile.type == 0 &&
						map[current_x][MOVERIGHT].Droid.icon == 'E') {
						attack(&Character, map, current_x, MOVERIGHT);
						statusCheck(map, current_x, MOVERIGHT);
						break;
					}

					if (map[current_x][MOVERIGHT].Tile.type == 1 &&
						map[current_x][MOVERIGHT].Tile.icon == 'D') {
						collectDatapad(&Character, &datapads);
					}

					moveDroid(map, &Character, 0, 1, &current_x, &current_y);

					if (doorTransition(&map, &Character, &current_x, &current_y, &enemyLevel)) {
						map[current_x][current_y] = Character;
						initializeEnemy(&Enemy, enemyLevel);
					}
				}
				break;

			case 'l':
				unloadMap(map);
				resetTerminal(&defaultTerminal);
				return 0;

			case 'b':
				
				while (1) {
					AIpath(map, current_x, current_y, nextDoor, &botX, &botY);

					if (map[botX][botY].Tile.icon == 'E') {
						attack(&Character, map, botX, botY);	

					} else {
						map[current_x][current_y] = path;
						map[botX][botY] = Character;
						moveEnemy(map, &Character, &Enemy, botX, botY);
						current_x = botX;
						current_y = botY;
						drawMap(map);
					}

					if (doorTransition(&map, &Character, &current_x, &current_y, &enemyLevel)) {
						map[current_x][current_y] = Character;
						initializeEnemy(&Enemy, enemyLevel);
						break;
					}

					sleep(1);
				}
	
				break;

			default:
				continue;

		};
		
		moveEnemy(map, &Character, &Enemy, current_x, current_y);
		drawMap(map);

		if (Character.Droid.powerLevel <= 0) {
			printf("Droid powering down, out of power!\nYou lose!\n");
			unloadMap(map);
			resetTerminal(&defaultTerminal);
			return 0;
		}
		if (Character.Droid.health <= 0) {
			printf("Droid structural integrity down to zero!\nYou lose!\n");
			unloadMap(map);
			resetTerminal(&defaultTerminal);
			return 0;
		}
		
	}
	resetTerminal(&defaultTerminal);
	return 0;
}


