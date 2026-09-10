/*
This project uses the Raylib framework to provide us functionality for math, graphics, GUI, input etc.
See documentation here: https://www.raylib.com/, and examples here: https://www.raylib.com/examples.html
*/

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

const unsigned int TARGET_FPS = 50;
float time = 0;
float frequency = 1/2.0f;
float amplitude = 90;
float dt = 1;

float X, Y;

int main()
{
    InitWindow(1200, 800, "Physics-1");
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(WHITE);
            DrawText("Joshua Chee - 101640384!", 10, 690, 20, LIGHTGRAY);


            Y = Y + (cos(time * frequency)) * frequency * amplitude * dt;
            X = X + (-sin(time * frequency)) * frequency * amplitude * dt;

			DrawCircle(X+600, Y+400, 50, RED);

            time += 1;

            GuiSliderBar(Rectangle{ 60, 5, 1000, 10 }, "Time", TextFormat("%.2f", time), &time, 0, 240);


        EndDrawing();
    }

    CloseWindow();
    return 0;
}
