#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "raymath.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_POINTS 100000

int main(int argc, char **argv) 
{
    (void)argc;
    char *program = *argv++;
    assert(program != NULL);
    
    char *thetaSpeedArg = *argv++;
    if (thetaSpeedArg == NULL) {
        printf("USAGE: %s <theta_increment>\n", program);
        exit(EXIT_FAILURE);
    }

    float thetaSpeed = 0.01f * atof(thetaSpeedArg);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pi Visualization");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Camera2D cam = {0};
    cam.zoom = 1;

    float theta = 0.0f;

    float _Complex outerPath[MAX_POINTS];
    int pathIndex = 0;

    while (!WindowShouldClose()) {

        // translate based on left click
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f / cam.zoom);
            cam.target = Vector2Add(cam.target, delta);
        }

        // zoom based on wheel
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), cam);
            cam.offset = GetMousePosition();
            cam.target = mouseWorldPos;

            // zoom
            cam.zoom += wheel * 0.125f;
            if (cam.zoom < 0.125f)
                cam.zoom = 0.125f;
        }

        theta += thetaSpeed;

        float _Complex innerRod = cexpf(I * theta);
        float _Complex outerRod = cexpf(I * M_PI * theta);

        outerPath[pathIndex] = innerRod + outerRod;

        pathIndex++;
        if (pathIndex >= MAX_POINTS) {
            pathIndex = 0;
        }

        Vector2 center = {
            .x = GetScreenWidth() / 2.0f,
            .y = GetScreenHeight() / 2.0f,
        };

        if (IsKeyReleased(KEY_N)) {
            pathIndex = 0;
            thetaSpeed += 1.0f;
            // thetaSpeed *= 2.0f;
        }

        if (IsKeyReleased(KEY_B)) {
            pathIndex = 0;
            thetaSpeed -= 1.0f;
            // thetaSpeed /= 1.0f;
        }

        BeginDrawing();
        {
            ClearBackground(BLACK);

            BeginMode2D(cam);
            {
                for (int i = 1; i < pathIndex; i++) {
                    Vector2 start = {center.x + crealf(outerPath[i - 1]) * 100, center.y + cimagf(outerPath[i - 1]) * 100};
                    Vector2 end = {center.x + crealf(outerPath[i]) * 100, center.y  + cimagf(outerPath[i]) * 100};
                    DrawLineV(start, end, RAYWHITE);
                }

                // Vector2 lastOuterPoint = {center.x + crealf(outerPath[pathIndex - 1]) * 100, center.y + cimagf(outerPath[pathIndex - 1]) * 100};
                // Vector2 outerPoint = {center.x + crealf(outerRod) * 100, center.y + cimagf(outerRod) * 100};

                // DrawLineV(lastOuterPoint, outerPoint, LIGHTGRAY);
                // DrawLineV(outerPoint, center, LIGHTGRAY);

                // DrawCircle(center.x, center.y , 2, GRAY);
                // DrawCircleV(lastOuterPoint, 2, GRAY);
                // DrawCircleV(outerPoint, 2, GRAY);
            }
            EndMode2D();

            DrawText("Press ESC to exit", 10, 10, 20, WHITE);
            DrawText(TextFormat("Frame: %i", pathIndex), 10, 40, 20, WHITE);
            DrawText(TextFormat("/%i", MAX_POINTS), 150, 40, 20, WHITE);
            DrawText(TextFormat("Speed: %f", thetaSpeed), 10, 80, 20, WHITE);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
