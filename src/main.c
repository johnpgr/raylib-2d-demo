#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include <raygui.h>

#define WIDTH 1280
#define HEIGHT 720
#define SHAPE_SPEED 2.0f

#define append(arr, item)\
    do {\
        if (arr.size >= arr.capacity) {\
            if(arr.capacity == 0) arr.capacity = 100;\
            else arr.capacity *= 2;\
            arr.items = realloc(arr.items, arr.capacity * sizeof(*arr.items));\
        }\
        arr.items[arr.size++] = item;\
    } while (0)

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
    bool should_draw;
    ShapeKind kind;
} Shape;

typedef struct {
    Shape* items;
    size_t size;
    size_t capacity;
} Shapes;

void print_shapes(Shapes* shapes) {
    for (size_t i = 0; i < shapes->size; i++) {
        const Shape s = shapes->items[i];
        printf("Shape %s: x=%f, y=%f\n", s.name, s.position.x, s.position.y);
    }
}

Vector2 shape_random_direction_vector() {
    float angle = (float) GetRandomValue(0,360) * PI / 180.0f;
    Vector2 direction = {
        .x = cosf(angle),
        .y = sinf(angle),
    };
    Vector2 velocity = {
        .x = direction.x * SHAPE_SPEED,
        .y = direction.y * SHAPE_SPEED
    };

    return velocity;
}

void draw_shapes(Shapes* shapes) {
    for (size_t i = 0; i < shapes->size; i++) {
        const Shape s = shapes->items[i];

        if (s.should_draw) {
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

void update_shapes(Shapes* shapes) { 
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();

    for (size_t i = 0; i < shapes->size; i++) {
        Shape* s = &shapes->items[i];

        if (s->should_draw) {
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

void draw_menu(bool* showMenu) {
    const int screenW = GetScreenWidth();
    if (GuiButton((Rectangle){ screenW - 150, 20, 120, 30 }, "Toggle Menu")) *showMenu = !(*showMenu);
    if (*showMenu) {
        int result = GuiMessageBox((Rectangle){ screenW - 280, 60, 250, 100 }, "#191#Message Box", "Hi! This is a message!", "Nice;Cool");
        if (result >= 0) *showMenu = false;
    }
}

int main(int argc, [[maybe_unused]] char* argv[argc + 1]) {
    Font customFont = LoadFont("resources/fonts/PixAntiqua.ttf");

    InitWindow(WIDTH, HEIGHT, "Untitled Game");
    SetTargetFPS(60);

    bool showMenu = false;
    Shapes shapes = {};
    Shape red_circle = {
        .position = { 100, 100 },
        .name = "Red Circle",
        .scale = 1.0f,
        .size = 50.0f,
        .color = RED,
        .kind = SHAPE_CIRCLE,
        .velocity = shape_random_direction_vector(),
        .should_draw = true,
    };
    Shape green_rectangle = {
        .position = { 200, 200 },
        .name = "Green Rectangle",
        .scale = 1.0f,
        .size = 50.0f,
        .color = GREEN,
        .kind = SHAPE_RECTANGLE,
        .velocity = shape_random_direction_vector(),
        .should_draw = true,
    };

    append(shapes, red_circle);
    append(shapes, green_rectangle);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            update_shapes(&shapes);
            draw_menu(&showMenu);
            draw_shapes(&shapes);
            print_shapes(&shapes);
        EndDrawing();
    }

    UnloadFont(customFont);
    CloseWindow();
    free(shapes.items);

    return EXIT_SUCCESS;
}
