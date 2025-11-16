#include "../include/interpreter.h"
#include "../include/renderer.h"
#include "../include/renderer_config.h"

void InitRenderer(void) {
    SetTraceLogLevel(LOG_ERROR); // print only error log
    //SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "braintercourse  -  Brainfuck Interpreter Visualizer");
    SetTargetFPS(SCREEN_FPS);
}

void RunRenderer(InterpreterContext *ctx) {
    // Get input length
    size_t inputLength = strlen(ctx->input);

    // Camera targets
    Rectangle topCameraTarget = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 2};
    Rectangle bottomCameraTarget = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 2};

    // Cameras init
    Camera2D topCamera = {0};
    topCamera.target = (Vector2){topCameraTarget.x, topCameraTarget.y};
    topCamera.offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    topCamera.rotation = 0.0f;
    topCamera.zoom = 1.0f;

    Camera2D bottomCamera = {0};
    bottomCamera.target = (Vector2){bottomCameraTarget.x, bottomCameraTarget.y};
    bottomCamera.offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    bottomCamera.rotation = 0.0f;
    bottomCamera.zoom = 1.0f;

    // Screen render textures
    RenderTexture topCameraScreen = LoadRenderTexture(SCREEN_WIDTH, TILE_SIZE * 8);
    RenderTexture bottomCameraScreen = LoadRenderTexture(SCREEN_WIDTH, TILE_SIZE * 8);

    // Build a flipped rectangle the size of the split view to use for drawing later
    Rectangle splitScreenRect = {
        0.0f, 0.0f,
        (float) topCameraScreen.texture.width,
        (float) -bottomCameraScreen.texture.height
    };

    //==================================================================================
    // MAIN RENDERING LOOP
    //==================================================================================
    while (!WindowShouldClose()) {
        // ----------------------------------------------------------------------------------
        // UPDATE
        // ----------------------------------------------------------------------------------
        MoveCamera1(&topCamera, &topCameraTarget);
        MoveCamera2(&bottomCamera, &bottomCameraTarget);

        // ----------------------------------------------------------------------------------
        // DRAW
        // ----------------------------------------------------------------------------------
        // *** TOP CAMERA ***
        BeginTextureMode(topCameraScreen);
        ClearBackground(BG_COLOR);

        BeginMode2D(topCamera);

        // Input
        //----------------------------------------------------------------------------------
        // Draw input array
        for (int i = 0; i < inputLength; i++) {
            // Draw cells
            DrawRectangle((TILE_SIZE * 8) + SQUARE_SIZE * i, TILE_SIZE * 3, SQUARE_SIZE, SQUARE_SIZE, CELL_COLOR);
            DrawRectangleLines((TILE_SIZE * 8) + SQUARE_SIZE * i, TILE_SIZE * 3, SQUARE_SIZE, SQUARE_SIZE, BG_COLOR);
        }
        EndMode2D();

        // Memory array header
        DrawLine(SQUARE_SIZE, SQUARE_SIZE - TILE_SIZE * 2 + TILE_SIZE / 2,
                 SCREEN_WIDTH - SQUARE_SIZE, SQUARE_SIZE - TILE_SIZE * 2 + TILE_SIZE / 2, BLACK);

        DrawRectangle(SCREEN_WIDTH / 2 - TILE_SIZE * 1.5f,SQUARE_SIZE - TILE_SIZE * 2,
                      TILE_SIZE * 3, TILE_SIZE, BG_COLOR);

        // Draw input text
        const char *inputText = "Input";
        Vector2 inputTextSize =
                MeasureTextEx(GetFontDefault(), inputText, TITLE_FONT_SIZE, 1);

        DrawText(inputText, SCREEN_WIDTH / 2 - (inputTextSize.x / 2), TILE_SIZE, TITLE_FONT_SIZE, DARKGRAY);

        //DrawLayoutGrid();

        EndTextureMode();

        // *** BOTTOM CAMERA ***
        BeginTextureMode(bottomCameraScreen);
        ClearBackground(BG_COLOR);

        BeginMode2D(bottomCamera);

        // Memory Array
        //----------------------------------------------------------------------------------
        // Draw memory array
        for (int i = 0; i < ctx->maxDataPtr + 1; i++) {
            // Draw cells
            DrawRectangle((TILE_SIZE * 8) + SQUARE_SIZE * i, TILE_SIZE * 3,
                          SQUARE_SIZE, SQUARE_SIZE, CELL_COLOR);
            DrawRectangleLines((TILE_SIZE * 8) + SQUARE_SIZE * i, TILE_SIZE * 3,
                               SQUARE_SIZE, SQUARE_SIZE, BG_COLOR);
        }

        EndMode2D();

        DrawLine(SQUARE_SIZE, SQUARE_SIZE - TILE_SIZE * 2 + TILE_SIZE / 2,
                 SCREEN_WIDTH - SQUARE_SIZE, SQUARE_SIZE - TILE_SIZE * 2 + TILE_SIZE / 2, BLACK);

        DrawRectangle(SCREEN_WIDTH / 2 - TILE_SIZE * 3.5f,SQUARE_SIZE - TILE_SIZE * 2,
                      TILE_SIZE * 7, TILE_SIZE, BG_COLOR);

        // Draw input text
        const char *memText = "Memory Array";
        Vector2 memTextSize =
                MeasureTextEx(GetFontDefault(), memText, TITLE_FONT_SIZE, 1);

        DrawText(memText, SCREEN_WIDTH / 2 - (memTextSize.x / 2), TILE_SIZE, TITLE_FONT_SIZE, DARKGRAY);

        //DrawLayoutGrid();

        EndTextureMode();

        // *** NO CAMERA ***
        BeginDrawing();
        ClearBackground(BG_COLOR);

        // Render screen textures
        DrawTextureRec(topCameraScreen.texture, splitScreenRect, (Vector2){0, 0}, WHITE);
        DrawTextureRec(bottomCameraScreen.texture, splitScreenRect, (Vector2){0, TILE_SIZE * 8}, WHITE);

        // Draw Input Array values
        for (int i = 0; i < inputLength; i++) {
            // Apply camera offset transformation
            int worldX = (TILE_SIZE * 8) + SQUARE_SIZE * i;
            int worldY = TILE_SIZE * 3;

            // Transform to screen coordinates
            int screenX = worldX - (int) topCamera.target.x + (int) topCamera.offset.x;
            int screenY = worldY - (int) topCamera.target.y + (int) topCamera.offset.y;

            // Draw indexes
            const char *indexStr = TextFormat("%i", i);
            Vector2 indexSize = MeasureTextEx(GetFontDefault(), indexStr, INDEX_FONT_SIZE, 1);

            int indexWorldY = TILE_SIZE * 7 - (TILE_SIZE / 2);
            int indexScreenY = indexWorldY - (int) topCamera.target.y + (int) topCamera.offset.y;

            DrawText(indexStr,
                     screenX + (SQUARE_SIZE - indexSize.x) / 2,
                     indexScreenY,
                     INDEX_FONT_SIZE, DARKGRAY);

            // Draw values
            const char *inputValueStr = TextFormat("%c", (int) ctx->input[i]);
            Vector2 inputValueSize = MeasureTextEx(GetFontDefault(), inputValueStr, VALUE_FONT_SIZE, 1);
            DrawText(inputValueStr,
                     screenX + (SQUARE_SIZE - inputValueSize.x) / 2,
                     screenY + (SQUARE_SIZE - inputValueSize.y) / 2,
                     VALUE_FONT_SIZE, BLACK);
        }

        // Draw input pointer triangle
        int triangleWorldX = (TILE_SIZE * 8) + 40;
        int triangleWorldY = TILE_SIZE * 7;

        /*
        DrawTriangle((Vector2){triangleWorldX, triangleWorldY},
                     (Vector2){triangleWorldX - 15, triangleWorldY + 25},
                     (Vector2){triangleWorldX + 15, triangleWorldY + 25}, BLUE);
        */

        // Draw Memory Array values
        for (int i = 0; i < ctx->maxDataPtr + 1; i++) {
            // Apply camera offset transformation
            int worldX = (TILE_SIZE * 8) + SQUARE_SIZE * i;
            int worldY = TILE_SIZE * 3;

            // Transform to screen coordinates
            int screenX = worldX - (int) bottomCamera.target.x + (int) bottomCamera.offset.x;
            int screenY = (TILE_SIZE * 8) + worldY - (int) bottomCamera.target.y + (int) bottomCamera.offset.y;

            // Draw indexes
            const char *indexStr = TextFormat("%i", i);
            Vector2 indexSize = MeasureTextEx(GetFontDefault(), indexStr, INDEX_FONT_SIZE, 1);

            int indexWorldY = TILE_SIZE * 7 - (TILE_SIZE / 2);
            int indexScreenY = (TILE_SIZE * 8) + indexWorldY - (int) bottomCamera.target.y + (int) bottomCamera.offset.
                               y;

            DrawText(indexStr,
                     screenX + (SQUARE_SIZE - indexSize.x) / 2,
                     indexScreenY,
                     INDEX_FONT_SIZE, DARKGRAY);

            // Draw values
            const char *memValueStr = TextFormat("%d", (int) ctx->memArray[i]);
            Vector2 memoryValueSize = MeasureTextEx(GetFontDefault(), memValueStr, VALUE_FONT_SIZE, 1);
            DrawText(memValueStr,
                     screenX + (SQUARE_SIZE - memoryValueSize.x) / 2,
                     screenY + (SQUARE_SIZE - memoryValueSize.y) / 2,
                     VALUE_FONT_SIZE, BLACK);
        }

        // Draw memory pointer triangle
        int memTriangleWorldX = (TILE_SIZE * 8) + 40;
        int memTriangleWorldY = TILE_SIZE * 6;

        int memTriangleScreenX = memTriangleWorldX - (int) bottomCamera.target.x + (int) bottomCamera.offset.x;
        int memTriangleScreenY = (TILE_SIZE * 9) + memTriangleWorldY - (int) bottomCamera.target.y + (int) bottomCamera.
                                 offset.y;

        /*
        DrawTriangle((Vector2){memTriangleScreenX, memTriangleScreenY},
                     (Vector2){memTriangleScreenX - 20, memTriangleScreenY + TRIANGLE_SIZE},
                     (Vector2){memTriangleScreenX + 20, memTriangleScreenY + TRIANGLE_SIZE}, RED);
        */

        // Output
        //----------------------------------------------------------------------------------
        // Output header
        DrawLine(SQUARE_SIZE, SCREEN_HEIGHT - TILE_SIZE * 6 - TILE_SIZE / 2,
                 SCREEN_WIDTH - SQUARE_SIZE, SCREEN_HEIGHT - TILE_SIZE * 6 - TILE_SIZE / 2, BLACK);

        // Draw rectangle
        DrawRectangle(SCREEN_WIDTH / 2 - TILE_SIZE * 2,SCREEN_HEIGHT - TILE_SIZE * 7,
                      TILE_SIZE * 4, TILE_SIZE, BG_COLOR);

        const char *outputText = "Output";
        Vector2 outputTextSize =
                MeasureTextEx(GetFontDefault(), outputText, TITLE_FONT_SIZE, 1);

        DrawText(outputText, SCREEN_WIDTH / 2 - (outputTextSize.x / 2),SCREEN_HEIGHT - TILE_SIZE * 7,
                 TITLE_FONT_SIZE, DARKGRAY);

        DrawText(ctx->output, SQUARE_SIZE * 4 + SQUARE_SIZE / 2,SCREEN_HEIGHT - TILE_SIZE * 5,
                 VALUE_FONT_SIZE, BLACK);

        // Transport controls
        // ----------------------------------------------------------------------------------
        DrawLine(SQUARE_SIZE, SCREEN_HEIGHT - TILE_SIZE - TILE_SIZE / 2,
                 SCREEN_WIDTH - SQUARE_SIZE, SCREEN_HEIGHT - TILE_SIZE - TILE_SIZE / 2, BLACK);

        //DrawControlButtons();

        //DrawLayoutGrid();

        EndDrawing();
    }
    CloseWindow();
}

