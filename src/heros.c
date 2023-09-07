#include "../include/heros.h"
#include "../include/units.h"
#include <stdlib.h>
#include <string.h>

hero* newHero(heroType hc, char* n) {
    hero* h = malloc(sizeof(hero));
    h->type = hc;
    strcpy(h->name, n);
    readUnits(h->protoList, hc);
    return h;
}

void freeHero(hero* h) {
    free(h);
    h = NULL;
}