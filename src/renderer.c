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

    Rectangle topCameraTarget = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 2};
    Rectangle bottomCameraTarget = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 2};

    // Camera init
    // Camera2D camera = {tgt.x, tgt.y};
    // //camera.target = (Vector2){0};
    // camera.offset = (Vector2){SCREEN_WIDTH / 2.0f + SCREEN_OFFSET, SCREEN_HEIGHT / 2.0f + SCREEN_OFFSET};
    // camera.rotation = 0.0f;
    // camera.zoom = 1.0f;

    Camera2D topCamera = {0};
    topCamera.target = (Vector2){topCameraTarget.x, topCameraTarget.y};
    topCamera.offset = (Vector2){SCREEN_WIDTH / 2.0f + SCREEN_OFFSET, SCREEN_HEIGHT / 2.0f + SCREEN_OFFSET};
    topCamera.rotation = 0.0f;
    topCamera.zoom = 1.0f;

    Camera2D bottomCamera = {0};
    bottomCamera.target = (Vector2){bottomCameraTarget.x, bottomCameraTarget.y};
    bottomCamera.offset = (Vector2){SCREEN_WIDTH / 2.0f + SCREEN_OFFSET, SCREEN_HEIGHT / 2.0f + SCREEN_OFFSET};
    bottomCamera.rotation = 0.0f;
    bottomCamera.zoom = 1.0f;

    RenderTexture topCameraScreen = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT/2);
    RenderTexture bottomCameraScreen = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT/2);

    // Build a flipped rectangle the size of the split view to use for drawing later
    Rectangle splitScreenRect = {
        0.0f, 0.0f,
        (float) topCameraScreen.texture.width,
        (float) -bottomCameraScreen.texture.height
    };


    //----------------------------------------------------------------------------------
    // MAIN RENDERING LOOP
    //----------------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        // ---------------
        // CAMERA
        // ---------------
        MoveCamera(&topCamera, &topCameraTarget);
        //MoveCamera(&bottomCamera, &bottomCameraTarget);

        // ---------------
        // DRAW
        // ---------------
        BeginTextureMode(topCameraScreen);
        ClearBackground(BG_COLOR);

        BeginMode2D(topCamera);

        DrawRectangleRec(topCameraTarget, BLUE);

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

        EndMode2D();
        EndTextureMode();

        BeginTextureMode(bottomCameraScreen);
        ClearBackground(BG_COLOR);

        BeginMode2D(bottomCamera);

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

        EndMode2D();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BG_COLOR);

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
        DrawTextureRec(topCameraScreen.texture, splitScreenRect, (Vector2){ 0, 0 }, WHITE);
        DrawTextureRec(bottomCameraScreen.texture, splitScreenRect, (Vector2){ SCREEN_HEIGHT/2.0f, 0 }, WHITE);

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
