#ifndef _COLORS_H_
#define _COLORS_H_

#include <stdlib.h>

typedef enum Color
{
  red = 0,
  green = 1,
  blue = 2,
  def
} color;

int randColor ();

void setPrintColor (color c);

#endif
