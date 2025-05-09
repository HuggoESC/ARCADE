//Copyright(c) 2025[unicef]
//All rights reserved.
//This code is for educational viewing purposes only.
//Use, modification, or distribution is prohibited without explicit permission.

#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>
#include <filesystem>
#include "Mario.h"

using namespace std;
namespace fs = filesystem;

#pragma region DEFINES y ENUMS

#define TILEMAP "../../resources/world/TILE_MAP.png"
#define BACKGROUND "../../resources/world/Empty_World_1_1.png"
#define SPRITESHEET "../../resources/sprites/NES - Super Mario Bros - Mario & Luigi.png"
#define ENEMIES "../../resources/sprites/NES - Super Mario Bros - Enemies & Bosses.png"
#define SOUNDS "../../resources/Super Mario Bros Efects"

#define PLAYER_JUMP_SPD 500.0f
#define GRAVEDAD 700
#define INICIALPAGE "../../resources/NES - Super Mario Bros - Title Screen HUD and Miscellaneous (1).png"

enum STAGE {
    INICIAL,
    INTRO,
    PLAYING
};

enum TILE_TYPE {
    VACIO,
    SUELO,
    BLOQUE,
    BLOQUEMONEDA,
    BLOQUESOLIDO,
    INVISIBLE,
    TUBERIA
};

#pragma endregion

//----------------------------------------------------------------------------------
// Class Definition
//----------------------------------------------------------------------------------

typedef struct Tile {
    bool isVisible;
    int neighbourMask;
    TILE_TYPE type;
} Tile;

class Goomba {
public:
    bool mirando_derecha = true;
    int sprite_status = 0;
    Rectangle position;
    float velocidad;

    Goomba();

    Goomba(float x, float y) {
        position = { x, y };
    }
};

struct Hitbox {
    Rectangle rect;
    int blocking;
    Color color;
};


#pragma region VARIABLES GLOBALES

constexpr int WORLD_WIDTH = 3376;
constexpr int WORLD_HEIGHT = 480;
constexpr int TILE_SIZE = 16;
constexpr Vector2 WORLD_GRID = { WORLD_WIDTH / TILE_SIZE, WORLD_HEIGHT / TILE_SIZE };

static Music music; //HUGO 
static Music Die;
static Music Gameover;
static Music Pause;

static Sound JumpSound;

static const int screenWidth = 800;
static const int screenHeight = 480;

static int selectedOption = 0;  // 0 para 1 jugador, 1 para 2 jugadores

Camera2D camera = { 0 };
Texture2D background;
Texture2D spriteSheet;
Texture2D EnemySpriteSheet;
Texture2D backgroundInicial;

vector <Hitbox> lista_hitboxes;

struct GameState {
    bool initialized = false;
    bool gameOver = false;
    int score = 0;
    int tiempo = 400;   // Tiempo en cuenta regresiva
    float introTimer = 2.0f;
    int monedas = 0;    // Contador de monedas
    int vidas = 3;      // N�mero de vidas de Mario
    int world = 1;
    int level = 1;
    float worldPosition = 0;

    STAGE state = INICIAL;

    Tile WorldGrid[(int)WORLD_GRID.x][(int)WORLD_GRID.y];
};

static GameState* gameState;

#pragma endregion

#pragma region Inits
void InitGrid() {
    Image tileMap = LoadImage(TILEMAP);
    int posX = 0;
    int posY = 0;

    for (int x = 0; x < WORLD_GRID.x; x++)
    {
        for (int y = 0; y < WORLD_GRID.y; y++)
        {
            Tile temp = Tile{ false, 0, VACIO };
            
            Color pixel = GetImageColor(tileMap, posX, posY);
           
            if (pixel.r == 255 && pixel.g == 0 && pixel.b == 0) { //Rojo
                temp.isVisible = true;
                temp.type = BLOQUE;
            }
            else if (pixel.r == 0 && pixel.g == 255 && pixel.b == 0) { //Verde
                temp.isVisible = true;
                temp.type = TUBERIA;
            }
            else if (pixel.r == 0 && pixel.g == 0 && pixel.b == 255) { //Azul
                temp.isVisible = true;
                temp.type = BLOQUESOLIDO;
            }
            else if (pixel.r == 255 && pixel.g == 255 && pixel.b == 0) { //Amarillo
                temp.isVisible = true;
                temp.type = BLOQUEMONEDA;
            }
            else if (pixel.r == 255 && pixel.g == 0 && pixel.b == 255) { //Rosa?
                temp.isVisible = true;
                temp.type = SUELO;
            }
            
            gameState->WorldGrid[x][y] = temp;

            posY += TILE_SIZE;
        }
        posX += TILE_SIZE;
        posY = 0;
    }

}

