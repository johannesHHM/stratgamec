#include "../include/units.h"
#include "../include/heros.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Board {
    int WIDTH;
    int HIGHT;
    unit* board[6][8];
    int backupUnits;
} board;

bool checkTagsNeeded(board* b);

board* newBoard()
{
    board* b = malloc(sizeof(board));
    b->HIGHT = 6;
    b->WIDTH = 8;
    for (int x = 0; x < b->HIGHT; ++x) {
        for (int y = 0; y < b->WIDTH; ++y) {
            b->board[x][y] = NULL;
        }
    }
    b->backupUnits = 30;
    return b;
}

bool cmpUnits(unit* u1, unit* u2)
{
    if (!u1 || !u2)
        return false;
    return ((u1->type == u2->type) && (strcmp(u1->name, u2->name) == 0) && (u1->color == u2->color));
}

void printBoard(board* b)
{
    printf("              Board\n");
    for (int x = 0; x < b->HIGHT; ++x) {
        printf("\n");
        for (int y = 0; y < b->WIDTH; ++y) {
            if (b->board[x][y]) {
                printf(" %c%d ", b->board[x][y]->icon, b->board[x][y]->color);
            } else {
                printf(" __ ");
            }
        }
    }
    printf("\n\n");
}

bool sendUnit(board* b, unit* u, int y)
{
    if (b->board[b->HIGHT - 1][y] != NULL) {
        return false;
    }
    int x = b->HIGHT - 1;
    while (x >= 0 && b->board[x][y] == NULL) {
        x--;
    }
    b->board[x + 1][y] = u;
    if (checkTagsNeeded(b)) {
        freeUnit(u);
        b->board[x + 1][y] = NULL;
        return false;
    }
    return true;
}

bool sendBackupUnits(board* b, int amount, hero* h)
{
    if (b->backupUnits < amount)
        return false;
    for (int i = 0; i < amount; ++i) {
        bool response = false;
        while (!response) {
            unitPrototype* up = &h->protoList[rand() % 3];
            unit* u = newUnitFromProto(up, randColor());
            int random = rand() % b->WIDTH;
            response = sendUnit(b, u, random);
            if (response)
                b->backupUnits--;
        }
    }
    return true;
}

bool removeUnit(board* b, int x, int y)
{
    if (!b->board[x][y])
        return false;
    freeUnit(b->board[x][y]);
    b->board[x][y] = NULL;
    return true;
}

void tagWalls(board* b)
{
    for (int x = 0; x < b->HIGHT; ++x) {
        for (int y = 0; y < b->WIDTH - 2; ++y) {
            if (!b->board[x][y])
                continue;
            if (cmpUnits(b->board[x][y], b->board[x][y + 1]) && cmpUnits(b->board[x][y], b->board[x][y + 2])) {
                b->board[x][y]->taggedWall = true;
                b->board[x][y]->icon = 'W';

                b->board[x][y + 1]->taggedWall = true;
                b->board[x][y + 1]->icon = 'W';

                b->board[x][y + 2]->taggedWall = true;
                b->board[x][y + 2]->icon = 'W';
            }
        }
    }
}

void tagAttacks3x1(board* b)
{
    for (int y = 0; y < b->WIDTH; ++y) {
        for (int x = 0; x < b->HIGHT - 2; ++x) {
            if (!b->board[x][y])
                continue;
            if (
                cmpUnits(b->board[x][y], b->board[x + 1][y]) && cmpUnits(b->board[x][y], b->board[x + 2][y]) && !b->board[x][y]->taggedAttck && !b->board[x + 1][y]->taggedAttck && !b->board[x + 2][y]->taggedAttck) {
                b->board[x][y]->taggedAttck = true;
                b->board[x][y]->icon = 'X';

                b->board[x + 1][y]->taggedAttck = true;
                b->board[x + 1][y]->icon = 'X';

                b->board[x + 2][y]->taggedAttck = true;
                b->board[x + 2][y]->icon = 'X';
            }
        }
    }
}

bool checkWall(board* b)
{
    for (int x = 0; x < b->HIGHT; ++x) {
        for (int y = 0; y < b->WIDTH - 2; ++y) {
            if (!b->board[x][y])
                continue;
            if (cmpUnits(b->board[x][y], b->board[x][y + 1]) && cmpUnits(b->board[x][y], b->board[x][y + 2]))
                return true;
        }
    }
    return false;
}

bool checkAttack3x1(board* b)
{
    for (int y = 0; y < b->WIDTH; ++y) {
        for (int x = 0; x < b->HIGHT - 2; ++x) {
            if (!b->board[x][y])
                continue;
            if (
                cmpUnits(b->board[x][y], b->board[x + 1][y]) && cmpUnits(b->board[x][y], b->board[x + 2][y]) && !b->board[x][y]->taggedAttck && !b->board[x + 1][y]->taggedAttck && !b->board[x + 2][y]->taggedAttck)
                return true;
        }
    }
    return false;
}

bool checkTagsNeeded(board* b)
{
    return checkWall(b) || checkAttack3x1(b);
}

int main()
{
    printf("Running Main\n\n");
    srand(time(NULL));
    board* b = newBoard();

    hero* paladinHero = newHero(paladin, "Uther");

    //unit* u1 = newUnitFromProto(&paladinHero->protoList[0], 0);
    //unit* u2 = newUnitFromProto(&paladinHero->protoList[0], 0);
    //unit* u3 = newUnitFromProto(&paladinHero->protoList[0], 0);

    //sendUnit(b, u1, 0);
    //sendUnit(b, u2, 0);
    //sendUnit(b, u3, 0);

    sendBackupUnits(b, 30, paladinHero);

    tagWalls(b);
    tagAttacks3x1(b);

    printBoard(b);

    tagWalls(b);
    tagAttacks3x1(b);

    printBoard(b);

    freeHero(paladinHero);
}