void DrawControlButtons() {
    DrawRectangle(SCREEN_WIDTH / 2 - TILE_SIZE * 6,
                  SCREEN_HEIGHT - TILE_SIZE * 2, TILE_SIZE * 12, TILE_SIZE, BG_COLOR);

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
}

void MoveCamera1(Camera2D *camera, Rectangle *player) {
    // Target movement
    if (IsKeyDown(KEY_A) && player->x > SCREEN_WIDTH / 2) {
        player->x -= 6;
    } else if (IsKeyDown(KEY_D)) {
        player->x += 6;
    }

    // Camera target follows player
    camera->target = (Vector2){player->x, player->y};
}

void MoveCamera2(Camera2D *camera, Rectangle *player) {
    // Target movement
    if (IsKeyDown(KEY_LEFT) && player->x > SCREEN_WIDTH / 2) {
        player->x -= 6;
    } else if (IsKeyDown(KEY_RIGHT)) {
        player->x += 6;
    }

    // Camera target follows player
    camera->target = (Vector2){player->x, player->y};
}

void DrawLayoutGrid() {
    //Draw grid
    for (int i = 0; i < SCREEN_WIDTH / TILE_SIZE + 1; i++) {
        for (int j = 0; j < SCREEN_HEIGHT / TILE_SIZE + 1; j++) {
            DrawRectangleLines(i * TILE_SIZE, j * TILE_SIZE,TILE_SIZE, TILE_SIZE, GREEN);
        }
    }
}
