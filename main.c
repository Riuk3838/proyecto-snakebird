

#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif
typedef enum Direction{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 4 
} Direction;

typedef enum BodyType{
    CARA,
    CUERPO,
    ESQUINA,
    COLA
} BodyType;

Rectangle sourceRec = { 0.0f, 0.0f, 32.0f, 32.0f };

static Vector2 origin = {25,25};
//structura
typedef struct square_collider{ 
    Vector2 position;
    int size;
} square_collider;

int collision (Vector2 point, square_collider col);

square_collider terrain[10]; 

Texture2D cara, cuerpo, esquina, cola;

typedef struct segment{
    Vector2 position;
    Direction dir;
    BodyType type;
} segment;

typedef struct gusano
{
    int size;
    segment body[10];
} gusano;

gusano snakebird;


//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);          // Update and draw one frame
static void Move(Direction dir);
static void Grow(Direction dir);

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
    
    cara = LoadTexture("Images/cara-l.png");
    cuerpo = LoadTexture("Images/body-h.png");
    esquina = LoadTexture("Images/body-ul.png");
    cola = LoadTexture("Images/tail-l.png");
    
    snakebird.size = 3;
    snakebird.body[0].position = (Vector2){25,25};
    snakebird.body[1].position = (Vector2){75,25};
    snakebird.body[2].position = (Vector2){125,25};
    snakebird.body[0].dir = LEFT;
    snakebird.body[1].dir = LEFT;
    snakebird.body[2].dir = LEFT;
    snakebird.body[0].type = CARA;
    snakebird.body[1].type = CUERPO;
    snakebird.body[2].type = COLA;

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
    if(IsKeyPressed(KEY_UP)) Move(UP);
    if(IsKeyPressed(KEY_DOWN)) Move(DOWN);
    if(IsKeyPressed(KEY_LEFT)) Move(LEFT);
    if(IsKeyPressed(KEY_RIGHT)) Move(RIGHT);
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(SKYBLUE);

        for (int i = 0; i < 10; i++)
        {
            DrawRectangle(terrain[i].position.x, terrain[i].position.y, terrain[i].size, terrain[i].size, GREEN);
        }

        for(int i=0; i<snakebird.size; i++)
        {
            segment seg = snakebird.body[i];
           
            DrawTexturePro(seg.type == CARA ? cara : seg.type == CUERPO ? cuerpo : seg.type == ESQUINA ? esquina : cola, sourceRec, (Rectangle){seg.position.x,seg.position.y,50,50}, origin, 0, WHITE);
            
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

static void Move(Direction dir){
    for (int i = 9; i >= 1; i--)
    {
        snakebird.body[i].position.x = snakebird.body[i-1].position.x;
        snakebird.body[i].position.y = snakebird.body[i-1].position.y;
        snakebird.body[i].dir = snakebird.body[i-1].dir;
        snakebird.body[i].type = snakebird.body[i-1].type;
    }
    switch (snakebird.body[0].dir)
    {
    case UP:
    case DOWN: 
        snakebird.body[1].type = (dir == LEFT || dir == RIGHT) ? ESQUINA : CUERPO;
        break;
    case LEFT:
    case RIGHT:
        snakebird.body[1].type = (dir == UP || dir == DOWN) ? ESQUINA : CUERPO;
        break;
    }
    snakebird.body[snakebird.size-1].type = COLA;
    snakebird.body[0].position.x += dir==LEFT ? -50 : dir==RIGHT ? 50 : 0;
    snakebird.body[0].position.y += dir==UP ? -50 : dir==DOWN ? 50 : 0;
    snakebird.body[0].dir = dir;
 }

 static void Grow(Direction dir){
    snakebird.size++;
    Move(dir);
    
 }