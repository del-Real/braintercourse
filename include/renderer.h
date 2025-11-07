#pragma once

#include "raygui.h"

void InitRenderer(void);
void RunRenderer(InterpreterContext *ctx);
void MoveCamera1(Camera2D *camera, Rectangle *player);
void MoveCamera2(Camera2D *camera, Rectangle *player);
void DrawControlButtons();
void DrawLayoutGrid();