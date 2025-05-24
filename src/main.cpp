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

#define BACKGROUND  "../../resources/world/Empty_World_1_1.png"
#define SPRITESHEET "../../resources/sprites/NES - Super Mario Bros - Mario & Luigi.png"
#define ENEMIES     "../../resources/sprites/NES - Super Mario Bros - Enemies & Bosses.png"
#define BLOCKS     "../../resources/sprites/NES - Super Mario Bros - Item and Brick Blocks.png"
#define SOUNDS      "../../resources/Super Mario Bros Efects"
#define MUSICS      "../../resources/Super Mario Bros Music"
#define INICIALPAGE "../../resources/NES - Super Mario Bros - Title Screen HUD and Miscellaneous (1).png"

#define PLAYER_JUMP_SPD 500.0f
#define GRAVEDAD 700
#define ACELERACION 0.5f
#define VELOCIDAD_MAXIMA 5.0f


enum GameState {
    INICIAL,
    INTRO,
    PLAYING,
    GAME_OVER, 
};

enum Powers {
    BASE = 0,
    SETA = 1,
    FLOR = 2,
    ESTRELLA = 3
};

enum BlockType {
    BLOQUE = 0,
    BLOQUEMONEDA = 1,
    TUBERIA = 2
};

#pragma endregion

//----------------------------------------------------------------------------------
// Class Definition
//----------------------------------------------------------------------------------

class Goomba {
public:

    int CurrentFrame = 0;
    float animationTimer = 0.0f;
    float framespeed = 0.2f;
    bool mirando_derecha;
    bool activo;
   /* int sprite_status = 0;*/
    Rectangle position;

    Rectangle pies;
    Rectangle cabeza;
    Rectangle derecha;
    Rectangle izquierda;

    float velocidad;
    Rectangle posicion_inicial;

    Goomba() {};

    Goomba(float x, float y) {
        position = { x, y, 32, 32 };
        posicion_inicial = position;
        velocidad = 30.0f;
        mirando_derecha = false;
        activo = true;
    }

    // Método reset para restaurar estado inicial
    void reset() {
        position = posicion_inicial;
        velocidad = 30.0f;
        mirando_derecha = false;
        activo = true;
        CurrentFrame = 0;
        animationTimer = 0.0f;
    }


    void Update(float delta)
    {
        if (!activo) return;

        position.x += (mirando_derecha ? velocidad : -velocidad) * delta;

        if (position.x < 0) mirando_derecha = true;
        if (position.x > 6000) mirando_derecha = false;

        animationTimer += delta;

        if (animationTimer >= framespeed) 
        {
            CurrentFrame = (CurrentFrame + 1) % 2;
            animationTimer = 0.0f;
        }
    }

    void Draw(Texture2D enemyTexture) {
        Rectangle source = { 18.0f * CurrentFrame, 16, 16, 16 };
        if (!mirando_derecha) source.width = -16;

        Rectangle dest = position;
        dest.y -= 32;
        DrawTexturePro(enemyTexture, source, dest, { 0, 0 }, 0.0f, WHITE);
    }
};

struct Hitbox {
    Rectangle rect;
    int blocking;
    Color color;
    BlockType type;

    Hitbox(Rectangle r) {
        rect = r;
    }

    Hitbox(Rectangle r, BlockType t) {
        rect = r;
        type = t;
    }
};

#pragma region VARIABLES GLOBALES

static Music music; //HUGO 
static Music Gameover;

bool playDeathSound = false;
bool musicRestarted = false;
bool musicPlaying = false;

static Sound Die;
static Sound Pause;
static Sound JumpSound;
static Sound UpSound;
static Sound BeepSound;
static Sound BigJumpSound;
static Sound Break;
static Sound Bump;
static Sound Coin;
static Sound FireBall;
static Sound Flagpole;
static Sound item;
static Sound kick;
static Sound Powerup;
static Sound Skid;
static Sound Squish;
static Sound Thwomp;
static Sound Vine;
static Sound Warp;
static Sound Enemyfire;

static const int screenWidth = 800;
static const int screenHeight = 480;
static GameState gameState = INICIAL;
static float introTimer = 2.0f;

static bool gameOver = false;
static bool gameovermusicplayed = false;
static int score = 0;
static float worldPosition = 0;
static int selectedOption = 0;  // 0 para 1 jugador, 1 para 2 jugadores

