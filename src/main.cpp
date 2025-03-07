#include "raylib.h"
#include "raymath.h"
#include <iostream>

using namespace std;

//----------------------------------------------------------------------------------
// Some Defines y ENUMS
//----------------------------------------------------------------------------------
#define BACKGROUND "resources/world/World_1_1.png"
#define SPRITESHEET "resources/sprites/NES - Super Mario Bros - Mario & Luigi.png"
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
    bool mirando_derecha = true;
    int sprite_status = 0;
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
static float worldPosition = 0;



Camera2D camera = { 0 };
Texture2D background;
Texture2D spriteSheet;


static int tiempo = 400;   // Tiempo en cuenta regresiva
static int monedas = 0;    // Contador de monedas
static int vidas = 3;      // Número de vidas de Mario
static int world = 1;
static int level = 1;


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
    spriteSheet = LoadTexture(SPRITESHEET);

    Mario mario(380, 380); //Creo el objeto de Mario
    Hitbox lista_hitboxes[] = {
        { {0, 416, 1000, 400}, 1, BLUE }
    }; // Creo la lista de todas las colisiones

    int envItemsLength = 1;
   
    score = 0;
    gameOver = false;


    camera.target = { mario.position.x + 20.0f, mario.position.y + 20.0f };
    camera.offset = { mario.position.x, mario.position.y + 20.0f };
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
void UpdateCameraCenterInsideMap(Camera2D* camera, Mario* mario, Hitbox* envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenter(Camera2D* camera, Mario* mario, Hitbox* envItems, int envItemsLength, float delta, int width, int height);

// Initialize game variables
void InitGame(void)
{
   

   

}

// Update game (one frame)
void UpdateGame(Mario *mario, Hitbox *hitboxes, float delta)
{
    if (!gameOver)
    {
        /* Movimiento de Mario */
        if (IsKeyDown(KEY_RIGHT)) {
            mario->position.x += 5;
            
            if (mario->sprite_status == 56)
                mario->sprite_status = 20;
            else
                mario->sprite_status += 18;

            if (mario->position.x >= (screenWidth / 2) - 12) {
                worldPosition = mario->position.x;
                mario->mirando_derecha = true;
                if (mario->sprite_status >= 56)
                {
                    mario->sprite_status = 20;
                }
                else
                {
                    mario->sprite_status += 18;
                }
            }

        }
        else if (IsKeyDown(KEY_LEFT)) {
            //Esto es para que mario no pueda salirse del mapa por la izquierda
            if (mario->position.x >= (GetScreenToWorld2D({ (1 - 0.16f) * 0.5f * screenWidth, (1 - 0.16f) * 0.5f * screenHeight }, camera).x) - 325) {
                mario->position.x -= 5;
                mario->mirando_derecha = false;
                if (mario->sprite_status >= 56)
                {
                    mario->sprite_status = 20;
                }
                else
                {
                    mario->sprite_status += 18;
                }
            }
        }
        else
        {
            mario->sprite_status = 0;
        }

        /* Salto */
        if (IsKeyDown(KEY_SPACE) ) {
            mario->position.y -= 10;
        }
      

        

        // Camera target follows player
        //camera.target = { mario->position.x + 20, mario->position.y + 20 };
        UpdateCameraCenter(&camera, mario, hitboxes, 1, delta, screenWidth, screenHeight);
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
        }
    }

    static float tiempoAcumulado = 0;

    tiempoAcumulado += delta;
    if (tiempoAcumulado >= 1.0f) {
        tiempo -= 1;
        tiempoAcumulado = 0;  // Reiniciamos el acumulador
    }

    if (tiempo <= 0) {
        tiempo = 0;
        gameOver = true;  // Termina el juego si el tiempo llega a 0
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
    Vector2 marioIni={mario->position.x, mario->position.y};
    Rectangle marioRecorte = { mario->sprite_status, 8, 16, 16 };

        //*Cambio de derecha a izquierda
    int mariowidth;
    if (mario->mirando_derecha == true)
    {
        marioRecorte.width = 16;
    }
    else
    {
        marioRecorte.width = -16;
    }

    Rectangle marioResized = { marioIni.x, marioIni.y, marioRecorte.width*2, marioRecorte.height * 2 }; // Escalado
    Vector2 MarioOrigen = {0,0};
        
    

    DrawTexturePro(spriteSheet, marioRecorte,marioResized,MarioOrigen,0, WHITE);

    

    /* Dibujado de Hitbox */
    for (int i = 0; i < 1; i++) 
        DrawRectangleRec(hitboxes[i].rect, hitboxes[i].color);

    EndMode2D();

    /*Hud draw*/
    DrawText("MARIO", 20, 10, 20, WHITE);
    DrawText(TextFormat("%06i", score), 20, 30, 20, WHITE);

    
    DrawText(TextFormat("x %02i", monedas), 180, 30, 20, YELLOW);

    DrawText("TIME", screenWidth - 100, 10, 20, WHITE);
    DrawText(TextFormat("%03i", tiempo), screenWidth - 100, 30, 20, WHITE);

    DrawText("WORLD", screenWidth / 2 - 40, 10, 20, WHITE);
    DrawText(TextFormat("%i - %i", world, level), screenWidth / 2 - 25, 30, 20, WHITE);

    

    EndDrawing();

}

void UpdateCameraCenter(Camera2D* camera, Mario* mario, Hitbox* envItems, int envItemsLength, float delta, int width, int height)
{
    static Vector2 bbox = { 0.16f, 0.16f };

    Vector2 bboxWorldMin = GetScreenToWorld2D({ (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height }, *camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D({ (1 + bbox.x) * 0.4f * width, (1 + bbox.y) * 0.5f * height }, *camera);
    //cout << mario->position.x << endl;
    //camera->target = { mario->position.x + 20, mario->position.y + 20 };
    camera->offset = { (1 - bbox.x) * 0.5f * width, 380};

    //if (mario->position.x < (width / 2) - 12) {
    //    camera->offset = { mario->position.x, mario->position.y + 20 };
    //}
   
    /*if (mario->position.x < bboxWorldMin.x) 
        camera->target.x = mario->position.x;*/
    if (mario->position.x > bboxWorldMax.x) 
        camera->target.x = bboxWorldMin.x + (mario->position.x - bboxWorldMax.x);

   
}

// Unload game variables
void UnloadGame(void)
{
    UnloadTexture(spriteSheet);
    UnloadTexture(background);
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}


// 