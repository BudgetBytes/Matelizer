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
#define MAX_POINTS 50000

int main(int argc, char **argv) 
{
    (void)argc;
    char *program = *argv++;
    assert(program != NULL);
    
    char *thetaArg = *argv++;
    if (thetaArg == NULL) {
        printf("USAGE: %s <theta_increment>\n", program);
        exit(EXIT_FAILURE);
    }

    double theta0 = 0.01l * atof(thetaArg);


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pi Visualization");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Camera2D cam = {0};
    cam.zoom = 1;
    
    double theta1 = theta0;
    double theta = 0.0l;

    double _Complex outerPath[MAX_POINTS];
    int pathIndex = 0;

    while (!WindowShouldClose()) {

        // translate based on left click
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0l / cam.zoom);
            cam.target = Vector2Add(cam.target, delta);
        }

        // zoom based on wheel
        double wheel = GetMouseWheelMove();
        if (wheel != 0) {
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), cam);
            cam.offset = GetMousePosition();
            cam.target = mouseWorldPos;

            // zoom
            cam.zoom += wheel * 1.25l;
            if (cam.zoom < 1.0l)
                cam.zoom = 1.0l;
        }

        theta += theta1;

        double _Complex innerRod = cexpf(I * theta);
        double _Complex outerRod = cexpf(I * M_PI * theta);

        outerPath[pathIndex] = innerRod + outerRod;

        pathIndex++;
        if (pathIndex >= MAX_POINTS) {
            pathIndex = 0;
        }

        Vector2 center = {
            .x = GetScreenWidth() / 2.0l,
            .y = GetScreenHeight() / 2.0l,
        };

        if (IsKeyReleased(KEY_N)) {
            pathIndex = 0;
            theta1 += theta0;
            printf("Theta: %lf\n", theta1);
        }

        if (IsKeyReleased(KEY_B)) {
            pathIndex = 0;
            theta1 -= theta0;
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

            }
            EndMode2D();

            DrawText("Press ESC to exit", 10, 10, 20, WHITE);
            DrawText("N -> Next animation", 10, 40, 20, WHITE);
            DrawText("B -> Previous animation", 10, 70, 20, WHITE);
            DrawText(TextFormat("Frame: %i", pathIndex), 10, 100, 20, WHITE);
            DrawText(TextFormat("/%i", MAX_POINTS), 150, 100, 20, WHITE);
            DrawText(TextFormat("Theta: %lf", theta1), 10, 130, 20, WHITE);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