Camera2D camera = { 0 };
Texture2D background;
Texture2D spriteSheet;
Texture2D blocksheet;
Texture2D EnemySpriteSheet;
Texture2D backgroundInicial;

static int tiempo = 400;   // Tiempo en cuenta regresiva
static int monedas = 0;    // Contador de monedas
static int vidas = 3;      // N�mero de vidas de Mario
static int world = 1;
static int level = 1;
vector <Hitbox> lista_hitboxes;

//CONTADOR FRAMES

int currentFrame = 0;
int framesCounter = 0;
int framesSpeed = 3;


#pragma endregion

#pragma region Inits

void InitGrid(vector <Hitbox> *lista_hitboxes) {
    Image tile_map = LoadImage("../../resources/world/Tile_Map.png");
    Color* colors = LoadImageColors(tile_map);
    for (int x = 0; x < 211; x++)
    {
        for (int y = 0; y < 30; y++)
        {
            int index = ((y * 16) * tile_map.width) + (x * 16);

            if (colors[index].r == 255 && colors[index].g == 0 && colors[index].b == 0) {
                Rectangle rec = { x * 32, y * 32, 32, 32 };
                lista_hitboxes->push_back(Hitbox(rec, BLOQUE));
            }
            else if (colors[index].r == 255 && colors[index].g == 255 && colors[index].b == 0) {
                Rectangle rec = { x * 32, y * 32, 32, 32 };

                lista_hitboxes->push_back(Hitbox(rec, BLOQUEMONEDA));
            }
            else if (colors[index].r == 0 && colors[index].g == 255 && colors[index].b == 0) {
                Rectangle rec = { x * 32, y * 32, 32, 32 };

                lista_hitboxes->push_back(Hitbox(rec, TUBERIA));
            }
            else if (colors[index].r == 0 && colors[index].g == 0 && colors[index].b == 255) {
                Rectangle rec = { x * 32, y * 32, 32, 32 };

                lista_hitboxes->push_back(Hitbox(rec));
            }
        }
    }
    
    UnloadImage(tile_map);
    UnloadImageColors(colors);
}

