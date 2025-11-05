#pragma once

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "config.h"

typedef struct {
    char memArray[ARRAY_SIZE];    // Memory array
    char output[BUFFER_SIZE];      // Output buffer
    char *input;                   // Input program
    int instPtr;                   // Program counter (points to current instruction in the program)
    int dataPtr;                  // Memory pointer (points to the current cell in the memory array)
    int maxDataPtr;                // Track highest memory position used
} InterpreterContext;

void InitRenderer(void);
void ExecuteInterpreter(InterpreterContext *ctx);
void RunRenderer(InterpreterContext *ctx);
void MoveCamera(Camera2D *camera, Rectangle *player);