#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include <raygui.h>

#define WIDTH 1280
#define HEIGHT 720
#define MAX_SHAPES 30
#define SHAPE_SPEED 2.0f

typedef enum {
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE,
} ShapeKind;

typedef struct {
    Vector2 position;
    char* name;
    float scale;
    float size;
    Color color;
    Vector2 velocity;
    bool shouldDraw;
    ShapeKind kind;
} Shape;

Shape* initShapes() {
    Shape* shapes = malloc(MAX_SHAPES * sizeof(Shape));
    if (shapes == NULL) {
        printf("Failed to allocate memory for shapes\n");
        return NULL;
    }

    for (int i = 0; i < MAX_SHAPES; ++i) {
        shapes[i] = (Shape) {
            .position = (Vector2){0,0},
            .name = NULL,
            .scale = 1.0f,
            .size = 0.0f,
            .color = WHITE,
            .velocity = (Vector2){0,0},
            .shouldDraw = false,
            .kind = SHAPE_RECTANGLE
        };
    }

    return shapes;
}

bool addShape(Shape* shapes, Vector2 position, char* name, float size, float scale, Color color, ShapeKind kind) {
    if (shapes == NULL) {
        return false;
    }

    for (int i = 0; i < MAX_SHAPES; ++i) {
        if (!shapes[i].shouldDraw) {
            float angle = (float) GetRandomValue(0,360) * PI / 180.0f;
            Vector2 direction = {
                .x = cosf(angle),
                .y = sinf(angle),
            };

            Vector2 velocity = {
                .x = direction.x * SHAPE_SPEED,
                .y = direction.y * SHAPE_SPEED
            };

            shapes[i] = (Shape) {
                .position = position,
                .name = name,
                .scale = scale,
                .size = size,
                .color = color,
                .velocity = velocity,
                .shouldDraw = true,
                .kind = kind
            };
            return true;
        }
    }

    return false;
}

void removeShape(Shape* shapes, int index) {
    if (shapes == NULL) {
        return;
    }

    if (index >= 0 && index < MAX_SHAPES) {
        shapes[index].shouldDraw = false;
    }
}

void drawShapes(Shape* shapes) {
    for (int i = 0; i < MAX_SHAPES; i++) {
        const Shape s = shapes[i];

        if (s.shouldDraw) {
            switch (s.kind) {
                case SHAPE_RECTANGLE:
                    DrawRectangle(
                        s.position.x, 
                        s.position.y, 
                        s.size * s.scale,  // width
                        s.size * s.scale,  // height
                        s.color
                    );
                    break;
                case SHAPE_CIRCLE:
                    DrawCircle(
                        s.position.x,
                        s.position.y,
                        s.size * s.scale,  // radius
                        s.color
                    );
                    break;
            }
        }
    }
}

void updateShapes(Shape* shapes) { 
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();

    for (int i = 0; i < MAX_SHAPES; i++) {
        Shape* s = &shapes[i];

        if (s->shouldDraw) {
            // Update position
            s->position.x += s->velocity.x;
            s->position.y += s->velocity.y;

            // Check for collision with screen borders
            // Right and left borders
            if (s->position.x > screenW - s->size || s->position.x < 0) {
                s->velocity.x *= -1;  // Invert x direction
                s->position.x = fmaxf(0, fminf(s->position.x, screenW - s->size));
            }

            // Bottom and top borders
            if (s->position.y > screenH - s->size || s->position.y < 0) {
                s->velocity.y *= -1;  // Invert y direction
                s->position.y = fmaxf(0, fminf(s->position.y, screenH - s->size));
            }
        }
    }
}

void drawMenu(bool* showMenu) {
    const int screenW = GetScreenWidth();
    if (GuiButton((Rectangle){ screenW - 150, 20, 120, 30 }, "Toggle Menu")) *showMenu = !(*showMenu);
    if (*showMenu) {
        int result = GuiMessageBox((Rectangle){ screenW - 280, 60, 250, 100 }, "#191#Message Box", "Hi! This is a message!", "Nice;Cool");
        if (result >= 0) *showMenu = false;
    }
}

int main(int argc, [[maybe_unused]] char* argv[argc + 1]) {
    Font customFont = LoadFontEx("resources/fonts/PixAntiqua.ttf", 32, 0, 250);

    SetTextLineSpacing(16);
    InitWindow(WIDTH, HEIGHT, "Untitled Game");
    SetTargetFPS(60);

    Shape* shapes = initShapes();
    if (shapes == NULL) {
        return EXIT_FAILURE;
    }

    addShape(shapes, (Vector2){ 100, 100 }, "Red Circle", 50.0f, 1.0f, RED, SHAPE_CIRCLE);
    addShape(shapes, (Vector2){ 120, 120 }, "Green Rectangle", 50.0f, 1.2f, GREEN, SHAPE_RECTANGLE);

    bool showMenu = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            drawMenu(&showMenu);
            updateShapes(shapes);
            drawShapes(shapes);
        EndDrawing();
    }

    UnloadFont(customFont);
    CloseWindow();
    free(shapes);

    return EXIT_SUCCESS;
}