void InitGame(void)
{
    background = LoadTexture(BACKGROUND); //Cargo la textura del background
    spriteSheet = LoadTexture(SPRITESHEET);
    blocksheet = LoadTexture(BLOCKS);
    EnemySpriteSheet = LoadTexture(ENEMIES);
    backgroundInicial = LoadTexture(INICIALPAGE);

    score = 0;
    gameOver = false;

    lista_hitboxes = {
    {{0,414,2208,400}},
    //{{512,288,32,32},1,0},
    //{{640,288,32,32},1,0},
    //{{672,288,32,32},1,0},
    //{{704,288,32,32},1,0},
    //{{736,288,32,32},1,0},
    //{{768,288,32,32},1,0},
    //{{704,160,32,32},1,0},
    //{{896,352,64,64},1,0},
    //{{1216,320,64,94},1,0},
    //{{1472,288,64,126},1,0},
    //{{1824,288,64,126},1,0},
    {{2272,414,500,400}},
    //{{2464,288,32,32},1,0},
    //{{2496,288,32,32},1,0},
    //{{2528,288,32,32},1,0},
    //{{2560,160,32,32},1,0},
    //{{2592,160,32,32},1,0},
    //{{2624,160,32,32},1,0},
    //{{2656,160,32,32},1,0},
    //{{2688,160,32,32},1,0},
    //{{2720,160,32,32},1,0},
    //{{2752,160,32,32},1,0},
    //{{2784,160,32,32},1,0},
    {{2848,414,2048,400}},
    //{{2912,160,32,32},1,0},
    //{{2944,160,32,32},1,0},
    //{{2976,160,32,32},1,0},
    //{{3008,160,32,32},1,0},
    //{{3008,288,32,32},1,0},
    //{{3200,288,32,32},1,0},
    //{{3232,288,32,32},1,0},
    //{{3392,288,32,32},1,0},
    //{{3488,288,32,32},1,0},
    //{{3488,160,32,32},1,0},
    //{{3584,288,32,32},1,0},
    //{{3776,288,32,32},1,0},
    //{{3872,160,32,32},1,0},
    //{{3904,160,32,32},1,0},
    //{{3936,160,32,32},1,0},
    //{{4096,160,32,32},1,0},
    //{{4160,160,32,32},1,0},
    //{{4128,160,32,32},1,0},
    //{{4192,160,32,32},1,0},
    //{{4128,288,32,32},1,0},
    //{{4160,288,32,32},1,0},
    //{{4288,382,32,32},1,0},
    //{{4320,382,32,32},1,0},
    //{{4352,382,32,32},1,0},
    //{{4384,382,32,32},1,0},
    //{{4320,350,32,32},1,0},
    //{{4352,350,32,32},1,0},
    //{{4384,350,32,32},1,0},
    //{{4352,318,32,32},1,0},
    //{{4384,318,32,32},1,0},
    //{{4384,286,32,32},1,0},
    //{{4480,286,32,32},1,0},
    //{{4480,318,32,32},1,0},
    //{{4512,318,32,32},1,0},
    //{{4480,350,32,32},1,0},
    //{{4512,350,32,32},1,0},
    //{{4544,350,32,32},1,0},
    //{{4480,382,32,32},1,0},
    //{{4512,382,32,32},1,0},
    //{{4544,382,32,32},1,0},
    //{{4576,382,32,32},1,0},
    //{{4736,382,32,32},1,0},
    //{{4768,382,32,32},1,0},
    //{{4800,382,32,32},1,0},
    //{{4832,382,32,32},1,0},
    //{{4864,382,32,32},1,0},
    //{{4768,350,32,32},1,0},
    //{{4800,350,32,32},1,0},
    //{{4832,350,32,32},1,0},
    //{{4864,350,32,32},1,0},
    //{{4800,318,32,32},1,0},
    //{{4832,318,32,32},1,0},
    //{{4864,318,32,32},1,0},
    //{{4832,286,32,32},1,0},
    //{{4864,286,32,32},1,0},
    {{4960,414,2000,400}},
    //{{4960,382,32,32},1,0},
    //{{4992,382,32,32},1,0},
    //{{5024,382,32,32},1,0},
    //{{5056,382,32,32},1,0},
    //{{4960,350,32,32},1,0},
    //{{4992,350,32,32},1,0},
    //{{5024,350,32,32},1,0},
    //{{4960,318,32,32},1,0},
    //{{4992,318,32,32},1,0},
    //{{4960,286,32,32},1,0},
    //{{5216,352,64,64},1,0},
    //{{5376,288,32,32},1,0},
    //{{5408,288,32,32},1,0},
    //{{5440,288,32,32},1,0},
    //{{5472,288,32,32},1,0},
    //{{5728,352,64,64},1,0},
    {{5792,382,288,32}},
    {{5824,350,256,32}},
    {{5856,318,224,32}},
    {{5888,288,192,32}},
    {{5920,256,160,32}},
    {{5952,224,128,32}},
    //{{5984,192,64,32},1,0},
    //{{6016,160,64,32},1,0},
    //{{6336,382,32,32},1,0},
    {{6348,80,8,302}}
    //CREO QUE NO HAY MAS HITBOXES (SOLO EL BLOQUE INVISIBLE)
    };
    
    InitGrid(&lista_hitboxes);

    InitAudioDevice(); // https://www.raylib.com/examples/audio/loader.html?name=audio_music_stream
    //MUSICA
    music = LoadMusicStream("../../resources/Super Mario Bros Music/overworld-theme-super-mario-world-made-with-Voicemod.wav");
    Gameover = LoadMusicStream("../../resources/Super Mario Bros Music/Game Over.wav");
   
    
    //sonidos
     Pause = LoadSound("../../resources/Super Mario Bros Efects/Pause.wav");
     Die = LoadSound("../../resources/Super Mario Bros Efects/Die.wav");
     JumpSound = LoadSound("../../resources/Super Mario Bros Efects/Jump.wav");
     UpSound = LoadSound("../../resources/Super Mario Bros Efects/1 up.wav");
     BeepSound = LoadSound("../../resources/Super Mario Bros Efects/Beep.wav");
     BigJumpSound = LoadSound("../../resources/Super Mario Bros Efects/Big Jump.wav");
     Break = LoadSound("../../resources/Super Mario Bros Efects/Break.wav");
     Bump = LoadSound("../../resources/Super Mario Bros Efects/Bump.wav");
     Coin = LoadSound("../../resources/Super Mario Bros Efects/Coin.wav");
     FireBall = LoadSound("../../resources/Super Mario Bros Efects/Fire Ball.wav");
     Flagpole = LoadSound("../../resources/Super Mario Bros Efects/Flagpole.wav");
     item = LoadSound("../../resources/Super Mario Bros Efects/Item.wav");
     kick = LoadSound("../../resources/Super Mario Bros Efects/Kick.wav");
     Powerup = LoadSound("../../resources/Super Mario Bros Efects/Powerup.wav");
     Skid = LoadSound("../../resources/Super Mario Bros Efects/Skid.wav");
     Squish = LoadSound("../../resources/Super Mario Bros Efects/Squish.wav");
     Thwomp = LoadSound("../../resources/Super Mario Bros Efects/Tjwomp.wav");
     Vine = LoadSound("../../resources/Super Mario Bros Efects/Vine.wav");
     Warp = LoadSound("../../resources/Super Mario Bros Efects/Wa`rp.wav");
     Enemyfire = LoadSound("../../resources/Super Mario Bros Efects/Enemy Fire.wav");

   /* PlayMusicStream(music);*/

    float timePlayed = 0.0f;
    bool pause = false;

}

