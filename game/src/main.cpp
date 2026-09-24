/*
This project uses the Raylib framework to provide us functionality for math, graphics, GUI, input etc.
See documentation here: https://www.raylib.com/, and examples here: https://www.raylib.com/examples.html
*/

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int screenWidth = 1200;
int screenHeight = 800;

const unsigned int TARGET_FPS = 50;
const float FIXED_DELTA_TIME = 1.0f / (float)TARGET_FPS;

struct PhysicsBody
{
    Vector2 position;
    Vector2 velocity;
};

PhysicsBody bird = { Vector2{-1000, -1000}, Vector2{0, 0} };

Vector2 launchPosition = {100, 700};
float launchSpeed = 100.0f;
float launchAngle = 0.0f;
float launchPosAdjustmentSpeed = 50.0f;


int main()
{
    InitWindow(screenWidth, screenHeight, "Physics-1");
    SetTargetFPS(TARGET_FPS);


    while (!WindowShouldClose())
    {
        BeginDrawing();
			ClearBackground(Color{100,100,150,255});

			// GUI
			DrawRectangle(0, 0, 400, 120, Color{0, 0, 0, 50});
			GuiSlider(Rectangle{ 120, 30, 100, 20 }, "LaunchSpeed", TextFormat("%.2f", launchSpeed), &launchSpeed, 0, 500);
            GuiSlider(Rectangle{ 120, 60, 100, 20 }, "LaunchAngle", TextFormat("%.2f", launchAngle), &launchAngle, 90, -90);

            if (IsKeyDown(KEY_UP)) {
                launchPosition.y -= launchPosAdjustmentSpeed * GetFrameTime();
            }
            if (IsKeyDown(KEY_DOWN)) {
                launchPosition.y += launchPosAdjustmentSpeed * GetFrameTime();
            }

			Vector2 velocityPreview = {cosf(launchAngle * DEG2RAD) * launchSpeed, sinf(launchAngle * DEG2RAD) * launchSpeed};

            DrawCircleV(launchPosition, 10, BROWN);
			DrawLineEx(launchPosition, launchPosition + velocityPreview, 2, RED);
            
			// Spawn Bird
			if (IsKeyPressed(KEY_SPACE))
			{
                bird.position = launchPosition;
				bird.velocity = velocityPreview;
			}
			
            DrawCircleV(bird.position, 30, RED);

            //Vector2 mouseDelta = launchPosition - GetMousePosition();
            DrawLineV(launchPosition, GetMousePosition(), Color{ 0,0,0, 60 });

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
