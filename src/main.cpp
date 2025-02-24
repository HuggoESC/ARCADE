#include "raylib.h"
#include <iostream>

using namespace std;

//----------------------------------------------------------------------------------
// Some Defines y ENUMS
//----------------------------------------------------------------------------------
#define MAX_TUBES 100
#define FLOPPY_RADIUS 24
#define TUBES_WIDTH 80
#define BACKGROUND "../../resources/world/World_1_1.png"

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
private:
    float velocidad;
    Powers poder = BASE;

public:
    /* Getters y Setters */

    int GetVelocidad() {
        return velocidad;
    }

    void SetVelocidad(int valor) {
        velocidad = valor;
    }

    Powers GetPower() {
        return poder;
    }

    void SetPower(Powers valor) {
        poder = valor;
    }

    /* Constructores */

    Mario(float vel, Powers pow) {
        velocidad = vel;
        poder = pow;
    };
};

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 480;

static bool gameOver = false;
static int score = 0;

Texture2D background;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "classic game: Super Mario Bros.");
    background = LoadTexture(BACKGROUND);
    InitGame();


    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
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
    score = 0;
    gameOver = false;



}

// Update game (one frame)
void UpdateGame(void)
{
    if (!gameOver)
    {
       
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
void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    /* Dibujado del Background */
    Rectangle initial_position = {0, 0, background.width, background.height };
    Rectangle final_position = { 0, screenHeight / 2, background.width*2, background.height*2 };
    DrawTexturePro(background, initial_position, final_position, { 0, (float)(background.height / 2) }, 0, WHITE);
    /***************************/

    
    EndDrawing();
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}

// Unload game variables
void UnloadGame(void)
{
    UnloadTexture(background);
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}