void InitGame(void)
{
    GameState temp;
    gameState = &temp;

    background = LoadTexture(BACKGROUND); //Cargo la textura del background
    spriteSheet = LoadTexture(SPRITESHEET);
    EnemySpriteSheet = LoadTexture(ENEMIES);
    backgroundInicial = LoadTexture(INICIALPAGE);


    lista_hitboxes = {
    { {0, 414, 2208, 400},1,0},
    {{512,288,32,32},1,0},
    {{640,288,32,32},1,0},
    {{672,288,32,32},1,0},
    {{704,288,32,32},1,0},
    {{736,288,32,32},1,0},
    {{768,288,32,32},1,0},
    {{704,160,32,32},1,0},
    {{896,352,96,62},1,0},
    {{1216,320,96,94},1,0},
    {{1472,288,96,126},1,0},
    {{1824,288,64,126},1,0},
    {{2272,414,500,400},1,0},
    {{2464,288,32,32},1,0},
    {{2496,288,32,32},1,0},
    {{2528,288,32,32},1,0},
    {{2560,160,32,32},1,0},
    {{2592,160,32,32},1,0},
    {{2624,160,32,32},1,0},
    {{2656,160,32,32},1,0},
    {{2688,160,32,32},1,0},
    {{2720,160,32,32},1,0},
    {{2752,160,32,32},1,0},
    {{2784,160,32,32},1,0},
    {{2848,414,2048,400},1,0},
    {{2912,160,32,32},1,0},
    {{2944,160,32,32},1,0},
    {{2976,160,32,32},1,0},
    {{3008,160,32,32},1,0},
    {{3008,288,32,32},1,0},
    {{3200,288,32,32},1,0},
    {{3232,288,32,32},1,0},
    {{3392,288,32,32},1,0},
    {{3488,288,32,32},1,0},
    {{3488,160,32,32},1,0},
    {{3584,288,32,32},1,0},
    {{3776,288,32,32},1,0},
    {{3872,160,32,32},1,0},
    {{3904,160,32,32},1,0},
    {{3936,160,32,32},1,0},
    {{4096,160,32,32},1,0},
    {{4160,160,32,32},1,0},
    {{4128,160,32,32},1,0},
    {{4192,160,32,32},1,0},
    {{4128,288,32,32},1,0},
    {{4160,288,32,32},1,0},
    {{4288,382,32,32},1,0},
    {{4320,382,32,32},1,0},
    {{4352,382,32,32},1,0},
    {{4384,382,32,32},1,0},
    {{4320,350,32,32},1,0},
    {{4352,350,32,32},1,0},
    {{4384,350,32,32},1,0},
    {{4352,318,32,32},1,0},
    {{4384,318,32,32},1,0},
    {{4384,286,32,32},1,0},
    {{4480,286,32,32},1,0},
    {{4480,318,32,32},1,0},
    {{4512,318,32,32},1,0},
    {{4480,350,32,32},1,0},
    {{4512,350,32,32},1,0},
    {{4544,350,32,32},1,0},
    {{4480,382,32,32},1,0},
    {{4512,382,32,32},1,0},
    {{4544,382,32,32},1,0},
    {{4576,382,32,32},1,0},
    {{4736,382,32,32},1,0},
    {{4768,382,32,32},1,0},
    {{4800,382,32,32},1,0},
    {{4832,382,32,32},1,0},
    {{4864,382,32,32},1,0},
    {{4768,350,32,32},1,0},
    {{4800,350,32,32},1,0},
    {{4832,350,32,32},1,0},
    {{4864,350,32,32},1,0},
    {{4800,318,32,32},1,0},
    {{4832,318,32,32},1,0},
    {{4864,318,32,32},1,0},
    {{4832,286,32,32},1,0},
    {{4864,286,32,32},1,0},
    {{4960,414,2000,400},1,0},
    {{4960,382,32,32},1,0},
    {{4992,382,32,32},1,0},
    {{5024,382,32,32},1,0},
    {{5056,382,32,32},1,0},
    {{4960,350,32,32},1,0},
    {{4992,350,32,32},1,0},
    {{5024,350,32,32},1,0},
    {{4960,318,32,32},1,0},
    {{4992,318,32,32},1,0},
    {{4960,286,32,32},1,0},
    {{5216,352,64,62},1,0},
    {{5376,288,32,32},1,0},
    {{5408,288,32,32},1,0},
    {{5440,288,32,32},1,0},
    {{5472,288,32,32},1,0},
    {{5728,352,64,62},1,0},
    {{5792,382,288,32},1,0},
    {{5824,350,256,32},1,0},
    {{5856,318,224,32},1,0},
    {{5888,288,192,32},1,0},
    {{5920,256,160,32},1,0},
    {{5952,224,128,32},1,0},
    {{5984,192,96,32},1,0},
    {{6016,160,64,32},1,0},
    {{6336,382,32,32},1,0},
    {{6348,80,8,302},1,0}
    //CREO QUE NO HAY MAS HITBOXES (SOLO EL BLOQUE INVISIBLE)
    };

    //HUGO MUSICA
    InitAudioDevice(); // https://www.raylib.com/examples/audio/loader.html?name=audio_music_stream
    //MUSICA
    music = LoadMusicStream("../../resources/Super Mario Bros Music/overworld-theme-super-mario-world-made-with-Voicemod.wav");
    Die = LoadMusicStream("../../resources/Super Mario Bros Music/Die.wav");
    Gameover = LoadMusicStream("../../resources/Super Mario Bros Music/Game Over.wav");
    Pause = LoadMusicStream("../../resources/Super Mario Bros Music/Pause.wav");

    //sonidos
    Sound JumpSound = LoadSound("../../resources/Super Mario Bros Efects/Jump.wav");
    Sound UpSound = LoadSound("../../resources/Super Mario Bros Efects/1 up.wav");
    Sound BeepSound = LoadSound("../../resources/Super Mario Bros Efects/Beep.wav");
    Sound BigJumpSound = LoadSound("../../resources/Super Mario Bros Efects/Big Jump.wav");
    Sound Break = LoadSound("../../resources/Super Mario Bros Efects/Break.wav");
    Sound Bump = LoadSound("../../resources/Super Mario Bros Efects/Bump.wav");
    Sound Coin = LoadSound("../../resources/Super Mario Bros Efects/Coin.wav");
    Sound FireBall = LoadSound("../../resources/Super Mario Bros Efects/Fire Ball.wav");
    Sound Flagpole = LoadSound("../../resources/Super Mario Bros Efects/Flagpole.wav");
    Sound item = LoadSound("../../resources/Super Mario Bros Efects/Item.wav");
    Sound kick = LoadSound("../../resources/Super Mario Bros Efects/Kick.wav");
    Sound Powerup = LoadSound("../../resources/Super Mario Bros Efects/Powerup.wav");
    Sound Skid = LoadSound("../../resources/Super Mario Bros Efects/Skid.wav");
    Sound Squish = LoadSound("../../resources/Super Mario Bros Efects/Squish.wav");
    Sound Thwomp = LoadSound("../../resources/Super Mario Bros Efects/Tjwomp.wav");
    Sound Vine = LoadSound("../../resources/Super Mario Bros Efects/Vine.wav");
    Sound Warp = LoadSound("../../resources/Super Mario Bros Efects/Wa`rp.wav");
    Sound Enemyfire = LoadSound("../../resources/Super Mario Bros Efects/Enemy Fire.wav");

    PlayMusicStream(music);

    float timePlayed = 0.0f;
    bool pause = false;

    InitGrid();
}



