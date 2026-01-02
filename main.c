#include <raylib.h>
#include <stdio.h>

#define WIN_W 800
#define WIN_H 600

#define MASS_W 67 // SIX SEVEN
#define MASS_H MASS_W
#define K 20
#define MASS_MASS 1 // MASS OF THE MASS 🔥
#define MASS_X (WIN_W / 2.0f) - (MASS_W / 2.0f)
#define DAMP 0.5

#define nzigs 35
#define ziglen 15

static void drawMass();
static void drawSpring();

static float abs(float x) { return x < 0 ? -x : x; }

static const float y0 = 200;
static float y = y0 + 50;

int main(void)
{
    InitWindow(WIN_W, WIN_H, "Spring Sim");

    float dt;
    float f;
    float a;
    float v = 0;
    float dl; // elongation
    while (!WindowShouldClose())
    {
        dt = GetFrameTime();

        BeginDrawing();

        ClearBackground(BLACK);

        DrawFPS(10, 10);
        DrawText("Press mouse to move the mass", 10, 30, 15, LIME);
        DrawText(TextFormat("Elongation: %d", (int)dl), 10, 50, 15, LIME);

        // HOOKES LAG!!!!!
        dl = y - y0; // elongation!!!!
        f = -K * dl - DAMP * v;
        a = f / MASS_MASS;
        v += a * dt;
        if (abs(v) < 0.1f)
            v = 0; // just stawp
        y += v * dt;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            y = GetMouseY();
            v = 0;
        }

        drawMass();
        drawSpring();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

static void drawMass() { DrawRectangle(MASS_X, y, MASS_W, MASS_H, GREEN); }

static void drawSpring()
{
    const float dy = y / nzigs;
    for (int i = 0; i < nzigs; i++)
    {
        float cx = MASS_X + MASS_W / 2.0f;
        Vector2 s = {cx - ziglen / 2.0f, i * dy};
        Vector2 e = {cx + ziglen / 2.0f, i * dy};
        DrawLineV(s, e, GREEN);
    }
}
