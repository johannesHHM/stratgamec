#include "colors.h"

#include <stdio.h>
#include <stdlib.h>

int
randColor ()
{
  int r = rand () % 3;
  return r;
}

void
setPrintColor (color c)
{
  switch (c)
    {
    case red:
      printf ("\033[1;31m");
      break;
    case green:
      printf ("\033[0;32m");
      break;
    case blue:
      printf ("\033[0;34m");
      break;
    case def:
      printf ("\033[0m");
      break;
    default:
      printf ("\033[0m");
      break;
    }
}
