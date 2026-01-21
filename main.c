#include <raylib.h>
#include <stdio.h>

#define WIN_W 800
#define WIN_H 600

#define MASS_W 67 // SIX SEVEN
#define MASS_H MASS_W
#define K 20
#define MASS_MASS 1 // MASS OF THE MASS 🔥
#define MASS_X (WIN_W / 8.0f) - (MASS_W / 2.0f)
#define DAMP .8

#define nzigs 35
#define ziglen 15
#define HISTORY_SIZE 100

static void drawMass();
static void drawSpring();
static void drawSine();

static float abs(float x) { return x < 0 ? -x : x; }

static const float y0 = 200;
static float y = y0 + 50;

static float positionHistory[HISTORY_SIZE] = {[0 ... HISTORY_SIZE - 1] =
                                                  y0 + (MASS_H / 2.0f)};
static int historyIndex = 0;

int main(void)
{
    InitWindow(WIN_W, WIN_H, "Spring Sim");
    SetTargetFPS(120);

    float dt;
    float f;
    float a;
    float v = 0;
    float dl; // elongation
    int fc = 0;

    while (!WindowShouldClose())
    {
        fc++;
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

        if (fc == 4)
        {
            positionHistory[historyIndex] = y + (MASS_H / 2.0f);
            historyIndex = (historyIndex + 1) % HISTORY_SIZE;
            fc = 0;
        }

        drawMass();
        drawSpring();
        drawSine();

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

static void drawSine(void)
{
    static const float sx = MASS_X + MASS_W;
    static const float w = WIN_W - sx;
    static const float dx = w / (float)(HISTORY_SIZE - 1);

    for (int i = 0; i < HISTORY_SIZE - 1; i++)
    {
        int idx0 = (historyIndex - i - 1 + HISTORY_SIZE) % HISTORY_SIZE;
        int idx1 = (historyIndex - i - 2 + HISTORY_SIZE) % HISTORY_SIZE;

        Vector2 s = {sx + i * dx, positionHistory[idx0]};
        Vector2 e = {sx + (i + 1) * dx, positionHistory[idx1]};

        DrawLineV(s, e, GREEN);
    }
}
