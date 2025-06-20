#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "braintercourse.h"
#include "renderer.h"

int main(int argc, char **argv) {
    char memArray[ARRAY_SIZE] = {0}; // Memory array
    int dataPtr = 0; // Memory pointer (points to the current cell in the memory array)
    int instPtr = 0; // Program counter (points to current instruction in the program)
    char *input; // Input program
    char output[BUFFER_SIZE] = {0}; // Output buffer

    // Get input
    if (argc == 2) {
        input = argv[1];
    } else {
        static char buffer[BUFFER_SIZE];
        printf("Enter program:\n"); // request input
        fgets(buffer, sizeof(buffer), stdin);
        input = buffer;
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

        // Evaluate instruction
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
                    // Process nested loops
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
                    // Process nested loops
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
                // Assign value to output
                int len = strlen(output);
                output[len] = memArray[dataPtr];
                output[len + 1] = '\0';
                break;
            case ',':
                memArray[dataPtr] = getchar();
                break;
        }
        instPtr++;
    }

    // Print output
    printf("%s", output);

    // Initialize and run renderer
    RunRenderer(input, memArray, output);

    return 0;
}
