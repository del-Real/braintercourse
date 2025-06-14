#include <stdio.h>
//#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_FPS 60

#define ARRAY_SIZE 30000

int main(int argc, char **argv) {
    char memArray[ARRAY_SIZE] = {0}; // Memory array
    int dataPtr = 0; // Memory pointer (points to the current cell in the memory array)
    int instPtr = 0; // Program counter (points to current instruction in the program)
    char *input;

    if (argc == 1) {
        static char buffer[1000];  // Or malloc if you prefer
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
                dataPtr++;
                break;
            case '<':
                dataPtr--;
                break;
            case '+':
                memArray[dataPtr]++;
                break;
            case '-':
                memArray[dataPtr]--;
                break;
            case '[':
                if (memArray[dataPtr] == 0) {
                    // skip loop
                    while (input[instPtr] != ']') {
                        instPtr++;
                    }
                }
                break;
            case ']':
                if (memArray[dataPtr] != 0) {
                    // jump back to '['
                    while (input[instPtr] != '[') {
                        instPtr--;
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

    /*
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

        Color grayMatter = (Color){180, 150, 140, 255};
        Color whiteMatter = (Color){220, 200, 190, 255};

        SetTargetFPS(SCREEN_FPS);

        // Main rendering loop
        while (!WindowShouldClose())
        {
            BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawRectangle(300, 150, 150, 150, grayMatter);
            DrawRectangle(450, 150, 150, 150, whiteMatter);

            EndDrawing();
        }

        CloseWindow();
    */

    return 0;
}
