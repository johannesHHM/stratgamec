#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum unitType {
	spearman,
	swordman,
	archer
} unitType;

typedef enum color {
	red,
	green,
	blue
} color;

typedef struct Unit {
	unitType type;
	char name[20];
	char icon;
	color color;
	
	bool taggedWall;
	bool taggedAttck;
} unit;

typedef struct Board {
	int WIDTH;
	int HIGHT;
	unit* board[6][8];
	int backupUnits;
} board;

board* newBoard() {
	board* b = malloc(sizeof(board));
	b->HIGHT = 6;
	b->WIDTH = 8;
	for(int x = 0; x < b->HIGHT; ++x) {
		for(int y = 0; y < b->WIDTH; ++y) {
			b->board[x][y] = NULL;
		}
	}
	b->backupUnits = 30;
	return b;
}

unit* newUnit(unitType t, char n[], char i, color c) {
	unit* u = malloc(sizeof(board));
	u->type = t;
	strcpy(u->name, n);
	u->icon = i;
	u->color = c;
	u->taggedWall = false;
	u->taggedAttck = false;
	return u;
}

void initUnit(unit* u, unitType t, char n[], char i, color c) {
	u->type = t;
	strcpy(u->name, n);
	u->icon = i;
	u->color = c;
	u->taggedWall = false;
	u->taggedAttck = false;
}

void printBoard(board* b) {
	for(int x = 0; x < b->HIGHT; ++x) {
		printf("\n");
		for(int y = 0; y < b->WIDTH; ++y) {
			if(b->board[x][y]) {
				printf(" %c ", b->board[x][y]->icon);
			} else {
				printf(" _ ");
			}
		}
	}
	printf("\n\n");
}

bool sendUnit(board* b, unit* u, int y) {
	//printf("%p %d\n",b->board[b->HIGHT-1][y], y);
	if(b->board[b->HIGHT-1][y] != NULL) {
		return false;
	}
	int x = b->HIGHT-1;
	while(x >= 0 && b->board[x][y] == NULL) {
		//printf("x: %d %p", x, b->board[x][y]);
		x--;
	}
	//printf("\n");
	//TODO: check if transformation will take place
	//printf("%d %d\n", x, y);
	b->board[x+1][y] = u;
	return true;
}

int main() {
	board* b = newBoard();
	unit* u = newUnit(archer, "Archer", 'a', red);
	unit* u3 = newUnit(archer, "Archer", 'a', red);
	unit* u4 = newUnit(archer, "Archer", 'a', red);
	unit* u5 = newUnit(archer, "Archer", 'a', red);
	unit* u6 = newUnit(archer, "Archer", 'a', red);
	unit* u7 = newUnit(archer, "Archer", 'a', red);
	unit* u8 = newUnit(archer, "Archer", 'a', red);

	unit u2;
		
	initUnit(&u2, spearman, "Spear", 's', red);

	bool send = sendUnit(b, u, 2);
	bool sen2 = sendUnit(b, &u2, 2);
	sendUnit(b, u3, 2);
	sendUnit(b, u4, 2);
	sendUnit(b, u5, 2);
	bool w = sendUnit(b, u6, 2);
	bool w1 = sendUnit(b, u7, 2);
	bool w2 = sendUnit(b, u8, 2);

	printf("%d ", send);
	printf("%d ", sen2);
	printf("%d ", w);
	printf("%d ", w1);
	printf("%d ", w2);
	
	printBoard(b);
}