void Reset(Mario* mario) {
    gameState = INTRO;
    StopMusicStream(music); 
    PlayMusicStream(music); 
    mario->SetX(316);
    mario->SetY(382);
    mario->velocidad = 0;
    mario->velocidadX = 0;
    camera.target = { mario->position.x + 20.0f, mario->position.y };
    camera.offset = { mario->position.x, mario->position.y };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    tiempo = 400;

    introTimer = 3.5f; 
}


void UnloadGame(void)
{
    UnloadTexture(background);
    UnloadTexture(spriteSheet);
    UnloadTexture(blocksheet);
    UnloadTexture(EnemySpriteSheet);
    UnloadTexture(backgroundInicial);

    UnloadSound(Pause);
    UnloadSound(Die);
    UnloadSound(JumpSound);
    UnloadSound(UpSound);
    UnloadSound(BeepSound);
    UnloadSound(BigJumpSound);
    UnloadSound(Break);
    UnloadSound(Bump);
    UnloadSound(Coin);
    UnloadSound(FireBall);
    UnloadSound(Flagpole);
    UnloadSound(item);
    UnloadSound(kick);
    UnloadSound(Powerup);
    UnloadSound(Skid);
    UnloadSound(Squish);
    UnloadSound(Thwomp);
    UnloadSound(Vine);
    UnloadSound(Warp);
    UnloadSound(Enemyfire);
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
    DrawText(TextFormat("%06i", score), 20, 30, 20, WHITE);
    DrawText("WORLD", screenWidth / 2 - 40, 10, 20, WHITE);
    DrawText(TextFormat("%i - %i", world, level), screenWidth / 2 - 25, 30, 20, WHITE);
   
    DrawText("x", screenWidth / 2 - 20, screenHeight / 2, 20, WHITE);
    DrawText(TextFormat("%i", vidas), screenWidth / 2 + 10, screenHeight / 2, 20, WHITE);
   
    
    Rectangle marioRecorte = { 0, 8, 16, 16 };
    Rectangle marioResized = { screenWidth / 2 - 50, screenHeight / 2, marioRecorte.width * 2, marioRecorte.height * 2 };
    DrawTexturePro(spriteSheet, marioRecorte, marioResized, { 0, 0 }, 0, WHITE);
    EndDrawing();
}

