#pragma once

#include "raygui.h"

void InitRenderer(void);
void RunRenderer(InterpreterContext *ctx);
void MoveCamera(Camera2D *camera, Rectangle *player);