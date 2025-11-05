#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raylib.h"

// Array params
#define ARRAY_SIZE 30000
#define BUFFER_SIZE 1000

typedef struct {
    char memArray[ARRAY_SIZE];    // Memory array
    char output[BUFFER_SIZE];      // Output buffer
    char *input;                   // Input program
    int instPtr;                   // Program counter (points to current instruction in the program)
    int dataPtr;                  // Memory pointer (points to the current cell in the memory array)
    int maxDataPtr;                // Track highest memory position used
} InterpreterContext;

void ExecuteInterpreter(InterpreterContext *ctx);
