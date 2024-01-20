#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <complex.h>
#include <raylib.h>
#include <raymath.h>

#define SCREEN_WIDTH    1200
#define SCREEN_HEIGHT   800
#define MAX_INPUT_CHARS 8


int main(void) 
{
   
    char thetaInput[MAX_INPUT_CHARS + 1] = "\0";
    int letterCount = 0;

    Rectangle textBox = {SCREEN_WIDTH - 220, 10, 200, 50};
    bool mouseOnText = false;

    double theta0 = 3.14l;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pi Irrationality");

    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Camera2D cam = {0};
    cam.zoom = 1;
    
    double theta1 = theta0;

    int frameCount = 0;
    bool toggle = false;
    bool autom = true;
    
    double theta = 0.0l;
    bool spinOnCenter = false;

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

        if (IsKeyPressed(KEY_A)) autom = !autom;

        if (IsKeyPressed(KEY_S)) spinOnCenter = !spinOnCenter;
            
        if (frameCount > 1000 && autom) {
            frameCount = 0;
            theta1 += theta0;
        }

        Vector2 center = {
            .x = SCREEN_WIDTH / 2.0f,
            .y = SCREEN_HEIGHT / 2.0f,
        };

        if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;

        if (mouseOnText)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((((key >= 48) && (key <= 57)) || key == 46) && (letterCount < MAX_INPUT_CHARS))
                {
                    thetaInput[letterCount] = (char)key;
                    thetaInput[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                    letterCount++;
                }

                key = GetCharPressed();  // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                thetaInput[letterCount] = '\0';
            }
            if (IsKeyPressed(KEY_ENTER)) {
                theta0 = atof(thetaInput);
                theta1 = theta0;
                frameCount = 0;
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        BeginDrawing();
        {
            ClearBackground(BLACK);

            BeginMode2D(cam);
            {
                if (!spinOnCenter) theta = 0.0l;
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
                DrawText("A -> Automatic animation", 10, 40, 20, WHITE);
                DrawText("N -> Next animation", 10, 70, 20, WHITE);
                DrawText("B -> Previous animation", 10, 100, 20, WHITE);
                DrawText("S -> Spin on center", 10, 130, 20, WHITE);
                DrawText("F2 -> Toggle this menu", 10, 160, 20, WHITE);
                DrawText(TextFormat("Frame: %i", frameCount), 10, 190, 20, WHITE);
                DrawText(TextFormat("Theta: %lf", theta1), 10, 220, 20, WHITE);
                DrawText(TextFormat("Automatic: %s", autom ? "True" : "False"), 10, 250, 20, WHITE);

                DrawRectangleRec(textBox, LIGHTGRAY);
                if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RAYWHITE);
                else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

                DrawText(thetaInput, (int)textBox.x + 5, (int)textBox.y + 8, 40, BLACK);
            }
        }
        
        EndDrawing();
        WaitTime(0.01);
        frameCount++;
    }

    CloseWindow();

    return 0;
}

