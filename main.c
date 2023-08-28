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

int main() {
	board* b = newBoard();
	unit* u = newUnit(archer, "Archer", 'a', red);
	
	b->board[1][4] = u;
	printBoard(b);
}
