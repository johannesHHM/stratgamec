#include "../include/colors.h"
#include <stdlib.h>
#include <stdio.h>

int randColor() {
    int r = rand() % 3;
    printf("%d ", r);
    return r;
}