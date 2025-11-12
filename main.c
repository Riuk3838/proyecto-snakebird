

#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//structura
typedef struct square_collider{ 
    Vector2 position;
    int size;
} square_collider;

int collision (Vector2 point, square_collider col);

square_collider terrain[10]; 

Texture2D cara, cuerpo, cola;

typedef struct gusano
{
    segment body[10];
} gusano;

typedef struct segment{
    Vector2 position;
    
} segment;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);          // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "snakebird");
    for(int i=0; i<10; i++)
    {
        terrain[i].position.x = i*50;
        terrain[i].position.y = 200;
        terrain[i].size = 50;
    }
    
    cara = LoadTexture("cara.png");
    
    
    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(SKYBLUE);

        for (int i = 0; i < 10; i++)
        {
            DrawRectangle(terrain[i].position.x, terrain[i].position.y, terrain[i].size, terrain[i].size, GREEN);
        }
    

    EndDrawing();
    //----------------------------------------------------------------------------------
}

int collision (Vector2 point, square_collider col)
{
    if(point.x>col.position.x && point.x<col.position.x+col.size && point.y>col.position.y && point.y<col.position.y+col.size)
    {
        return 1;
    }
    return 0;
}