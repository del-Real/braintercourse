#include "renderer.h"

void InitRenderer(void) {
    SetTraceLogLevel(LOG_ERROR); // print only error log
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "braintercourse  -  Brainfuck Interpreter Visualizer");
    SetTargetFPS(SCREEN_FPS);
}

void RunRenderer(char *input, char memArray[], int memLength, char *output) {
    // Constant size values
    int TITLE_FONT_SIZE = 20;
    int INDEX_FONT_SIZE = 10;
    int VALUE_FONT_SIZE = 30;
    int SQUARE_SIZE = 75;

    // Custom colors
    Color grayMatter = (Color){180, 150, 140, 255};
    Color whiteMatter = (Color){220, 200, 190, 255};

    // Get input length
    size_t inputLength = strlen(input);

    InitRenderer(); // Initialize renderer

    //----------------------------------------------------------------------------------
    // MAIN RENDERING LOOP
    //----------------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(whiteMatter);

        //----------------------------------------------------------------------------------
        // Input
        //----------------------------------------------------------------------------------
        // Draw input text
        const char *inputText = "Input";
        Vector2 inputTextSize =
                MeasureTextEx(GetFontDefault(), inputText, TITLE_FONT_SIZE, 1);

        DrawText(inputText, SCREEN_WIDTH / 2 - (inputTextSize.x / 2),SCREEN_HEIGHT / 10,
                 TITLE_FONT_SIZE, DARKGRAY);

        // Draw input Array
        for (int i = 0; i < inputLength; i++) {
            // Draw cells
            DrawRectangle(SCREEN_WIDTH / 4 + SQUARE_SIZE * i, SCREEN_HEIGHT / 6, SQUARE_SIZE, SQUARE_SIZE, grayMatter);
            DrawRectangleLines(SCREEN_WIDTH / 4 + SQUARE_SIZE * i, SCREEN_HEIGHT / 6, SQUARE_SIZE, SQUARE_SIZE, BLACK);

            // Draw indexes
            DrawText(TextFormat("%i", i), SCREEN_WIDTH / 4 + (SQUARE_SIZE + 1) / 2 + (SQUARE_SIZE + 1) * i,
                     SCREEN_HEIGHT / 6 + SQUARE_SIZE + 10, INDEX_FONT_SIZE,
                     DARKGRAY);

            // Draw input values
            DrawText(TextFormat("%c", input[i]), SCREEN_WIDTH / 4 + 65 / 2 + (SQUARE_SIZE + 1) * i,
                     SCREEN_HEIGHT / 6 + SQUARE_SIZE - 50, VALUE_FONT_SIZE,
                     BLACK);
        }

        // Draw input pointer
        DrawTriangle((Vector2){SCREEN_WIDTH / 4.0f + 40.0f, SCREEN_WIDTH / 4.0f},
                     (Vector2){SCREEN_WIDTH / 4.0f + 20.0f, SCREEN_WIDTH / 4.0f + 30},
                     (Vector2){SCREEN_WIDTH / 4.0f + 60.0f, SCREEN_WIDTH / 4.0f + 30}, BLUE);

        //----------------------------------------------------------------------------------
        // Memory Array
        //----------------------------------------------------------------------------------
        // Draw memory text
        const char *memText = "Memory array";
        Vector2 memTextSize =
                MeasureTextEx(GetFontDefault(), memText, TITLE_FONT_SIZE, 1);

        DrawText(memText, SCREEN_WIDTH / 2 - (memTextSize.x / 2),SCREEN_HEIGHT / 3 + 60,
                 TITLE_FONT_SIZE, DARKGRAY);

        // Draw memory array
        for (int i = 0; i < memLength; i++) {
            // Draw cells
            DrawRectangle(SCREEN_WIDTH / 4 + SQUARE_SIZE * i, SCREEN_HEIGHT / 2, SQUARE_SIZE, SQUARE_SIZE, grayMatter);
            DrawRectangleLines(SCREEN_WIDTH / 4 + SQUARE_SIZE * i, SCREEN_HEIGHT / 2, SQUARE_SIZE, SQUARE_SIZE, BLACK);

            // Draw indexes
            DrawText(TextFormat("%i", i), SCREEN_WIDTH / 4 + (SQUARE_SIZE + 1) / 2 + 76 * i,
                     SCREEN_HEIGHT / 2 + SQUARE_SIZE + 10, INDEX_FONT_SIZE,
                     DARKGRAY);

            // Get memory array values
            const char *memValueStr = TextFormat("%d", (int) memArray[i]);
            Vector2 memoryValueSize = MeasureTextEx(GetFontDefault(), memValueStr, VALUE_FONT_SIZE, 1);

            // Center value in each cell
            int cellCenterX = SCREEN_WIDTH / 4 + SQUARE_SIZE * i + (SQUARE_SIZE / 2);
            DrawText(memValueStr, cellCenterX - (memoryValueSize.x / 2), SCREEN_HEIGHT / 2 + SQUARE_SIZE - 50,
                     VALUE_FONT_SIZE,
                     BLACK);
        }

        // Draw memory pointer
        DrawTriangle((Vector2){SCREEN_WIDTH / 4.0f + 40.0f, SCREEN_WIDTH / 2.0f},
                     (Vector2){SCREEN_WIDTH / 4.0f + 20.0f, SCREEN_WIDTH / 2.0f + 30},
                     (Vector2){SCREEN_WIDTH / 4.0f + 60.0f, SCREEN_WIDTH / 2.0f + 30}, RED);

        //----------------------------------------------------------------------------------
        // Output
        //----------------------------------------------------------------------------------
        // Draw output text
        const char *outputText = "Output";
        Vector2 outputTextSize =
                MeasureTextEx(GetFontDefault(), outputText, TITLE_FONT_SIZE, 1);

        DrawText(outputText, SCREEN_WIDTH / 2 - (outputTextSize.x / 2),SCREEN_HEIGHT - 125,
                 TITLE_FONT_SIZE, DARKGRAY);

        // Draw output value
        Vector2 outputValueSize =
                MeasureTextEx(GetFontDefault(), output, VALUE_FONT_SIZE, 1);

        DrawText(output, SCREEN_WIDTH / 2 - (outputValueSize.x / 2),SCREEN_HEIGHT - 85,
                 VALUE_FONT_SIZE, BLACK);

        EndDrawing();
    }

    CloseWindow();
}