void Reset(Mario* mario) {
    gameState->state = INICIAL;
    mario->position.x = 316;
    mario->position.y = 414;
    camera.target = { mario->position.x + 20.0f, mario->position.y - 32.0f };
    camera.offset = { mario->position.x, mario->position.y + 20.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    gameState->tiempo = 400;
    gameState->introTimer = 2.0f;
}

Tile* GetTile(int x, int y) {
    
    //Si las coordenadas estan dentro del mapa, devuelve el Tile que hay alli
    if (x >= 0 && x < WORLD_GRID.x && y >= 0 && y < WORLD_GRID.y) {

        return &gameState->WorldGrid[x][y];
    }

}

Tile* GetTile(Vector2 worldPos) {
    return GetTile(worldPos.x / TILE_SIZE, worldPos.y / TILE_SIZE);
}

void UnloadGame(void)
{
    UnloadTexture(spriteSheet);
    UnloadTexture(background);
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

#pragma endregion



#pragma region FUNCIONES DE DRAW

void DrawIntro() {

    BeginDrawing();

    /* Dibujado del Background */
    Rectangle initial_position = { 0,15, backgroundInicial.width - 282, backgroundInicial.height };
    Rectangle final_position = { 0, screenHeight / 2, backgroundInicial.width * 2, backgroundInicial.height * 2 };
    DrawTexturePro(backgroundInicial, initial_position, final_position, { 0, (float)(backgroundInicial.height / 2) }, 0, WHITE);

    DrawText("1 Player Game", screenWidth / 2 - 80, screenHeight / 2 + 60, 20, (selectedOption == 0) ? YELLOW : WHITE);
    DrawText("2 Player Game", screenWidth / 2 - 80, screenHeight / 2 + 100, 20, (selectedOption == 1) ? YELLOW : WHITE);

    EndDrawing();
}

void DrawIntroScreen() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("MARIO", 20, 10, 20, WHITE);
    DrawText(TextFormat("%06i", gameState->score), 20, 30, 20, WHITE);
    DrawText("WORLD", screenWidth / 2 - 40, 10, 20, WHITE);
    DrawText(TextFormat("%i - %i", gameState->world, gameState->level), screenWidth / 2 - 25, 30, 20, WHITE);
   
    DrawText("x", screenWidth / 2 - 20, screenHeight / 2, 20, WHITE);
    DrawText(TextFormat("%i", gameState->vidas), screenWidth / 2 + 10, screenHeight / 2, 20, WHITE);

    Rectangle marioRecorte = { 0, 8, 16, 16 };
    Rectangle marioResized = { screenWidth / 2 - 50, screenHeight / 2, marioRecorte.width * 2, marioRecorte.height * 2 };
    DrawTexturePro(spriteSheet, marioRecorte, marioResized, { 0, 0 }, 0, WHITE);
    EndDrawing();
}

void DrawGame(Mario* mario, Goomba* goomba1, vector<Hitbox> hitboxes)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode2D(camera);

    /* Dibujado del Background */
    Rectangle initial_position = { 0, 0, background.width, background.height };
    Rectangle final_position = { 0, screenHeight / 2, background.width * 2, background.height * 2 };
    DrawTexturePro(background, initial_position, final_position, { 0, (float)(background.height / 2) }, 0, WHITE);


    /***************************/

    /* Dibujado de Mario */
    Vector2 marioIni = { mario->position.x, mario->position.y };
    Rectangle marioRecorte = { mario->sprite_status, 8, 16, 16 };

    //*Cambio de derecha a izquierda
    if (mario->mirando_derecha)
    {
        marioRecorte.width = 16;
    }
    else
    {
        marioRecorte.width = -16;
    }

    Rectangle marioResized = { marioIni.x, marioIni.y - 32, 16 * 2, 16 * 2 }; // Escalado
    Vector2 MarioOrigen = { 0,0 };

    DrawTexturePro(spriteSheet, marioRecorte, marioResized, MarioOrigen, 0, WHITE);

    //Hitbox de mario
    //DrawRectangleLines(marioResized.x, marioResized.y, marioResized.width, marioResized.height, WHITE);

    //Goomba


    Vector2 PosGoomba1 = { goomba1->position.x,goomba1->position.y };
    Rectangle Goomba1Recorte = { goomba1->sprite_status, 17, 16,16 };

    Rectangle Goomba1Resized = { PosGoomba1.x, PosGoomba1.y - 32, 16 * 2, 16 * 2 }; // Escalado
    Vector2 GoombaOrigen = { 0,0 };
    DrawTexturePro(EnemySpriteSheet, Goomba1Recorte, Goomba1Resized, GoombaOrigen, 0, WHITE);
    //DrawRectangleLines(Goomba1Resized.x, Goomba1Resized.y, Goomba1Resized.width, Goomba1Resized.height, WHITE);

    /* Dibujado de Hitbox */
    float posX = 0;
    float posY = 0;
    for (int x = 0; x <= WORLD_GRID.x; x++)
    {
        for (int y = 0; y <= WORLD_GRID.y; y++)
        {
            switch (gameState->WorldGrid[x][y].type) {
            case BLOQUE: {
                DrawRectangleRec({posX, posY, TILE_SIZE*2, TILE_SIZE*2}, RED);
                break;
            };
            case BLOQUEMONEDA: {
                DrawRectangleRec({ posX, posY, TILE_SIZE*2, TILE_SIZE*2 }, YELLOW);

                break;
            };
            case SUELO: {
                DrawRectangleRec({ posX, posY, TILE_SIZE*2, TILE_SIZE*2 }, PINK);

                break;
            };
            case TUBERIA: {
                DrawRectangleRec({ posX, posY, TILE_SIZE*2, TILE_SIZE*2 }, GREEN);

                break;
            };
            case BLOQUESOLIDO: {
                DrawRectangleRec({ posX, posY, TILE_SIZE * 2, TILE_SIZE * 2 }, BLUE);

                break;
            };
            }

            posY += TILE_SIZE * 2;
        }
        posX += TILE_SIZE * 2;
        posY = 0;
    }

    for (int i = 0; i < hitboxes.size(); i++)
        DrawRectangleRec(hitboxes[i].rect, hitboxes[i].color);

    EndMode2D();

    /*Hud draw*/
    DrawText("MARIO", 20, 10, 20, WHITE);
    DrawText(TextFormat("%06i", gameState->score), 20, 30, 20, WHITE);


    DrawText(TextFormat("x %02i", gameState->monedas), 180, 30, 20, YELLOW);

    DrawText("TIME", screenWidth - 100, 10, 20, WHITE);
    DrawText(TextFormat("%03i", gameState->tiempo), screenWidth - 100, 30, 20, WHITE);

    DrawText("WORLD", screenWidth / 2 - 40, 10, 20, WHITE);
    DrawText(TextFormat("%i - %i", gameState->world, gameState->level), screenWidth / 2 - 25, 30, 20, WHITE);



    EndDrawing();

}

