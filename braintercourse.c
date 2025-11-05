#include "braintercourse.h"

int main(int argc, char **argv) {
    // Initialize context struct values
    InterpreterContext ctx;
    memset(ctx.memArray, 0, sizeof(ctx.memArray));
    memset(ctx.output, 0, sizeof(ctx.output));
    ctx.input = NULL;
    ctx.instPtr = 0;
    ctx.dataPtr = 0;
    ctx.maxDataPtr = 0;

    // Get input program
    if (argc == 2) {
        ctx.input = argv[1];
    } else if (argc > 2) {
        fprintf(stderr, "Usage:\n\t./braintercourse\n\t./braintercourse <brainfuck program>\n");
    } else {
        static char buffer[BUFFER_SIZE];
        printf("Enter program:\n"); // request input
        fgets(buffer, sizeof(buffer), stdin);
        ctx.input = buffer;
    }

    // Execute interpreter algorithm
    ExecuteInterpreter(&ctx);

    // Print output
    printf("%s", ctx.output);

    // Initialize and run (raylib) renderer
    InitRenderer();
    RunRenderer(&ctx);

    return 0;
}

void ExecuteInterpreter(InterpreterContext *ctx) {
    /* ----------------------------------------------------------------------------------
     * MAIN INTERPRETER LOOP
     * ----------------------------------------------------------------------------------
     * Operation rules:
     * '>' - Move data pointer 1 block to the right
     * '<' - Move data pointer 1 block to the left
     * '+' - Increment value stored in current cell
     * '-' - Decrement value stored in current cell
     * '[' - Jump forward past matching ']' if current cell == 0
     * ']' - Jump back after matching '[' if current cell != 0
     * '.' - Output current cell as ASCII (similar to putchar() in C)
     * ',' - Read input byte into current cell (similar to getchar() in C)
     */

    while (ctx->input[ctx->instPtr] != '\0') {
        // Evaluate instruction
        switch (ctx->input[ctx->instPtr]) {
            case '>':
                if (ctx->dataPtr < ARRAY_SIZE - 1) {
                    ctx->dataPtr++;
                    if (ctx->dataPtr > ctx->maxDataPtr) {
                        ctx->maxDataPtr = ctx->dataPtr; // Track highest position
                    }
                } else {
                    fprintf(stderr, "Memory overflow error caused by '>'\n");
                    exit(1);
                }
                break;
            case '<':
                if (ctx->dataPtr > 0) {
                    ctx->dataPtr--;
                } else {
                    fprintf(stderr, "Memory underflow error caused by '<'\n");
                    exit(1);
                }
                break;
            case '+':
                ctx->memArray[ctx->dataPtr]++;
                break;
            case '-':
                ctx->memArray[ctx->dataPtr]--;
                break;
            case '[':
                if (ctx->memArray[ctx->dataPtr] == 0) {
                    // Process nested loops
                    int bracketCount = 1;
                    while (bracketCount > 0) {
                        ctx->instPtr++;
                        if (ctx->input[ctx->instPtr] == '[') {
                            bracketCount++;
                        } else if (ctx->input[ctx->instPtr] == ']') {
                            bracketCount--;
                        }
                    }
                }
                break;
            case ']':
                if (ctx->memArray[ctx->dataPtr] != 0) {
                    // Process nested loops
                    int bracketCount = 1;
                    while (bracketCount > 0) {
                        ctx->instPtr--;
                        if (ctx->input[ctx->instPtr] == ']') {
                            bracketCount++;
                        } else if (ctx->input[ctx->instPtr] == '[') {
                            bracketCount--;
                        }
                    }
                }
                break;
            case '.': {
                // Assign value to output
                int len;
                len = strlen(ctx->output);
                ctx->output[len] = ctx->memArray[ctx->dataPtr];
                ctx->output[len + 1] = '\0';
                break;
            }
            case ',':
                int ch = getchar();
                if (ch != EOF) {
                    ctx->memArray[ctx->dataPtr] = ch;
                }
                break;
        }

        // Next instruction
        ctx->instPtr++;
    }
}

