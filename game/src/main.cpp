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

enum ColliderShape
{
	CIRCLE,
	HALFPLANE
};

class Collider
{
public:
	virtual ColliderShape GetShape() = 0;
	
};

class PhysicsBody
{
public:
    Vector2 position = Vector2{ 0, 0 };
    Vector2 velocity = Vector2{ 0, 0 };
	float mass = 0.0f;
	float gravityScale = 0.0f;
	float drag = 0.0f;
	Color color = YELLOW;
	float radius = 0.0f;

	virtual void Draw() = 0;
};

class Circle : public Collider, public PhysicsBody
{
public:
	ColliderShape GetShape() override
	{
		return CIRCLE;
	}

	void Draw() override
	{
		DrawCircleV(position, radius, color);
	}
};

class Halfplane : public Collider, public PhysicsBody
{
public:
	Vector2 normal;

	ColliderShape GetShape() override
	{
		return HALFPLANE;
	}

	void Draw() override
	{
		DrawLineV(position, position + normal * 100.0f, MAGENTA); // Draw the normal vector

		Vector2 perp = { -normal.y, normal.x };
		DrawLineV(position - perp * 10000.0f, position + perp * 10000.0f, color);
	}
};



static bool isOverlappingCIRCLECIRCLE(Circle* a, Circle* b)
{
	Vector2 disp = a->position - b->position;
	float distance = Vector2LengthSqr(disp);

	float radA = a->radius;
	float radB = b->radius;

	return distance < (radA + radB) * (radA + radB);
}

// TODO
static bool isOverlappingCIRCLEHALFPLANE(Circle* c, Halfplane* hp)
{
	Vector2 disp = c->position - hp->position;
	float distance = Vector2DotProduct(disp, hp->normal);
	return distance < c->radius;
}

static bool isOverlapping(PhysicsBody* a, PhysicsBody* b)
{
	Vector2 disp = a->position - b->position;
	float distance = Vector2LengthSqr(disp);
	return distance < (a->radius + b->radius) * (a->radius + b->radius);
}

struct Bounds2D
{
	Bounds2D()
	{
		min = { 0, 0 };
		max = { 0, 0 };
	}
	Bounds2D(float top, float bot, float left, float right)
	{
		min = { left, top };
		max = { right, bot };
	}

	Vector2 min;
	Vector2 max;
	
	bool Contains(Vector2 point) {
		return (point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y);
	}
};

class PhysicsSimulation
{
public:
	Bounds2D bounds;
    const float FIXED_DELTA_TIME = 1.0f / (float)TARGET_FPS;
    Vector2 gravity = { 0, 100 };

	void CheckCollisions() {
		for (int i = 0; i < bodies.size(); i++) {
			bodies[i]->color = GREEN;
		}
		for (int i = 0; i < bodies.size(); i++) {
			for (int j = i + 1; j < bodies.size(); j++)
			{
				PhysicsBody* bodyA = bodies[i];
				PhysicsBody* bodyB = bodies[j];
				
				bool isOverlapping = false;

				Circle* aCircle = dynamic_cast<Circle*>(bodyA);
				Circle* bCircle = dynamic_cast<Circle*>(bodyB);
				Halfplane* aHalfplane = dynamic_cast<Halfplane*>(bodyA);
				Halfplane* bHalfplane = dynamic_cast<Halfplane*>(bodyB);

				// Circle circle collision detection
				if (aCircle && bCircle) {
					isOverlapping = isOverlappingCIRCLECIRCLE(aCircle, bCircle);
				}
				else if (aCircle && bHalfplane) {
					isOverlapping = isOverlappingCIRCLEHALFPLANE(aCircle, bHalfplane);
				}
				else if (aHalfplane && bCircle) {
					isOverlapping = isOverlappingCIRCLEHALFPLANE(bCircle, aHalfplane);
				}

				if (isOverlapping) {
					bodyA->color = RED;
					bodyB->color = RED;
				}
			}
		}
	}

	void Update()
	{
		for (PhysicsBody* body : bodies)
		{
			body->position += body->velocity * FIXED_DELTA_TIME;
			body->velocity += gravity * body->gravityScale * body->mass * FIXED_DELTA_TIME;
			body->velocity *= 1.0f - body->drag * FIXED_DELTA_TIME;
		}
		
		// Check for collisions between bodies
		CheckCollisions();
	}
	void Draw()
	{
		for (PhysicsBody* body : bodies)
		{
			body->Draw();
		}
	}

