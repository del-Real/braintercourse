#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "braintercourse.h"
#include "renderer.h"

int main(int argc, char **argv) {
    char memArray[ARRAY_SIZE] = {0}; // Memory array
    int dataPtr = 0; // Memory pointer (points to the current cell in the memory array)
    int instPtr = 0; // Program counter (points to current instruction in the program)
    char *input; // Input program

    InitRenderer();
    RunRenderLoop();

    // Get input
    if (argc == 1) {
        static char buffer[BUFFER_SIZE];
        printf("Enter program:\n");
        fgets(buffer, sizeof(buffer), stdin);
        input = buffer;
    } else {
        input = argv[1];
    }

    // Main interpreter loop
    while (input[instPtr] != '\0') {
        /*
         * OPERATION RULES:
         * '>' - Move data pointer 1 block to the right
         * '<' - Move data pointer 1 block to the left
         * '+' - Increment value stored in current cell
         * '-' - Decrement value stored in current cell
         * '[' - Jump forward past matching ']' if current cell == 0
         * ']' - Jump back after matching '[' if current cell != 0
         * '.' - Output current cell as ASCII (similar to putchar() in C)
         * ',' - Read input byte into current cell (similar to getchar() in C)
         */
        switch (input[instPtr]) {
            case '>':
                if (dataPtr < ARRAY_SIZE - 1) {
                    dataPtr++;
                } else {
                    fprintf(stderr, "Memory overflow error caused by '>'\n");
                    exit(1);
                }
                break;
            case '<':
                if (dataPtr > 0) {
                    dataPtr--;
                } else {
                    fprintf(stderr, "Memory underflow error caused by '<'\n");
                    exit(1);
                }
                break;
            case '+':
                memArray[dataPtr]++;
                break;
            case '-':
                memArray[dataPtr]--;
                break;
            case '[':
                if (memArray[dataPtr] == 0) {
                    int bracketCount = 1;
                    while (bracketCount > 0) {
                        instPtr++;
                        if (input[instPtr] == '[') {
                            bracketCount++;
                        } else if (input[instPtr] == ']') {
                            bracketCount--;
                        }
                    }
                }
                break;
            case ']':
                if (memArray[dataPtr] != 0) {
                    int bracketCount = 1;
                    while (bracketCount > 0) {
                        instPtr--;
                        if (input[instPtr] == ']') {
                            bracketCount++;
                        } else if (input[instPtr] == '[') {
                            bracketCount--;
                        }
                    }
                }
                break;
            case '.':
                putchar(memArray[dataPtr]);
                fflush(stdout);
                break;
            case ',':
                memArray[dataPtr] = getchar();
                break;
        }
        instPtr++;
    }

    return 0;
}
