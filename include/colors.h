#ifndef _COLORS_H_
#define _COLORS_H_

#include <stdlib.h>

#define COLOR_COUNT 3

typedef enum ColorG
{
  red = 0,
  green = 1,
  blue = 2,
  def
} colorG;

int randColor ();

void setPrintColor (colorG c);

#endif
