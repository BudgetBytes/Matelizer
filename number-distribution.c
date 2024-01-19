#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 
#include <raylib.h>
#include <raymath.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char **argv) {

    (void)argc;
    char *program = *argv++;
    assert(program != NULL);
    char *arg = *argv++;
    if (arg == NULL) {
        printf("USAGE: %s <max number>\n", program);
        return 1;
    }

    int max_number = atoi(arg);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Numbers Distribution");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Camera2D cam = {0};
    cam.zoom = 1;

    while (!WindowShouldClose()) {
        // translate based on left click
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f / cam.zoom);
            cam.target = Vector2Add(cam.target, delta);
        }

        // zoom based on wheel
        double wheel = GetMouseWheelMove();
        if (wheel != 0) {
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), cam);
            cam.offset = GetMousePosition();
            cam.target = mouseWorldPos;

            // zoom
            cam.zoom += wheel * 0.125f;
            if (cam.zoom < 0.05f)
                cam.zoom = 0.05f;
        }



        ClearBackground(BLACK);
        BeginDrawing();
        BeginMode2D(cam);

        {
            Vector2 center = {
                .x = GetScreenWidth() / 2.0f,
                .y = GetScreenHeight() / 2.0f,
            };
            for (int i = 0; i < max_number; ++i) {
                Vector2 polar = {
                    .x = i * cos(i),
                    .y = i * sin(i),
                };
                if (i % 2 == 0) {
                    DrawCircle(center.x + polar.x, center.y + polar.y, 2/cam.zoom, RAYWHITE);
                } else {

                    DrawCircle(center.x + polar.x, center.y + polar.y, 2/cam.zoom, BLUE);
                }
            }
        }
        EndDrawing();
    }
    return 0;
}
