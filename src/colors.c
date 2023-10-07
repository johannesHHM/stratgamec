#include "colors.h"
#include <stdio.h>
#include <stdlib.h>

int
randColor ()
{
  int r = rand () % 3;
  return r;
}
