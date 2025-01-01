#include <raylib.h>
#include <stdlib.h>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

int main(int argc, char* argv[]) {
    InitWindow(WIDTH, HEIGHT, "Untitled Game");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Untitled Game", 0, 0, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return EXIT_SUCCESS;
}
