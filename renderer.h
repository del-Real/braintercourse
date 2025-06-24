#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_FPS 60

void InitRenderer(void);
void RunRenderer(char* input, char memArray[], int memLength, char *output);

#endif