void DrawGame(Mario* mario, vector<Goomba>& goombas, vector<Hitbox> hitboxes)
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

    Rectangle marioResized = { marioIni.x, marioIni.y, 16 * 2, 16 * 2 }; // Escalado
    Vector2 MarioOrigen = { 0,0 };

    DrawTexturePro(spriteSheet, marioRecorte, marioResized, MarioOrigen, 0, WHITE);

    //Hitbox de mario
    DrawRectangleLines(marioResized.x, marioResized.y, marioResized.width, marioResized.height, WHITE);
    DrawRectangleLines(mario->pies.x, mario->pies.y, mario->pies.width, mario->pies.height, RED);
    DrawRectangleLines(mario->cabeza.x, mario->cabeza.y, mario->cabeza.width, mario->cabeza.height, BLUE);
    DrawRectangleLines(mario->derecha.x, mario->derecha.y, mario->derecha.width, mario->derecha.height, GREEN);
    DrawRectangleLines(mario->izquierda.x, mario->izquierda.y, mario->izquierda.width, mario->izquierda.height, YELLOW);

    //Goomba


    //Vector2 PosGoomba1 = { goomba1->position.x,goomba1->position.y };
    //Rectangle Goomba1Recorte = { goomba1->sprite_status, 17, 16,16 };

    //Rectangle Goomba1Resized = { PosGoomba1.x, PosGoomba1.y - 32, 16 * 2, 16 * 2 }; // Escalado
    //Vector2 GoombaOrigen = { 0,0 };
    //DrawTexturePro(EnemySpriteSheet, Goomba1Recorte, Goomba1Resized, GoombaOrigen, 0, WHITE);
    
    for (Goomba& goomba : goombas) 
    {
        goomba.Draw(EnemySpriteSheet);
    }

    //DrawRectangleLines(Goomba1Resized.x, Goomba1Resized.y, Goomba1Resized.width, Goomba1Resized.height, WHITE);

    /* Dibujado de Hitbox */
    for (int i = 0; i < hitboxes.size(); i++) {
        if (hitboxes[i].type == BLOQUE) {
            DrawTexturePro(blocksheet, { 272, 112, 16, 16 }, { hitboxes[i].rect.x, hitboxes[i].rect.y, 32, 32 }, {0,0}, 0, WHITE);
        }
        else if (hitboxes[i].type == BLOQUEMONEDA) {
            DrawTexturePro(blocksheet, { 80, 112, 16, 16 }, { hitboxes[i].rect.x, hitboxes[i].rect.y, 32, 32 }, { 0,0 }, 0, WHITE);
        }

        //DrawRectangleRec(hitboxes[i].rect, hitboxes[i].color);
    }

    EndMode2D();

    /*Hud draw*/
    DrawText("MARIO", 20, 10, 20, WHITE);
    DrawText(TextFormat("%06i", score), 20, 30, 20, WHITE);


    DrawText(TextFormat("x %02i", monedas), 180, 30, 20, YELLOW);

    DrawText("TIME", screenWidth - 100, 10, 20, WHITE);
    DrawText(TextFormat("%03i", tiempo), screenWidth - 100, 30, 20, WHITE);

    DrawText("WORLD", screenWidth / 2 - 40, 10, 20, WHITE);
    DrawText(TextFormat("%i - %i", world, level), screenWidth / 2 - 25, 30, 20, WHITE);

    //Dibujado de Game Over



    EndDrawing();

}

void Drawgameintoscreen() {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("GAME OVER", screenWidth / 2 - 100, screenHeight /2, 40, RED);
    EndDrawing();
}

#pragma endregion

#pragma region FUNCIONES DE UPDATE

