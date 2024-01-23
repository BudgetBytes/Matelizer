#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <complex.h>
#include <raylib.h>
#include <raymath.h>

#define SCREEN_WIDTH    1200
#define SCREEN_HEIGHT   800
#define MAX_INPUT_CHARS 8
#define UUID_LEN        37
#define MAX_COLORS      8

const Color COLORS[MAX_COLORS] = {RAYWHITE, GOLD, PINK, LIME, SKYBLUE, VIOLET, BEIGE};

void generate_uuid(char* str) {
    srand(time(NULL));
    for (size_t i = 0; i < UUID_LEN - 1; ++i) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            str[i] = '-';
        } else if (i == 14) {
            str[i] = '4';
        } else {
            size_t random = rand() % 16;
            if (random >= 10) {
                char letter = 'A' + (random - 10);
                str[i] = letter;
            } else {
                char digit = '0' + random;
                str[i] = digit;
            }
        }
    }
    str[UUID_LEN - 1] = '\0';
}

int main(void) 
{
    char thetaInput[MAX_INPUT_CHARS + 1] = "\0";
    int letterCount = 0;

    Rectangle textBox = {10, 40, 150, 30};
    bool mouseOnText = false;
    
    Rectangle colorsRecs[MAX_COLORS] = {0};

    for (size_t i = 0; i < MAX_COLORS; ++i){
        colorsRecs[i].x = 10 + 30.0f*i + 2*i;
        colorsRecs[i].y = 10;
        colorsRecs[i].width = 20;
        colorsRecs[i].height = 20;
    }

    size_t colorSelected = 0;
    int colorMouseHover = colorSelected;

    double theta0 = 3.14l;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pi Irrationality");

    SetTargetFPS(40);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    double theta1 = theta0;

    size_t frameCount = 0;
    bool toggle = false;
    bool autom = true;
    
    double theta = 0.0l;
    bool spinOnCenter = false;
    bool stopFrameCount = false;
    bool isCursorDisabled = false;
    
    while (!WindowShouldClose()) {

        UpdateCamera(&camera, CAMERA_THIRD_PERSON);

        if (IsKeyPressed('Z')) {
            isCursorDisabled ? EnableCursor() : DisableCursor();
            isCursorDisabled = !isCursorDisabled;
        }

        if (IsKeyReleased(KEY_N)) {
            frameCount = 0;
            theta1 += theta0;
            stopFrameCount = false;
        }

        if (IsKeyReleased(KEY_B)) {
            frameCount = 0;
            theta1 -= theta0;
            stopFrameCount = false;
        }
        
        if (IsKeyPressed(KEY_S)) {
            char uuid_str[UUID_LEN];
            generate_uuid(uuid_str);

            const char *filename = TextFormat("matelizer-%s.png", uuid_str);

            TakeScreenshot(filename);
            if (rename(filename, TextFormat("./screenshots/%s", filename)) < 0) {
                printf("ERROR: Failed to move file: %s", strerror(errno));
            }
        }

        if (IsKeyPressed(KEY_F2)) toggle = !toggle;

        if (IsKeyPressed(KEY_A)) autom = !autom;

        if (IsKeyPressed(KEY_R)) spinOnCenter = !spinOnCenter;

        if (IsKeyPressed(KEY_SPACE)) stopFrameCount = !stopFrameCount;
            
        if (frameCount > 1000 && autom) {
            frameCount = 0;
            theta1 += theta0;
            stopFrameCount = false;
        }

        Vector2 mousePos = GetMousePosition();

        if (CheckCollisionPointRec(mousePos, textBox)) mouseOnText = true;
        else mouseOnText = false;

        // Choose color with mouse
        for (size_t i = 0; i < MAX_COLORS; i++)
        {
            if (CheckCollisionPointRec(mousePos, colorsRecs[i]))
            {
                colorMouseHover = i;
                break;
            }
            else colorMouseHover = -1;
        }

        if ((colorMouseHover >= 0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            colorSelected = colorMouseHover;
        }


        if (mouseOnText)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            size_t key = GetCharPressed();

            while (key > 0)
            {
                if ((((key >= 48) && (key <= 57)) || key == 46) && (letterCount < MAX_INPUT_CHARS))
                {
                    thetaInput[letterCount] = (char)key;
                    thetaInput[letterCount+1] = '\0'; 
                    letterCount++;
                }

                key = GetCharPressed();  
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

            BeginMode3D(camera);
            {
                if (!spinOnCenter) theta = 0.0l;

                for (size_t i = 0 ; i < frameCount; ++i) {

                    double _Complex prevPoint = cexpl(I * theta) + cexpl(I * M_PI * theta);
                    theta += theta1;
                    double _Complex point = cexpl(I * theta) + cexpl(I * M_PI * theta);

                    Vector3 start = {
                        .x = crealf(prevPoint),
                        .y = cimagf(prevPoint),
                        .z = cabs(prevPoint), 
                    };

                    Vector3 end = {
                        .x = crealf(point),
                        .y = cimagf(point),
                        .z = cabs(point),
                    };

                    DrawLine3D(start, end, COLORS[colorSelected]);
                }
            }
            EndMode3D();


            if (!toggle) {
                // Draw color selection rectangles
                for (int i = 0; i < MAX_COLORS; i++) DrawRectangleRec(colorsRecs[i], COLORS[i]);
                DrawText("Press ESC to exit", 10, 70, 20, WHITE);
                DrawText("A -> Automatic animation", 10, 100, 20, WHITE);
                DrawText("N -> Next animation", 10, 130, 20, WHITE);
                DrawText("B -> Previous animation", 10, 160, 20, WHITE);
                DrawText("R -> Rotate on center", 10, 190, 20, WHITE);
                DrawText("S -> Take screenshot", 10, 220, 20, WHITE);
                DrawText("SPACE -> Stop incrementing", 10, 250, 20, WHITE);
                DrawText("F2 -> Toggle this menu", 10, 280, 20, WHITE);
                DrawText(TextFormat("Frame: %i", frameCount), 10, 310, 20, WHITE);
                DrawText(TextFormat("Theta: %lf", theta1), 10, 340, 20, WHITE);
                DrawText(TextFormat("Automatic: %s", autom ? "True" : "False"), 10, 370, 20, WHITE);

                DrawRectangleRec(textBox, LIGHTGRAY);
                if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RAYWHITE);
                else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

                DrawText(thetaInput, (int)textBox.x + 5, (int)textBox.y + 8, (int)textBox.height - 10, BLACK);
            }
        }
        
        EndDrawing();
        WaitTime(0.01);
        if (!stopFrameCount) frameCount++;
    }
    CloseWindow();

    return 0;
}