#pragma endregion

#pragma region FUNCIONES DE UPDATE

void UpdateTimer(float delta) {
    static float tiempoAcumulado = 0;

    tiempoAcumulado += delta;
    if (tiempoAcumulado >= 1.0f) {
        gameState->tiempo -= 1;
        tiempoAcumulado = 0;  // Reiniciamos el acumulador
    }

    if (gameState->tiempo <= 0) {
        gameState->tiempo = 0;
        gameState->gameOver = true;  // Termina el juego si el tiempo llega a 0
    }
}

void UpdateCameraCenter(Camera2D* camera, Mario* mario, Hitbox* envItems, int envItemsLength, float delta, int width, int height)
{
    static Vector2 bbox = { 0.16f, 0.16f };

    Vector2 bboxWorldMin = GetScreenToWorld2D({ (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height }, *camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D({ (1 + bbox.x) * 0.4f * width, (1 + bbox.y) * 0.5f * height }, *camera);
    //cout << mario->position.x << endl;
    //camera->target = { mario->position.x + 20, mario->position.y + 20 };
    camera->offset = { (1 - bbox.x) * 0.5f * width, 380 };

    //if (mario->position.x < (width / 2) - 12) {
    //    camera->offset = { mario->position.x, mario->position.y + 20 };
    //}

    /*if (mario->position.x < bboxWorldMin.x)
        camera->target.x = mario->position.x;*/
    if (mario->position.x > bboxWorldMax.x)
        camera->target.x = bboxWorldMin.x + (mario->position.x - bboxWorldMax.x);


}

void UpdateGameState(float delta) {
    if (gameState->state == INICIAL) {
       
        if (IsKeyPressed(KEY_DOWN)) selectedOption = 1;
        if (IsKeyPressed(KEY_UP)) selectedOption = 0;
        if (IsKeyPressed(KEY_ENTER)) gameState->state = INTRO;
    }
    else if (gameState->state == INTRO) {
        gameState->introTimer -= delta; //COMENTADO PARA PROBAR SELECCION PERSONAJE
        if (gameState->introTimer <= 0 || IsKeyPressed(KEY_ENTER)) {
            gameState->state = PLAYING;
        }
    }
}

void UpdateGame(Mario* mario, Goomba* goomba1, Hitbox* hitboxes, float delta, int envItems)
{
    if (!gameState->gameOver)
    {
        //colisions
       
        bool hitObstacle = false;

        for (int i = 0; i < envItems; i++)
        {
            Hitbox* ei = hitboxes + i;

            /*if (ei->blocking &&
                ei->rect.x <= mario->position.x &&
                ei->rect.x + ei->rect.width >= mario->position.x &&
                ei->rect.y >= mario->position.y &&
                ei->rect.y <= mario->position.y + mario->velocidad * delta)
                {
                    hitObstacle = true;
                    mario->velocidad = 0.0f;
                    mario->position.y = ei->rect.y;
                    break;

                }*/
        }

        /* Colision con enemigos */
        /*if (mario->position.x + 24 >= goomba1->position.x &&
            mario->position.x < (goomba1->position.x + 32) &&
            mario->position.y >= goomba1->position.y)
        {
            gameState->gameOver = true;
        }*/

        /* Movimiento de Mario */
        if (IsKeyDown(KEY_RIGHT) && mario->canMoveRight) {
            mario->MoveX(5, hitObstacle);
            //mario->position.x += 5;
            mario->mirando_derecha = true;

            if (mario->canJump) {
                if (mario->sprite_status >= 56)
                    mario->sprite_status = 20;
                else
                    mario->sprite_status += 18;
            }

            if (mario->position.x >= (screenWidth / 2) - 12) {
                gameState->worldPosition = mario->position.x;
            }
        }
        else if (IsKeyDown(KEY_LEFT) && mario->canMoveLeft) {
            //Esto es para que mario no pueda salirse del mapa por la izquierda
            if (mario->position.x >= (GetScreenToWorld2D({ (1 - 0.16f) * 0.5f * screenWidth, (1 - 0.16f) * 0.5f * screenHeight }, camera).x) - 325) {
                
                mario->MoveX(-5, hitObstacle);

                //mario->position.x -= 5;
                mario->mirando_derecha = false;
                
                if (mario->canJump) {
                    if (mario->sprite_status >= 56)
                        mario->sprite_status = 20;
                    else
                        mario->sprite_status += 18;
                }
            }
        }
        

        /* Salto */
        if (IsKeyDown(KEY_SPACE) && mario->canJump) {
            //mario->velocidad = -PLAYER_JUMP_SPD;
            mario->canJump = false;
            mario->sprite_status = 96;
            PlaySound(JumpSound);
        }

        UpdateCameraCenter(&camera, mario, hitboxes, 1, delta, screenWidth, screenHeight);

        if (!hitObstacle)
        {
            //mario->position.y += mario->velocidad * delta;
            //mario->velocidad += GRAVEDAD * delta;
            mario->canJump = false;
        }
        else {
            mario->canJump = true;
        }

        if (mario->canJump && !IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT))
            mario->sprite_status = 0;

    }
    else
    {
        Reset(mario);
        gameState->gameOver = false;
    }

    /* Si Mario llega al final o se cae */
    if (mario->position.y > 580 || mario->position.x >= 6336) {
        gameState->gameOver = true;
    }
}

bool CheckCollision(Rectangle *player, Rectangle *obstacle) {
    return  player->x < obstacle->x + obstacle->width &&   // Si mario choca desde la derecha
            player->x + player->width > obstacle->x &&     // Si mario choca desde la izquierda
            player->y < obstacle->y + obstacle->height &&  // Si mario choca desde abajo
            player->y + player->height > obstacle->y;      // Si mario choca desde arriba
}

#pragma endregion

int main(void)
{
    InitWindow(screenWidth, screenHeight, "classic game: Super Mario Bros.");

    InitGame();

    Mario mario(316, 414); //Creo el objeto de Mario
    Goomba goomba1(500, 414);

    camera.target = { mario.position.x + 20.0f, mario.position.y - 32.0f };
    camera.offset = { mario.position.x, mario.position.y + 20.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key

    {

        UpdateMusicStream(music); //HUGO

        float deltaTime = GetFrameTime();
        UpdateGameState(deltaTime);

        ClearBackground(RAYWHITE);

        if (gameState->state == INTRO) {
            DrawIntroScreen();
            PlayMusicStream(music);
        }
        else if (gameState->state == INICIAL) {
            DrawIntro();
            PlayMusicStream(music);
        }
        else {
            UpdateGame(&mario, &goomba1, &lista_hitboxes[0], deltaTime, lista_hitboxes.size());
            DrawGame(&mario, &goomba1, lista_hitboxes);
        }
    }


    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(music);
    CloseAudioDevice();
    UnloadGame();         // Unload loaded data (textures, sounds, models...)
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