	void AddBody(PhysicsBody* body)
	{
		bodies.push_back(body);
	}

	void ClearOutOfBoundsBodies()
	{
		for (auto it = bodies.begin(); it != bodies.end(); )
		{
			if (!bounds.Contains((*it)->position))
			{
				it = bodies.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

private:
	std::vector<PhysicsBody*> bodies; //container for all physics bodies in the simulation
};

PhysicsSimulation sim;
float margin = 50;

Vector2 launchPosition = {100, 700};
float launchSpeed = 400.0f;
float launchAngle = 45.0f;
float launchPosAdjustmentSpeed = 100.0f;
float drag = 0.1f;
float mass = 1.0f;
float gravityScale = 1.0f;
float spawnRadius = 10.0f;

int main()
{
    InitWindow(screenWidth, screenHeight, "GAME2005 - Joshua Chee 101640384");
    SetTargetFPS(TARGET_FPS);
	
	sim.bounds = Bounds2D((float)(screenHeight + margin), margin, (float)(screenWidth + margin), margin);

	Circle target;
	target.position = { 900, (float)(screenHeight - 100) };
	target.radius = 50.0f;
	target.gravityScale = 0.0f; // Target is static, no gravity
	sim.AddBody(&target);

	Halfplane wall;
	wall.position = { 1000, 400 };
	wall.normal = { -1, 0 };
	sim.AddBody(&wall);

    while (!WindowShouldClose())
    {
        BeginDrawing();
			ClearBackground(Color{100,100,150,255});

			// GUI
			DrawRectangle(0, 0, 400, 200, Color{0, 0, 0, 100});
			GuiSlider(Rectangle{ 120, 30, 100, 20 }, "LaunchSpeed", TextFormat("%.2f", launchSpeed), &launchSpeed, 0, 750);
            GuiSlider(Rectangle{ 120, 60, 100, 20 }, "LaunchAngle", TextFormat("%.2f", launchAngle), &launchAngle, -90, 90);
			GuiSlider(Rectangle{ 120, 90, 100, 20 }, "Gravity", TextFormat("%.2f", sim.gravity.y), &sim.gravity.y, -700, 700);
			GuiSlider(Rectangle{ 120, 120, 100, 20 }, "Drag", TextFormat("%.2f", drag), &drag, 0, 1);
			GuiSlider(Rectangle{ 120, 150, 100, 20 }, "Mass", TextFormat("%.2f", mass), &mass, 0.1f, 10);
			GuiSlider(Rectangle{ 120, 180, 100, 20 }, "Spawn Radius", TextFormat("%.2f", spawnRadius), &spawnRadius, 0, 50);
		
			GuiDrawText("Use Arrow Keys to Adjust Launch Position", Rectangle{ 50, 180, 400, 20 }, 0, Color{ 255,255,255,255 });

            if (IsKeyDown(KEY_UP)) {
                launchPosition.y -= launchPosAdjustmentSpeed * GetFrameTime();
            }
            if (IsKeyDown(KEY_DOWN)) {
                launchPosition.y += launchPosAdjustmentSpeed * GetFrameTime();
            }

			Vector2 velocityPreview = { cosf(-1 * launchAngle * DEG2RAD) * launchSpeed, sinf(-1 * launchAngle * DEG2RAD) * launchSpeed };

            DrawCircleV(launchPosition, 10, BROWN);
			DrawLineEx(launchPosition, launchPosition + (velocityPreview * 0.5f), 2, RED);
            
			// Spawn Bird
			if (IsKeyPressed(KEY_SPACE))
			{
				Circle bird;
				bird.position = launchPosition;
				bird.velocity = velocityPreview;
				bird.mass = mass;
				bird.gravityScale = gravityScale;
				bird.drag = drag;
				bird.color = YELLOW;
				bird.radius = spawnRadius;

				Circle* birdPtr = new Circle(bird);
				sim.AddBody(birdPtr);
			}

			sim.Update();
			sim.Draw();

            DrawLineV(launchPosition, GetMousePosition(), Color{ 0,0,0, 60 });

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
