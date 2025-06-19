#include "renderer.h"

#include <stdio.h>

void InitRenderer(void) {
    SetTraceLogLevel(LOG_ERROR); // print only error log
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "braintercourse  -  Brainfuck Interpreter Visualizer");
    SetTargetFPS(SCREEN_FPS);
}

void RunRenderer(char *input, char memArray[], char *output) {
    // Custom colors
    Color grayMatter = (Color){180, 150, 140, 255};
    Color whiteMatter = (Color){220, 200, 190, 255};

    // Get input length
    size_t inputLength = strlen(input) - 1;
    printf("Input length: %d\n", inputLength); // debug

    // Get memory array length
    size_t memLength = strlen(memArray);
    printf("Memory array length: %d\n", memLength); // debug

    // Initialize renderer
    InitRenderer();

    // Main rendering loop
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(whiteMatter);

        int TITLE_FONT_SIZE = 20;
        int SQUARE_SIZE = 75;

        // Input text
        const char *inputText = "Input";
        Vector2 inputTextSize =
                MeasureTextEx(GetFontDefault(), inputText, TITLE_FONT_SIZE, 1);

        DrawText(inputText, SCREEN_WIDTH / 2 - (inputTextSize.x / 2),SCREEN_HEIGHT / 10,
                 TITLE_FONT_SIZE, DARKGRAY);

        // Input array
        for (int i = 0; i < inputLength; i++) {
            DrawRectangle(SCREEN_WIDTH / 4 + SQUARE_SIZE * i, SCREEN_HEIGHT / 6, SQUARE_SIZE, SQUARE_SIZE, grayMatter);
            DrawRectangleLines(SCREEN_WIDTH / 4 + SQUARE_SIZE * i, SCREEN_HEIGHT / 6, SQUARE_SIZE, SQUARE_SIZE, BLACK);
            DrawText(TextFormat("%i", i), SCREEN_WIDTH / 4 + 76 / 2 + 76 * i, SCREEN_HEIGHT / 6 + SQUARE_SIZE + 10, 10,
                     DARKGRAY);
            DrawText(TextFormat("%c", input[i]), SCREEN_WIDTH / 4 + 60 / 2 + 76 * i, SCREEN_HEIGHT / 6 + SQUARE_SIZE - 55, 40,
                     BLACK);
        }

        // Input pointer
        DrawTriangle((Vector2){SCREEN_WIDTH / 4.0f + 40.0f, SCREEN_WIDTH / 4.0f},
                     (Vector2){SCREEN_WIDTH / 4.0f + 20.0f, SCREEN_WIDTH / 4.0f + 30},
                     (Vector2){SCREEN_WIDTH / 4.0f + 60.0f, SCREEN_WIDTH / 4.0f + 30}, BLUE);

        // Memory text
        const char *memText = "Memory array";
        Vector2 memTextSize =
                MeasureTextEx(GetFontDefault(), memText, TITLE_FONT_SIZE, 1);

        DrawText(memText, SCREEN_WIDTH / 2 - (memTextSize.x / 2),SCREEN_HEIGHT / 3 + 60,
                 TITLE_FONT_SIZE, DARKGRAY);

        // Memory array
        for (int i = 0; i < memLength; i++) {
            DrawRectangle(SCREEN_WIDTH / 4 + SQUARE_SIZE * i, SCREEN_HEIGHT / 2, SQUARE_SIZE, SQUARE_SIZE, grayMatter);
            DrawRectangleLines(SCREEN_WIDTH / 4 + SQUARE_SIZE * i, SCREEN_HEIGHT / 2, SQUARE_SIZE, SQUARE_SIZE, BLACK);
            DrawText(TextFormat("%i", i), SCREEN_WIDTH / 4 + 76 / 2 + 76 * i, SCREEN_HEIGHT / 2 + SQUARE_SIZE + 10, 10,
                     DARKGRAY);
            DrawText(TextFormat("%i", memArray[i]), SCREEN_WIDTH / 4 + 20 / 2 + 76 * i, SCREEN_HEIGHT / 2 + SQUARE_SIZE - 55, 40,
                     BLACK);
        }

        // Memory pointer
        DrawTriangle((Vector2){SCREEN_WIDTH / 4.0f + 40.0f, SCREEN_WIDTH / 2.0f},
                     (Vector2){SCREEN_WIDTH / 4.0f + 20.0f, SCREEN_WIDTH / 2.0f + 30},
                     (Vector2){SCREEN_WIDTH / 4.0f + 60.0f, SCREEN_WIDTH / 2.0f + 30}, RED);

        // Output text
        const char *outputText = "Output";
        Vector2 outputTextSize =
                MeasureTextEx(GetFontDefault(), outputText, TITLE_FONT_SIZE, 1);

        DrawText(outputText, SCREEN_WIDTH / 2 - (outputTextSize.x / 2),SCREEN_HEIGHT - 125,
                 TITLE_FONT_SIZE, DARKGRAY);

        // Output
        //const char *output = "Hello World";
        Vector2 outputSize =
                MeasureTextEx(GetFontDefault(), output, 30, 1);

        DrawText(output, SCREEN_WIDTH / 2 - (outputSize.x / 2),SCREEN_HEIGHT - 85,
                 30, BLACK);

        EndDrawing();
    }

    CloseWindow();
}
