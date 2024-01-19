#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <complex.h>
#include <raylib.h>
#include <raymath.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

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

    double theta0 = atof(thetaArg);
    if (theta0 == 0.0l) {
        printf("ERROR: Invalid theta\n");
        exit(EXIT_FAILURE);
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pi Visualization");

    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Camera2D cam = {0};
    cam.zoom = 1;
    
    double theta1 = theta0;

    int frameCount = 0;
    bool toggle = 0;

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
            cam.zoom += wheel * 1.25f;
            if (cam.zoom < 1.0f)
                cam.zoom = 1.0f;
        }

        if (IsKeyReleased(KEY_N)) {
            frameCount = 0;
            theta1 += theta0;
        }

        if (IsKeyReleased(KEY_B)) {
            frameCount = 0;
            theta1 -= theta0;
        }

        if (IsKeyPressed(KEY_F2)) toggle = !toggle;

        Vector2 center = {
            .x = SCREEN_WIDTH / 2.0f,
            .y = SCREEN_HEIGHT / 2.0f,
        };

        BeginDrawing();
        {
            ClearBackground(BLACK);

            BeginMode2D(cam);
            {
                // DrawTexture(target.texture, 0, 0, RAYWHITE);
                double theta = 0.0l;
                for (int i = 0 ; i < frameCount; ++i) {

                    double _Complex prevPoint = cexpl(I * theta) + cexpl(I * M_PI * theta);
                    theta += theta1;
                    double _Complex point = cexpl(I * theta) + cexpl(I * M_PI * theta);

                    Vector2 start = {center.x + crealf(prevPoint) * 100, center.y + cimagf(prevPoint) * 100 };
                    Vector2 end = {center.x + crealf(point) * 100, center.y  + cimagf(point) * 100 };
        
                    DrawLineV(start, end, RAYWHITE);
                }
            }
            EndMode2D();
                
            if (!toggle) {
                DrawText("Press ESC to exit", 10, 10, 20, WHITE);
                DrawText("N -> Next animation", 10, 40, 20, WHITE);
                DrawText("B -> Previous animation", 10, 70, 20, WHITE);
                DrawText("F2 -> Toggle this menu", 10, 100, 20, WHITE);
                DrawText(TextFormat("Frame: %i", frameCount), 10, 130, 20, WHITE);
                DrawText(TextFormat("Theta: %lf", theta1), 10, 160, 20, WHITE);
            }
        }
        
        EndDrawing();
        frameCount++;
    }

    CloseWindow();

    return 0;
}