void InitRenderer(void) {
    SetTraceLogLevel(LOG_ERROR); // print only error log
    //SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "braintercourse  -  Brainfuck Interpreter Visualizer");
    SetTargetFPS(SCREEN_FPS);
}

void RunRenderer(InterpreterContext *ctx) {
    // Get input length
    size_t inputLength = strlen(ctx->input);

    Rectangle tgt = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 2};

    // Camera init
    Camera2D camera = {tgt.x, tgt.y};
    //camera.target = (Vector2){0};
    camera.offset = (Vector2){SCREEN_WIDTH / 2.0f + SCREEN_OFFSET, SCREEN_HEIGHT / 2.0f + SCREEN_OFFSET};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    //----------------------------------------------------------------------------------
    // MAIN RENDERING LOOP
    //----------------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        // ---------------
        // CAMERA
        // ---------------
        MoveCamera(&camera, &tgt);

        // ---------------
        // DRAW
        // ---------------
        BeginDrawing();
        ClearBackground(BG_COLOR);
        BeginMode2D(camera);

        DrawRectangleRec(tgt, BG_COLOR);

        // Draw grid
        // for (int i = 0; i < SCREEN_WIDTH / TILE_SIZE + 1; i++) {
        //     for (int j = 0; j < SCREEN_HEIGHT / TILE_SIZE + 1; j++) {
        //         DrawRectangleLines(i * TILE_SIZE, j * TILE_SIZE,TILE_SIZE, TILE_SIZE, LIGHTGRAY);
        //     }
        // }

        //----------------------------------------------------------------------------------
        // Input
        //----------------------------------------------------------------------------------

        // Draw input array
        for (int i = 0; i < inputLength; i++) {
            // Draw cells
            DrawRectangle(SCREEN_WIDTH / 4 + SQUARE_SIZE * i, TILE_SIZE * 3, SQUARE_SIZE, SQUARE_SIZE, CELL_COLOR);
            DrawRectangleLines(SCREEN_WIDTH / 4 + SQUARE_SIZE * i, TILE_SIZE * 3, SQUARE_SIZE, SQUARE_SIZE, BG_COLOR);

            // Draw indexes
            DrawText(TextFormat("%i", i), SCREEN_WIDTH / 4 + SQUARE_SIZE * i + (SQUARE_SIZE / 2),
                     TILE_SIZE * 7 - TILE_SIZE / 2, INDEX_FONT_SIZE, DARKGRAY);

            // Get input array values
            const char *inputValueStr = TextFormat("%c", (int) ctx->input[i]);
            Vector2 inputValueSize = MeasureTextEx(GetFontDefault(), inputValueStr, VALUE_FONT_SIZE, 1);

            // Center value in each cell
            int cellCenterX = SCREEN_WIDTH / 4 + SQUARE_SIZE * i + (SQUARE_SIZE / 2);
            DrawText(inputValueStr, cellCenterX - (inputValueSize.x / 2), TILE_SIZE * 4, VALUE_FONT_SIZE, BLACK);
        }

        // Draw input pointer
        DrawTriangle((Vector2){SCREEN_WIDTH / 4.0f + 40.0f, TILE_SIZE * 7},
                     (Vector2){SCREEN_WIDTH / 4.0f + 20.0f, TILE_SIZE * 7 + TRIANGLE_SIZE},
                     (Vector2){SCREEN_WIDTH / 4.0f + 60.0f, TILE_SIZE * 7 + TRIANGLE_SIZE}, BLUE);

        //----------------------------------------------------------------------------------
        // Memory Array
        //----------------------------------------------------------------------------------

        // Draw memory array
        for (int i = 0; i < ctx->maxDataPtr + 1; i++) {
            // Draw cells
            DrawRectangle(SCREEN_WIDTH / 4 + SQUARE_SIZE * i, SCREEN_HEIGHT / 2 - TILE_SIZE, SQUARE_SIZE, SQUARE_SIZE,
                          CELL_COLOR);
            DrawRectangleLines(SCREEN_WIDTH / 4 + SQUARE_SIZE * i, SCREEN_HEIGHT / 2 - TILE_SIZE, SQUARE_SIZE,
                               SQUARE_SIZE,
                               BG_COLOR);

            // Draw indexes
            DrawText(TextFormat("%i", i), SCREEN_WIDTH / 4 + SQUARE_SIZE * i + (SQUARE_SIZE / 2),
                     SCREEN_HEIGHT / 2 + SQUARE_SIZE - TILE_SIZE / 2, INDEX_FONT_SIZE,
                     DARKGRAY);

            // Get memory array values
            const char *memValueStr = TextFormat("%d", (int) ctx->memArray[i]);
            Vector2 memoryValueSize = MeasureTextEx(GetFontDefault(), memValueStr, VALUE_FONT_SIZE, 1);

            // Center value in each cell
            int cellCenterX = SCREEN_WIDTH / 4 + SQUARE_SIZE * i + (SQUARE_SIZE / 2);
            DrawText(memValueStr, cellCenterX - (memoryValueSize.x / 2), SCREEN_HEIGHT / 2, VALUE_FONT_SIZE, BLACK);
        }

        // Draw memory pointer
        DrawTriangle((Vector2){SCREEN_WIDTH / 4.0f + 40.0f, SCREEN_HEIGHT / 2 + TILE_SIZE * 3},
                     (Vector2){SCREEN_WIDTH / 4.0f + 20.0f, SCREEN_HEIGHT / 2 + TILE_SIZE * 3 + TRIANGLE_SIZE},
                     (Vector2){SCREEN_WIDTH / 4.0f + 60.0f, SCREEN_HEIGHT / 2 + TILE_SIZE * 3 + TRIANGLE_SIZE}, RED);

        EndMode2D();

        DrawText(ctx->output, SQUARE_SIZE * 4 + SQUARE_SIZE / 2,SCREEN_HEIGHT - SQUARE_SIZE * 2 + SQUARE_SIZE / 2,
                 VALUE_FONT_SIZE, BLACK);

        DrawLine(SQUARE_SIZE, SQUARE_SIZE - TILE_SIZE * 2 + TILE_SIZE / 2,
                 SCREEN_WIDTH - SQUARE_SIZE, SQUARE_SIZE - TILE_SIZE * 2 + TILE_SIZE / 2, BLACK);

        DrawRectangle(SCREEN_WIDTH / 2 - 40,SQUARE_SIZE - TILE_SIZE * 2, 80, 30, BG_COLOR);


        DrawLine(SQUARE_SIZE, SQUARE_SIZE * 4 - TILE_SIZE * 3 + TILE_SIZE / 2,
                 SCREEN_WIDTH - SQUARE_SIZE, SQUARE_SIZE * 4 - TILE_SIZE * 3 + TILE_SIZE / 2, BLACK);

        DrawRectangle(SCREEN_WIDTH / 2 - 90,SCREEN_HEIGHT / 2 - TILE_SIZE * 3, 180, 30, BG_COLOR);

        // Draw input text
        const char *inputText = "Input";
        Vector2 inputTextSize =
                MeasureTextEx(GetFontDefault(), inputText, TITLE_FONT_SIZE, 1);

        DrawText(inputText, SCREEN_WIDTH / 2 - (inputTextSize.x / 2), TILE_SIZE, TITLE_FONT_SIZE, DARKGRAY);


        // Draw memory text
        const char *memText = "Memory array";
        Vector2 memTextSize =
                MeasureTextEx(GetFontDefault(), memText, TITLE_FONT_SIZE, 1);

        DrawText(memText, SCREEN_WIDTH / 2 - (memTextSize.x / 2),SQUARE_SIZE * 3,
                 TITLE_FONT_SIZE, DARKGRAY);


        //----------------------------------------------------------------------------------
        // Output
        //----------------------------------------------------------------------------------
        DrawLine(SQUARE_SIZE, SCREEN_HEIGHT - TILE_SIZE * 6 - TILE_SIZE / 2,
                 SCREEN_WIDTH - SQUARE_SIZE, SCREEN_HEIGHT - TILE_SIZE * 6 - TILE_SIZE / 2, BLACK);

        // Draw rectangle
        DrawRectangle(SCREEN_WIDTH / 2 - 50,SCREEN_HEIGHT - TILE_SIZE * 7, 100, 30, BG_COLOR);

        const char *outputText = "Output";
        Vector2 outputTextSize =
                MeasureTextEx(GetFontDefault(), outputText, TITLE_FONT_SIZE, 1);

        // Transport controls
        // ------------------
        DrawLine(SQUARE_SIZE, SCREEN_HEIGHT - TILE_SIZE - TILE_SIZE / 2,
                 SCREEN_WIDTH - SQUARE_SIZE, SCREEN_HEIGHT - TILE_SIZE - TILE_SIZE / 2, BLACK);

        DrawRectangle(SCREEN_WIDTH / 2 - 150,
                      SCREEN_HEIGHT - TILE_SIZE * 2, 300, 30, BG_COLOR);

        DrawText(outputText, SCREEN_WIDTH / 2 - (outputTextSize.x / 2),SCREEN_HEIGHT - TILE_SIZE * 7,
                 TITLE_FONT_SIZE, DARKGRAY);

        // Normal button
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(LIGHTGRAY));
        GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(GRAY));
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

        // Hover button
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(BG_COLOR));
        GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, ColorToInt(DARKGRAY));
        GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));

        // Press button
        GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(CELL_COLOR));
        GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, ColorToInt(DARKGRAY));
        GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(BLACK));

        // Start button
        if (GuiButton((Rectangle){
                          SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2 - BUTTON_WIDTH * 3 - BUTTON_OFFSET * 3,
                          SCREEN_HEIGHT - TILE_SIZE * 2,
                          BUTTON_WIDTH, BUTTON_HEIGHT
                      }, "#129#")) {
            // button clicked
        }

        // Rewind button
        if (GuiButton((Rectangle){
                          SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2 - BUTTON_WIDTH * 2 - BUTTON_OFFSET * 2,
                          SCREEN_HEIGHT - TILE_SIZE * 2,
                          BUTTON_WIDTH, BUTTON_HEIGHT
                      },
                      "#130#")) {
            // button clicked
        }

        // Pause button
        if (GuiButton((Rectangle){
                          SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2 - BUTTON_WIDTH - BUTTON_OFFSET,
                          SCREEN_HEIGHT - TILE_SIZE * 2,
                          BUTTON_WIDTH,
                          BUTTON_HEIGHT
                      },
                      "#132#")) {
            // button clicked
        }

        // Play button
        if (GuiButton((Rectangle){
                          SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2,
                          SCREEN_HEIGHT - TILE_SIZE * 2,
                          BUTTON_WIDTH, BUTTON_HEIGHT
                      },
                      "#131#")) {
            // button clicked
        }


        // Stop button
        if (GuiButton((Rectangle){
                          SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2 + BUTTON_WIDTH + BUTTON_OFFSET,
                          SCREEN_HEIGHT - TILE_SIZE * 2,
                          BUTTON_WIDTH, BUTTON_HEIGHT
                      },
                      "#133#")) {
            // button clicked
        }

        // End button
        if (GuiButton((Rectangle){
                          SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2 + BUTTON_WIDTH * 2 + BUTTON_OFFSET * 2,
                          SCREEN_HEIGHT - TILE_SIZE * 2,
                          BUTTON_WIDTH, BUTTON_HEIGHT
                      },
                      "#134#")) {
            // button clicked
        }

        GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(RED));
        GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, ColorToInt(BLACK));
        GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(RAYWHITE));

        // Record button
        if (GuiButton((Rectangle){
                          SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2 + BUTTON_WIDTH * 3 + BUTTON_OFFSET * 3,
                          SCREEN_HEIGHT - TILE_SIZE * 2,
                          BUTTON_WIDTH, BUTTON_HEIGHT
                      },
                      "#135#")) {
            // button clicked
        }
        EndDrawing();
    }
    CloseWindow();
}

// Debug fn
void MoveCamera(Camera2D *camera, Rectangle *player) {
    // Target movement
    if (IsKeyDown(KEY_A) && player->x > SCREEN_WIDTH / 2 + SCREEN_OFFSET / 2) {
        player->x -= 6;
    } else if (IsKeyDown(KEY_D)) {
        player->x += 6;
    }

    // Camera target follows player
    camera->target = (Vector2){player->x + 20, player->y + 20};
}
