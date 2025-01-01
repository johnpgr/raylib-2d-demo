#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 1280
#define HEIGHT 960
#define MAX_SHAPES 30

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
    Vector2 direction;
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
            float angle = (float)GetRandomValue(0,360) * PI / 180.0f;
            Vector2 direction = {
                .x = cosf(angle),
                .y = sinf(angle),
            };

            float speed = 2.0f;
            Vector2 velocity = {
                .x = direction.x * speed,
                .y = direction.y * speed
            };

            shapes[i] = (Shape){
                .position = position,
                .name = name,
                .scale = scale,
                .size = size,
                .color = color,
                .direction = direction,
                .velocity = velocity,
                .shouldDraw = true,
                .kind = kind
            };
            return true;
        }
    }

    return false;
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

int main(int argc, char* argv[]) {
    Font customFont = LoadFontEx("resources/fonts/PixAntiqua.ttf", 32, 0, 250);

    SetTextLineSpacing(16);
    InitWindow(WIDTH, HEIGHT, "Untitled Game");
    SetTargetFPS(165);

    Shape* shapes = initShapes();
    if (shapes == NULL) {
        return EXIT_FAILURE;
    }

    addShape(shapes, (Vector2){ 100, 100 }, "Red Circle", 50.0f, 1.0f, RED, SHAPE_CIRCLE);
    addShape(shapes, (Vector2){ 120, 120 }, "Green Rectangle", 50.0f, 1.2f, GREEN, SHAPE_RECTANGLE);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        updateShapes(shapes);
        drawShapes(shapes);
        EndDrawing();
    }

    UnloadFont(customFont);
    CloseWindow();
    free(shapes);

    return EXIT_SUCCESS;
}
