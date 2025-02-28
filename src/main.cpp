#include "raylib.h"
#include "raymath.h"
#include <iostream>

using namespace std;

//----------------------------------------------------------------------------------
// Some Defines y ENUMS
//----------------------------------------------------------------------------------
#define BACKGROUND "../../resources/world/World_1_1.png"
#define PLAYER_JUMP_SPD 350.0f
#define GRAVEDAD 400

enum Powers {
    BASE = 0,
    SETA = 1,
    FLOR = 2,
    ESTRELLA = 3
};

//----------------------------------------------------------------------------------
// Class Definition
//----------------------------------------------------------------------------------

class Mario {
public:
    Rectangle position;
    bool canJump = true;
    float velocidad;
    Powers poder;

    /* Constructores */
    Mario();
    
    Mario(float posX, float posY) {
        position = { posX, posY, 24, 32 };
        velocidad = 0;
        poder = BASE;
    }
};

struct Hitbox {
    Rectangle rect;
    int blocking;
    Color color;
};

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 480;

static bool gameOver = false;
static int score = 0;



Camera2D camera = { 0 };
Texture2D background;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(Mario* mario, Hitbox* hitboxes, float delta);       // Update game (one frame)
static void DrawGame(Mario* mario, Hitbox* hitboxes);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "classic game: Super Mario Bros.");
    background = LoadTexture(BACKGROUND); //Cargo la textura del background

    Mario mario(380, 380); //Creo el objeto de Mario
    Hitbox lista_hitboxes[] = {
        { {0, 416, 1000, 400}, 1, BLUE }
    }; // Creo la lista de todas las colisiones

    int envItemsLength = 1;


    score = 0;
    gameOver = false;


    camera.target = { mario.position.x + 20.0f, mario.position.y + 20.0f };
    camera.offset = { mario.position.x, mario.position.y };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        float deltaTime = GetFrameTime();

        UpdateGame(&mario, lista_hitboxes, deltaTime);
        DrawGame(&mario, lista_hitboxes);
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame();         // Unload loaded data (textures, sounds, models...)
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(void)
{
   

   

}

// Update game (one frame)
void UpdateGame(Mario *mario, Hitbox *hitboxes, float delta)
{

    if (!gameOver)
    {
        if (IsKeyDown(KEY_RIGHT)) mario->position.x += 5;
        else if (IsKeyDown(KEY_LEFT)) mario->position.x -= 5;

        if (IsKeyDown(KEY_SPACE) ) {
            mario->position.y -= 10;
        }
      
        // Camera target follows player
        camera.target = { mario->position.x + 20, mario->position.y + 20 };
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
        }
    }
}

// Draw game (one frame)
void DrawGame(Mario* mario, Hitbox* hitboxes)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode2D(camera);

    /* Dibujado del Background */
    Rectangle initial_position = {0, 0, background.width, background.height };
    Rectangle final_position = { 0, screenHeight / 2, background.width*2, background.height*2 };
    DrawTexturePro(background, initial_position, final_position, { 0, (float)(background.height / 2) }, 0, WHITE);
    /***************************/

    /* Dibujado de Mario */
    DrawRectangleRec(mario->position, RED);

    /* Dibujado de Hitbox */
    for (int i = 0; i < 1; i++) 
        DrawRectangleRec(hitboxes[i].rect, hitboxes[i].color);

    EndMode2D();

    EndDrawing();
}

void UpdateCameraCenter(Camera2D* camera, Mario* mario, Hitbox* envItems, int envItemsLength, float delta, int width, int height)
{
    camera->offset = { width / 2.0f, height / 2.0f };
    camera->target = { mario->position.x, mario->position.y };
}

// Unload game variables
void UnloadGame(void)
{
    UnloadTexture(background);
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}