void UpdateTimer(float delta) {
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

void UpdateCameraCenter(Camera2D* camera, Mario* mario, Hitbox* envItems, int envItemsLength, float delta, int width, int height)
{
    static Vector2 bbox = { 0.16f, 0.16f };

    Vector2 bboxWorldMin = GetScreenToWorld2D({ (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height }, *camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D({ (1 + bbox.x) * 0.4f * width, (1 + bbox.y) * 0.5f * height }, *camera);
    camera->offset = { (1 - bbox.x) * 0.5f * width, 380 };

    if (mario->position.x > bboxWorldMax.x)
        camera->target.x = bboxWorldMin.x + (mario->position.x - bboxWorldMax.x);
}

void UpdateGameState(float delta) {
    if (gameState == INICIAL) {
       
        if (IsKeyPressed(KEY_DOWN)) selectedOption = 1;
        if (IsKeyPressed(KEY_UP)) selectedOption = 0;
        if (IsKeyPressed(KEY_ENTER)) gameState = INTRO;
    }
    else if (gameState==INTRO) {
        introTimer -= delta; //COMENTADO PARA PROBAR SELECCION PERSONAJE
        if (introTimer <= 0 || IsKeyPressed(KEY_ENTER)) {
            gameState = PLAYING;
            if (!musicPlaying) {
                PlayMusicStream(music);
                musicPlaying = true;
            }
        }
    }
}

void UpdateGame(Mario* mario, vector<Goomba>& goombas, Hitbox* hitboxes, float delta, int envItems)
{
    if (!gameOver)
    {
        //colisions

        bool hitGround = false;

        for (int i = 0; i < envItems; i++)
        {
            Hitbox* ei = hitboxes + i;

            if (CheckCollisionRecs(ei->rect, mario->pies)) {
                hitGround = true;
                mario->velocidad = 0.0f;
                mario->SetY(ei->rect.y - mario->position.height + 2);
            }
            if (CheckCollisionRecs(ei->rect, mario->cabeza) && !hitGround) {
                mario->velocidad = 0.0f;
                mario->SetY(ei->rect.y + ei->rect.height + 2);
                if (ei->type == BLOQUE) {
                    if (mario->poder == BASE) {
                        PlaySound(Bump);

                    }
                    else {
                        PlaySound(Break);
                        lista_hitboxes.erase(lista_hitboxes.begin() + i);
                    }
                }

            }
            if (CheckCollisionRecs(ei->rect, mario->derecha)) {
                mario->SetX(ei->rect.x - mario->position.width - 2);
            }
            if (CheckCollisionRecs(ei->rect, mario->izquierda)) {
                mario->SetX(ei->rect.x + ei->rect.width + 2);
            }
        }

        /* Colision con enemigos */
        
        for (Goomba& goomba : goombas) 
        {
            /*if (CheckCollisionRecs(mario->position, goomba.position))*/
            if (!mario->isDead && CheckCollisionRecs(mario->position, goomba.position))
            {

                mario->isDead = true;
                mario->deathAnimationInProgress = true;
                mario->velocidad = mario->deathVelocity;
                StopMusicStream(music);
                for (int i = 0; i < goombas.size(); ++i) {
                    goombas[i].reset();
                }
                return;
               /* gameOver = true;
                StopMusicStream(music);
                for (int i = 0; i < goombas.size(); ++i) {
                    goombas[i].reset();
                }*/
             

                if (!playDeathSound) {

                    vidas--;
                    StopMusicStream(music);
                    PlaySound(Die);
                    musicPlaying = false;

                    if (vidas <= 0 && !gameovermusicplayed)
                    {
                        StopMusicStream(music);
                        StopMusicStream(Gameover);
                        PlayMusicStream(Gameover);
                        gameovermusicplayed = true;
                        gameState = GAME_OVER;
                    }
                    else
                    {
                        PlaySound(Die);
                    }

                    playDeathSound = true;
                    musicRestarted = false;
                }
                break;
            }
        }

        /* Movimiento de Mario */
        if (IsKeyDown(KEY_RIGHT)) {
            if (mario->animTimer >= 0.01) //el 0.02f cambia la vel
            {
                mario->animTimer += delta;
                mario->mirando_derecha = true;
                
                mario->velocidadX += ACELERACION;
                if (mario->velocidadX > VELOCIDAD_MAXIMA)
                    mario->velocidadX = VELOCIDAD_MAXIMA;
                
                if (mario->canJump) {
                    if (mario->sprite_status >= 56)
                        mario->sprite_status = 20;
                    else
                        mario->sprite_status += 18;
                }
                mario->animTimer = 0.0f;
            }
            mario->animTimer += delta;
        }
        else if (IsKeyDown(KEY_LEFT)) {
           
            if (mario->position.x >= (GetScreenToWorld2D({ (1 - 0.16f) * 0.5f * screenWidth, (1 - 0.16f) * 0.5f * screenHeight }, camera).x) - 325 && mario->animTimer >= 0.01f) {
                mario->animTimer = delta;
                mario->mirando_derecha = false;
                
                mario->velocidadX -= ACELERACION;
                if (mario->velocidadX < -VELOCIDAD_MAXIMA)
                    mario->velocidadX = -VELOCIDAD_MAXIMA;
                if (mario->canJump) {
                    if (mario->sprite_status >= 56)
                        mario->sprite_status = 20;
                    else
                        mario->sprite_status += 18;
                }

                if (mario->position.x >= (screenWidth / 2) - 12) {
                    worldPosition = mario->position.x;
                }
            }
            else {
                mario->velocidadX = 0;
            }

            mario->animTimer += delta;
        }
        else {
            // Desaceleración natural (fricción)
            if (mario->velocidadX > 0) {
                mario->velocidadX -= ACELERACION;
                if (mario->velocidadX < 0) mario->velocidadX = 0;
            }
            else if (mario->velocidadX < 0) {
                mario->velocidadX += ACELERACION;
                if (mario->velocidadX > 0) mario->velocidadX = 0;
            }
        }

        // Aplicar movimiento horizontal 
        mario->MoveX(mario->velocidadX);
       
        cout << mario->position.x << endl;

        /* Salto */
        if (IsKeyDown(KEY_SPACE) && mario->canJump) {
            hitGround = false;
            mario->velocidad = -PLAYER_JUMP_SPD;
            mario->canJump = false;
            mario->sprite_status = 96;
            PlaySound(JumpSound);
        }

        UpdateCameraCenter(&camera, mario, hitboxes, 1, delta, screenWidth, screenHeight);

        if (!hitGround)
        {
            mario->MoveY(mario->velocidad * delta);
            mario->velocidad += GRAVEDAD * delta;
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
        if (vidas <= 0) 
        {
            gameState = GAME_OVER;

            if (!gameovermusicplayed) {
                StopMusicStream(music);
                PlayMusicStream(Gameover);
                gameovermusicplayed = true;
            }
        }
        else
        {
            if (vidas <= 0)
            {
                gameState = GAME_OVER;

            }
            else
            {
                Reset(mario);
                gameOver = false;
                if (!musicRestarted) {
                    StopMusicStream(music);
                   /* PlayMusicStream(music);*/   // Vuelve a empezar desde el inicio
                    musicRestarted = true;
                }

                playDeathSound = false; // Resetea para futuras muertes
            }
        }
    }


    /* Si Mario llega al final o se cae */
    if (mario->position.y > 580 || mario->position.x >= 6336) {
        gameOver = true;
        StopMusicStream(music);  //hugo

        if (!playDeathSound) {

            vidas--;
            PlaySound(Die);
            StopMusicStream(music);
            musicPlaying = false;
            for (int i = 0; i < goombas.size(); ++i) {
                goombas[i].reset();
            }

            if (vidas <= 0 && !gameovermusicplayed)
            {
                StopMusicStream(music);
                StopMusicStream(Gameover);
                PlayMusicStream(Gameover);
                gameovermusicplayed = true;
                gameState = GAME_OVER;
            }
            else 
            {
                PlaySound(Die);
            }


            playDeathSound = true;
            musicRestarted = false;
        }
    }
}

#pragma endregion

int main(void)
{
    InitWindow(screenWidth, screenHeight, "classic game: Super Mario Bros.");

    InitGame();

    Mario mario(316, 382); //Creo el objeto de Mario
    vector<Goomba> goombas = {
    
    Goomba(500, 416),
    Goomba(700, 416),
    Goomba(1100, 416),
    Goomba(1800, 416)

    };

    camera.target = { mario.position.x + 20.0f, mario.position.y };
    camera.offset = { mario.position.x, mario.position.y };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        UpdateMusicStream(music); //HUGO
        UpdateMusicStream(Gameover);

        float deltaTime = GetFrameTime();

        UpdateGameState(deltaTime);

        ClearBackground(RAYWHITE);


        if (gameState == INTRO) {
            DrawIntroScreen();
         
        }
        else if (gameState == GAME_OVER) {
            if (!gameovermusicplayed)
            { 
                StopMusicStream(music);
                PlayMusicStream(Gameover);
                gameovermusicplayed = true;
                Drawgameintoscreen(); 

            }
            
            if (IsKeyPressed(KEY_ENTER)) {
                vidas = 3;
                score = 0;
                monedas = 0;
                worldPosition = 0;

                StopMusicStream(music);
                PlayMusicStream(Gameover);

                mario = Mario(316, 414);
                gameState = INICIAL;

                playDeathSound = false;
                musicRestarted = false;
                gameovermusicplayed = false;
            }
        }
        else if (gameState == INICIAL) {
            DrawIntro();
         
        }
        else {
            for (Goomba& goomba : goombas) goomba.Update(deltaTime);
            UpdateGame(&mario, goombas, &lista_hitboxes[0], deltaTime, lista_hitboxes.size());
            DrawGame(&mario, goombas, lista_hitboxes);
        }

        if (vidas <= 0) {
            Drawgameintoscreen();
        }
    }


    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(music);
    UnloadMusicStream(Gameover);
    CloseAudioDevice();
    UnloadGame();         // Unload loaded data (textures, sounds, models...)
    CloseWindow();        // Close window and OpenGL context
    //------------------------------------------------------------------------------
    // --------

    return 0;
}
