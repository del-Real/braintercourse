#include "renderer.h"

void InitRenderer(void)
{
    SetTraceLogLevel(LOG_ERROR); // print only error log
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "braintercourse");
    SetTargetFPS(SCREEN_FPS);
}

void RunRenderLoop(void)
{
    Color grayMatter = (Color){180, 150, 140, 255};
    Color whiteMatter = (Color){220, 200, 190, 255};

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
}