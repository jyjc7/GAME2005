/*
This project uses the Raylib framework to provide us functionality for math, graphics, GUI, input etc.
See documentation here: https://www.raylib.com/, and examples here: https://www.raylib.com/examples.html
*/

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "vector"

int screenWidth = 1200;
int screenHeight = 800;

const unsigned int TARGET_FPS = 50;

enum ShapeType
{
	CIRCLE,
	RECTANGLE,
	TRIANGLE
};

class PhysicsBody
{
public:
    Vector2 position = Vector2{ 0, 0 };
    Vector2 velocity = Vector2{ 0, 0 };
	float mass = 1.0f;
	float drag = 0.1f;
	Color color = RED;
	float radius = 10.0f;
	ShapeType shapeType = CIRCLE;
};

class PhysicsSimulation
{
public:
	std::vector<PhysicsBody> bodies; //container for all physics bodies in the simulation
    const float FIXED_DELTA_TIME = 1.0f / (float)TARGET_FPS;
    Vector2 gravity = { 0, 100 };

	void Update()
	{
		for (PhysicsBody& body : bodies)
		{
			body.position += body.velocity * FIXED_DELTA_TIME;
			body.velocity += gravity * body.mass * FIXED_DELTA_TIME;
			body.velocity *= 1.0f - body.drag * FIXED_DELTA_TIME;
		}
	}
	void Draw()
	{
		for (PhysicsBody& body : bodies)
		{
			switch (body.shapeType)
			{
			case CIRCLE:
				DrawCircleV(body.position, body.radius, body.color);
				break;
			case RECTANGLE:
				DrawRectangleV(body.position, Vector2{ body.radius * 2, body.radius * 2 }, body.color);
				break;
			case TRIANGLE:
				// Draw triangle logic here
				break;
			}
		}
	}
};

//PhysicsBody bird;// = { Vector2{-1000, -1000}, Vector2{0, 0} };
PhysicsSimulation sim;


Vector2 launchPosition = {100, 700};
float launchSpeed = 400.0f;
float launchAngle = 45.0f;
float launchPosAdjustmentSpeed = 100.0f;
float drag = 0.1f;
float mass = 1.0f;

int main()
{
    InitWindow(screenWidth, screenHeight, "GAME2005 - Joshua Chee 101640384");
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose())
    {
        BeginDrawing();
			ClearBackground(Color{100,100,150,255});

			// GUI
			DrawRectangle(0, 0, 400, 200, Color{0, 0, 0, 100});
			GuiSlider(Rectangle{ 120, 30, 100, 20 }, "LaunchSpeed", TextFormat("%.2f", launchSpeed, Color{ 255,255,255,255 }), &launchSpeed, 0, 750);
            GuiSlider(Rectangle{ 120, 60, 100, 20 }, "LaunchAngle", TextFormat("%.2f", launchAngle, Color{ 255,255,255,255 }), &launchAngle, -90, 90);
			GuiSlider(Rectangle{ 120, 90, 100, 20 }, "Gravity", TextFormat("%.2f", sim.gravity.y, Color{ 255,255,255,255 }), &sim.gravity.y, -700, 700);
			GuiSlider(Rectangle{ 120, 120, 100, 20 }, "Drag", TextFormat("%.2f", drag, Color{ 255,255,255,255 }), &drag, 0, 1);
			GuiSlider(Rectangle{ 120, 150, 100, 20 }, "Mass", TextFormat("%.2f", mass, Color{ 255,255,255,255 }), &mass, 0.1f, 10);
			GuiDrawText("Use Arrow Keys to Adjust Launch Position", Rectangle{ 50, 180, 400, 20 }, 0, Color{ 255,255,255,255 });

            if (IsKeyDown(KEY_UP)) {
                launchPosition.y -= launchPosAdjustmentSpeed * GetFrameTime();
            }
            if (IsKeyDown(KEY_DOWN)) {
                launchPosition.y += launchPosAdjustmentSpeed * GetFrameTime();
            }

			Vector2 velocityPreview = {cosf(-1 * launchAngle * DEG2RAD) * launchSpeed, sinf(-1 * launchAngle * DEG2RAD) * launchSpeed};

            DrawCircleV(launchPosition, 10, BROWN);
			DrawLineEx(launchPosition, launchPosition + velocityPreview, 2, RED);
            
			// Spawn Bird
			if (IsKeyPressed(KEY_SPACE))
			{
				PhysicsBody bird;
				bird.position = launchPosition;
				bird.velocity = velocityPreview;
				bird.mass = mass;
				bird.drag = drag;
				bird.color = RED;
				bird.radius = 10.0f;
				bird.shapeType = CIRCLE;
				sim.bodies.push_back(bird);
			}

			sim.Update();
			sim.Draw();

            DrawLineV(launchPosition, GetMousePosition(), Color{ 0,0,0, 60 });

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
