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
#include "Goomba.h"
#include "Koopa.h"
#include "Mushroom.h"
#include "Moneda.h"

using namespace std;
namespace fs = filesystem;

#pragma region DEFINES y ENUMS

#define BACKGROUND  "../../resources/world/Empty_World_1_1.png"
#define SPRITESHEET "../../resources/sprites/NES - Super Mario Bros - Mario & Luigi.png"
#define ENEMIES     "../../resources/sprites/NES - Super Mario Bros - Enemies & Bosses.png"
#define BLOCKS      "../../resources/sprites/NES - Super Mario Bros - Item and Brick Blocks.png"
#define ITEMS       "../../resources/sprites/NES - Super Mario Bros - Items Objects and NPCs.png"
#define SOUNDS      "../../resources/Super Mario Bros Efects"
#define MUSICS      "../../resources/Super Mario Bros Music"
#define INICIALPAGE "../../resources/NES - Super Mario Bros - Title Screen HUD and Miscellaneous (1).png"
#define TILEMAP     "../../resources/world/Tile_Map.png"

#define BACKGROUND2  "resources/world/Empty_World_1_1.png"
#define SPRITESHEET2 "resources/sprites/NES - Super Mario Bros - Mario & Luigi.png"
#define ENEMIES2     "resources/sprites/NES - Super Mario Bros - Enemies & Bosses.png"
#define BLOCKS2      "resources/sprites/NES - Super Mario Bros - Item and Brick Blocks.png"
#define ITEMS2       "resources/sprites/NES - Super Mario Bros - Items Objects and NPCs.png"
#define SOUNDS2      "resources/Super Mario Bros Efects"
#define MUSICS2      "resources/Super Mario Bros Music"
#define INICIALPAGE2 "resources/NES - Super Mario Bros - Title Screen HUD and Miscellaneous (1).png"
#define TILEMAP2     "resources/world/Tile_Map.png"

#define PLAYER_JUMP_SPD 420.0f
#define GRAVEDAD 1000
#define ACELERACION 0.5f
#define VELOCIDAD_MAXIMA 5.0f
#define ANIM_FRAME_MIN 0.03f
#define ANIM_FRAME_MAX 1.8f

enum GameState {
    INICIAL,
    INTRO,
    PLAYING,
    GAME_OVER,
    LEVEL_COMPLETED
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
    TUBERIA = 2,
    BANDERA = 3,
    VACIO = 4
};

#pragma endregion


struct Hitbox {
    Rectangle rect;
    BlockType type;
    int sprite_status = 0;

    Hitbox(Rectangle r) {
        rect = r;
        type = VACIO;
    }

    Hitbox(Rectangle r, BlockType t) {
        rect = r;
        type = t;
    }
};

#pragma region VARIABLES GLOBALES

static Music music; //HUGO 
static Music Gameover;
static Music Outoftime;
static Music StageClear;


bool playDeathSound = false;
bool musicRestarted = false;
bool musicPlaying = false;
bool waitingforgameover = false;
bool diesoundplayed = false;

float gameoverwaitimer = 0.0f;
bool flagpoleSoundPlayed = false;
bool stageClearMusicStarted = false;
bool stageClearMusicFinished = false;
float stageClearTimer = 0.0f;

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
static float monedaTimer = 0.0f;

static bool gameOver = false;
static bool gameovermusicplayed = false;
static int score = 0;
static float worldPosition = 0;
static int selectedOption = 0;  // 0 para 1 jugador, 1 para 2 jugadores

Camera2D camera = { 0 };
Texture2D background;
Texture2D spriteSheet;
Texture2D blocksheet;
Texture2D itemsheet;
Texture2D EnemySpriteSheet;
Texture2D backgroundInicial;

static int tiempo = 400;   // Tiempo en cuenta regresiva
static int monedas = 0;    // Contador de monedas
static int vidas = 3;      // N�mero de vidas de Mario
static int world = 1;
static int level = 1;
vector <Hitbox> lista_hitboxes;
vector <Mushroom> lista_setas;
vector <Moneda> lista_monedas;

//CONTADOR FRAMES

int currentFrame = 0;
int framesCounter = 0;
int framesSpeed = 3;


#pragma endregion

#pragma region Inits

void InitGrid(vector<Hitbox>* lista_hitboxes) {
    Image tile_map = LoadImage(TILEMAP);
    if (tile_map.height == 0)
        tile_map = LoadImage(TILEMAP2);

    Color* colors = LoadImageColors(tile_map);

    if (colors == nullptr) {
        cout << "Error cargando los colores del mapa." << endl;
        return;
    }

    for (int x = 0; x < 211; x++) {
        for (int y = 0; y < 30; y++) {
            int index = ((y * 16) * tile_map.width) + (x * 16);

            Rectangle rec = { x * 32, y * 32, 32, 32 };

            if (colors[index].r == 255 && colors[index].g == 0 && colors[index].b == 0) {
                lista_hitboxes->push_back(Hitbox(rec, BLOQUE));
            }
            else if (colors[index].r == 255 && colors[index].g == 255 && colors[index].b == 0) {
                lista_hitboxes->push_back(Hitbox(rec, BLOQUEMONEDA));
            }
            else if (colors[index].r == 0 && colors[index].g == 255 && colors[index].b == 0) {
                lista_hitboxes->push_back(Hitbox(rec, TUBERIA));
            }
            else if (colors[index].r == 255 && colors[index].g == 0 && colors[index].b == 255) {
                lista_hitboxes->push_back(Hitbox(rec, BANDERA));
            }
            else if (colors[index].r == 0 && colors[index].g == 0 && colors[index].b == 255) {
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
    
    if (background.height == 0) { //Si falla en cargar, usa el otro path
        background = LoadTexture(BACKGROUND2); 
        spriteSheet = LoadTexture(SPRITESHEET2);
        blocksheet = LoadTexture(BLOCKS2);
        itemsheet = LoadTexture(ITEMS2);
        EnemySpriteSheet = LoadTexture(ENEMIES2);
        backgroundInicial = LoadTexture(INICIALPAGE2);
    }
    else {
        spriteSheet = LoadTexture(SPRITESHEET);
        blocksheet = LoadTexture(BLOCKS);
        itemsheet = LoadTexture(ITEMS);
        EnemySpriteSheet = LoadTexture(ENEMIES);
        backgroundInicial = LoadTexture(INICIALPAGE);
    }

    score = 0;
    gameOver = false;

    lista_hitboxes = {
        {{0,414,2208,400}},
        {{2272,414,500,400}},
        {{2848,414,2048,400}},
        {{4960,414,2000,400}}
    };
    
    //Crea los bloques
    InitGrid(&lista_hitboxes);

    //Musica
    InitAudioDevice(); // https://www.raylib.com/examples/audio/loader.html?name=audio_music_stream
    
    music = LoadMusicStream("resources/Super Mario Bros Music/01. Ground Theme.mp3");
    if (music.stream.buffer == 0) {
        music = LoadMusicStream("../../resources/Super Mario Bros Music/01. Ground Theme.mp3");
        Pause = LoadSound("../../resources/Super Mario Bros Efects/Pause.wav");
        Die = LoadSound("../../resources/Super Mario Bros Efects/Die.wav");
        JumpSound = LoadSound("../../resources/Super Mario Bros Efects/Jump.wav");
        UpSound = LoadSound("../../resources/Super Mario Bros Efects/1up.wav");
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
        Thwomp = LoadSound("../../resources/Super Mario Bros Efects/Thwomp.wav");
        Vine = LoadSound("../../resources/Super Mario Bros Efects/Vine.wav");
        Warp = LoadSound("../../resources/Super Mario Bros Efects/Warp.wav");
        Enemyfire = LoadSound("../../resources/Super Mario Bros Efects/Enemy Fire.wav");
        Gameover = LoadMusicStream("../../resources/Super Mario Bros Music/Game Over.wav");
        StageClear = LoadMusicStream("../../resources/Super Mario Bros Music/Stage-Clear.wav");
        StageClear.looping = false;
        Outoftime = LoadMusicStream("../../resources/Super Mario Bros Music/Out-of-time.wav");
        Gameover.looping = false;
    }
    else {
        Pause = LoadSound("resources/Super Mario Bros Efects/Pause.wav");
        Die = LoadSound("resources/Super Mario Bros Efects/Die.wav");
        JumpSound = LoadSound("resources/Super Mario Bros Efects/Jump.wav");
        UpSound = LoadSound("resources/Super Mario Bros Efects/1up.wav");
        BeepSound = LoadSound("resources/Super Mario Bros Efects/Beep.wav");
        BigJumpSound = LoadSound("resources/Super Mario Bros Efects/Big Jump.wav");
        Break = LoadSound("resources/Super Mario Bros Efects/Break.wav");
        Bump = LoadSound("resources/Super Mario Bros Efects/Bump.wav");
        Coin = LoadSound("resources/Super Mario Bros Efects/Coin.wav");
        FireBall = LoadSound("resources/Super Mario Bros Efects/Fire Ball.wav");
        Flagpole = LoadSound("resources/Super Mario Bros Efects/Flagpole.wav");
        item = LoadSound("resources/Super Mario Bros Efects/Item.wav");
        kick = LoadSound("resources/Super Mario Bros Efects/Kick.wav");
        Powerup = LoadSound("resources/Super Mario Bros Efects/Powerup.wav");
        Skid = LoadSound("resources/Super Mario Bros Efects/Skid.wav");
        Squish = LoadSound("resources/Super Mario Bros Efects/Squish.wav");
        Thwomp = LoadSound("resources/Super Mario Bros Efects/Thwomp.wav");
        Vine = LoadSound("resources/Super Mario Bros Efects/Vine.wav");
        Warp = LoadSound("resources/Super Mario Bros Efects/Warp.wav");
        Enemyfire = LoadSound("resources/Super Mario Bros Efects/Enemy Fire.wav");
        Gameover = LoadMusicStream("resources/Super Mario Bros Music/Game Over.wav");
        StageClear = LoadMusicStream("../../resources/Super Mario Bros Music/Stage-Clear.wav");
        StageClear.looping = false;
        Outoftime = LoadMusicStream("../../resources/Super Mario Bros Music/Out-of-time.wav");
        Gameover.looping = false;
    }
     
    float timePlayed = 0.0f;
    bool pause = false;

}

void Reset(Mario* mario, vector<Goomba>& goombas, vector<Koopa>& koopas) {
    gameState = INTRO;

    StopMusicStream(music);              // Detiene la música actual
    musicPlaying = false;               // Marca que aún NO se debe reproducir
    playDeathSound = false;              // Permite reproducir "Die" la próxima vez
    flagpoleSoundPlayed = false;        // 🔁 Reinicio de sistema de bandera
    stageClearMusicStarted = false;
    stageClearMusicFinished = false;
    stageClearTimer = 0.0f;

    mario->SetX(316);
    mario->SetY(382);
    mario->poder = Mario::BASE;
    mario->position.height = 32;
    mario->velocidad = 0;
    mario->velocidadX = 0;
    mario->isDead = false;
    mario->deathAnimationInProgress = false;
    mario->deathAnimTimer = 0.0f;
    mario->estado = Mario::NORMAL;

    camera.target = { mario->position.x + 20.0f, mario->position.y };
    camera.offset = { mario->position.x, mario->position.y };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    tiempo = 400;
    introTimer = 3.5f;               // Se muestra pantalla de vidas durante 3.5 segundos

    //ENEMIGOS RESET
    for (Goomba& g : goombas) g.reset();
    for (Koopa& g : koopas) g.reset();

    lista_hitboxes.clear();

    lista_hitboxes = {
        {{0,414,2208,400}},
        {{2272,414,500,400}},
        {{2848,414,2048,400}},
        {{4960,414,2000,400}}
    };

    InitGrid(&lista_hitboxes);
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

void DrawLevelCompletedScreen() {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("LEVEL COMPLETED!", screenWidth / 2 - 120, screenHeight / 2 - 20, 30, GREEN);
    DrawText("Pulsa ENTER para volver al inicio", screenWidth / 2 - 150, screenHeight / 2 + 30, 20, WHITE);
    EndDrawing();
}

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

void DrawGame(Mario* mario, vector<Goomba>& goombas, vector <Koopa>& koopas, vector<Hitbox> hitboxes)
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
    
    if (mario->estado == Mario::TRANSFORMANDOSE) {
        if (mario->transform_timer == 4)
            mario->transform_timer = 0;

        if (mario->transform_timer == 0) {
            
            float x;
            float y;
            float height;
            if (mario->poder == Mario::BASE) {
                x = mario->transform_secuence[mario->transform_status];

                if (x == 0) {
                    y = 8;
                    height = 16;
                }
                else if (x == 18) {
                    y = 80;
                    height = 24;
                    marioIni.y = mario->position.y - 16;
                }
                else if (x == 36) {
                    y = 72;
                    height = 32;
                    marioIni.y = mario->position.y - 32;
                }

                marioRecorte = { x, y, 16, height };

                if (mario->transform_status < 9) {
                    mario->transform_status++;
                }
                else {
                    mario->transform_status = 0;
                    mario->poder = Mario::SETA;
                    mario->position.height = 64;
                    mario->velocidad = 0;
                    mario->estado = Mario::NORMAL;
                    mario->SetY(marioIni.y);
                    mario->transform_timer--;
                }
            }
            else {
                x = mario->inverse_transform_secuence[mario->transform_status];

                if (x == 0) {
                    y = 8;
                    height = 16;
                }
                else if (x == 18) {
                    y = 80;
                    height = 24;
                    marioIni.y = mario->position.y - 16;
                }
                else if (x == 36) {
                    y = 72;
                    height = 32;
                    marioIni.y = mario->position.y - 32;
                }

                marioRecorte = { x, y, 16, height };

                if (mario->transform_status < 9) {
                    mario->transform_status++;
                }
                else {
                    mario->transform_status = 0;
                    mario->poder = Mario::BASE;
                    mario->position.height = 32;
                    mario->velocidad = 0;
                    mario->estado = Mario::NORMAL;
                    mario->SetY(marioIni.y);
                    mario->transform_timer--;
                }
            }
        }

        mario->transform_timer++;

    }

    //*Cambio de derecha a izquierda
    if (mario->mirando_derecha)
    {
        marioRecorte.width = 16;
    }
    else
    {
        marioRecorte.width = -16;
    }

    Rectangle marioResized = { marioIni.x, marioIni.y, 32, 32 }; // Escalado
    
    if (mario->poder == Mario::SETA) {
        marioRecorte.y = 32;
        marioRecorte.height = 32;

        marioResized.height = 64;
        //mario->SetY(marioIni.y - 32);

    }

    Vector2 MarioOrigen = { 0,0 };

    DrawTexturePro(spriteSheet, marioRecorte, marioResized, MarioOrigen, 0, WHITE);

    //Hitbox de mario
    //DrawRectangleLines(marioResized.x, marioResized.y, marioResized.width, marioResized.height, WHITE);
    //DrawRectangleLines(mario->pies.x, mario->pies.y, mario->pies.width, mario->pies.height, RED);
    //DrawRectangleLines(mario->cabeza.x, mario->cabeza.y, mario->cabeza.width, mario->cabeza.height, BLUE);
    //DrawRectangleLines(mario->derecha.x, mario->derecha.y, mario->derecha.width, mario->derecha.height, GREEN);
    //DrawRectangleLines(mario->izquierda.x, mario->izquierda.y, mario->izquierda.width, mario->izquierda.height, YELLOW);

    //Goomba

    for (Goomba& goomba : goombas) 
    {
        goomba.Draw(EnemySpriteSheet);
    }

    for (Koopa& koopa : koopas)
    {
        koopa.Draw(EnemySpriteSheet);
    }

    for (Mushroom& seta : lista_setas)
    {
        seta.Draw(itemsheet);
    }

    for (Moneda& coin : lista_monedas)
    {
        coin.Draw(itemsheet);
    }

    for (int i = 0; i < lista_monedas.size(); i++)
    {
        Moneda coin = lista_monedas[i];
        if (lista_monedas[i].transform_status >= 3) {
            lista_monedas.erase(lista_monedas.begin() + i);
        }
    }


    //DrawRectangleLines(Goomba1Resized.x, Goomba1Resized.y, Goomba1Resized.width, Goomba1Resized.height, WHITE);

    /* Dibujado de Hitbox */
    for (int i = 0; i < hitboxes.size(); i++) {
        if (hitboxes[i].type == BLOQUE) {
            DrawTexturePro(blocksheet, { 272, 112, 16, 16 }, { hitboxes[i].rect.x, hitboxes[i].rect.y, 32, 32 }, {0,0}, 0, WHITE);
        }
        else if (hitboxes[i].type == BLOQUEMONEDA) {
            if (hitboxes[i].sprite_status == 0)
                DrawTexturePro(blocksheet, { 80, 112, 16, 16 }, { hitboxes[i].rect.x, hitboxes[i].rect.y, 32, 32 }, { 0,0 }, 0, WHITE);
            else
                DrawTexturePro(blocksheet, { 128, 112, 16, 16 }, { hitboxes[i].rect.x, hitboxes[i].rect.y, 32, 32 }, { 0,0 }, 0, WHITE);

        }
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
    for (Hitbox& h : hitboxes) {
        if (h.type == BANDERA) DrawRectangleLinesEx(h.rect, 2, PURPLE); // ZONA MOARADA A TOCAR AL FINAL
    }

    DrawText(TextFormat("Estado Mario: %d", mario->estado), 10, 60, 20, RED); // DESCOMENTAR PARA MIRAR ESTADO DE MARIO

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

void UpdateGame(Mario* mario, vector<Goomba>& goombas, vector<Koopa>& koopas, Hitbox* hitboxes, float delta, int envItems)
{
    
    if (mario->estado == Mario::TOCANDO_BANDERA) {
            mario->MoveY(60 * delta);            // Mario baja lentamente
            mario->sprite_status = 136;           // Sprite de bajada
            StopMusicStream(music);

            if (mario->poder == BASE) {
                if (mario->position.y >= 382) {
                    mario->position.y = 382;
                    mario->estado = Mario::CAMINANDO_CASTILLO;
                }
            }
            else {
                if (mario->position.y >= 350) {
                    mario->position.y = 350;
                    mario->estado = Mario::CAMINANDO_CASTILLO;
                }
            }


            return;
    }
    if (mario->estado == Mario::CAMINANDO_CASTILLO) {
        mario->MoveX(60 * delta); 
        // Mario camina hacia el castillo
        mario->animTimer += delta;
        if (mario->animTimer >= 0.15f) { // Aumenta este valor para hacer más lenta la animación
            mario->sprite_status += 18;
            if (mario->sprite_status > 56)
                mario->sprite_status = 20;
            mario->animTimer = 0.0f;
        }

        // Inicia Stage Clear solo una vez
        if (!stageClearMusicStarted) {
            PlayMusicStream(StageClear);
            stageClearMusicStarted = true;
        }

        return;
    }
    if (mario->estado == Mario::TRANSFORMANDOSE) {
        return;
    }

     
    if (waitingforgameover) {
        gameoverwaitimer += delta;

        if (!diesoundplayed) {
            PlaySound(Die);
            diesoundplayed = true;
        }
        if (gameoverwaitimer >= 5.0f) {
            gameState = GAME_OVER;
            waitingforgameover = false;
        }

        // --- BAJADA POR LA BANDERA ---
        if (mario->estado == Mario::TOCANDO_BANDERA) {
            mario->MoveY(60 * delta); // Baja lentamente
            mario->sprite_status = 136; // Sprite quieto o de bajada

            if (mario->position.y >= 384) {  // Punto donde termina de bajar
                mario->position.y = 384;
                mario->estado = Mario::CAMINANDO_CASTILLO;
            }

            return; // IMPORTANTE: no procesar nada más mientras baja
        }

     }

    if (!gameOver)
    {
        if (mario->deathAnimationInProgress) {
            mario->deathAnimTimer += delta;
            mario->MoveY(mario->velocidad * delta);
            mario->velocidad += GRAVEDAD * delta;

            if (mario->deathAnimTimer >= 1.5f) {
                vidas--;
                mario->deathAnimationInProgress = false;
                mario->isDead = false;
                mario->deathAnimTimer = 0.0f;
                gameOver = true;  // Esto activa el reinicio desde el bloque inferior
                for (Goomba& g : goombas) g.reset();
                for (Koopa& g : koopas) g.reset();
                // Reiniciar enemigos
                for (Goomba& g : goombas) g.reset();
                for (Koopa& g : koopas) g.reset();
            }

            return; // IMPORTANTE: Evita que se ejecute el resto del código durante la animación
        }

        //colisions

        bool hitGround = false;
        bool isJumping = false;

        /* Colisiones con bloques */
        for (int i = 0; i < envItems; i++)
        {
            Hitbox* ei = hitboxes + i;

            //Bandera
            if (ei->type == BANDERA) {
                // Sub-hitbox más precisa centrada en Mario para evitar falsos positivos
                Rectangle centroMario = {
                    mario->position.x + 8,  // 8 px de margen lateral
                    mario->position.y,
                    mario->position.width - 16, // reducir 16 px total (8+8)
                    mario->position.height
                };

                if (CheckCollisionRecs(centroMario, ei->rect)) {
                    mario->estado = Mario::TOCANDO_BANDERA;
                    mario->velocidadX = 0;
                    mario->velocidad = 0;
                    mario->canJump = false;
                    mario->isJumping = false;
                    mario->sprite_status = 96;

                    PlaySound(Flagpole);
                    break;
                }
            }

            if (CheckCollisionRecs(ei->rect, mario->pies)) {
                hitGround = true;
                mario->velocidad = 0.0f;

                if (mario->poder == Mario::SETA)
                    cout << endl;

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
                else if (ei->type == BLOQUEMONEDA) {
                    if (ei->rect.x / 32 == 21) {
                        if (ei->sprite_status != 1) {
                            PlaySound(item);
                            ei->sprite_status = 1;
                            score += 1000;
                            lista_setas.push_back(Mushroom(ei->rect.x, ei->rect.y - 32));
                        }
                        else {
                            PlaySound(Bump);
                        }
                    }
                    else {
                        if (ei->sprite_status != 1) {
                            PlaySound(Coin);
                            score += 200;
                            ei->sprite_status = 1;
                            lista_monedas.push_back(Moneda(ei->rect.x, ei->rect.y - 32));
                        }
                        else {
                            PlaySound(Bump);
                        }

                    }
                }
            }

            if (CheckCollisionRecs(ei->rect, mario->derecha)) {

                if (ei->type != BANDERA)
                    mario->SetX(ei->rect.x - mario->position.width - 2);
            }

            if (CheckCollisionRecs(ei->rect, mario->izquierda)) {
                if (ei->type != BANDERA)
                    mario->SetX(ei->rect.x + ei->rect.width + 2);
            }
        }     

        /* Colisiones con enemigos */

        for (Goomba& goomba : goombas)
        {
            if (!goomba.activo) continue;

            for (int i = 0; i < envItems; i++) {
                Hitbox* block = &hitboxes[i];

                // Si choca con la parte derecha de un bloque (esquina izquierda del Goomba)
                if (CheckCollisionRecs(goomba.izquierda, block->rect)) {
                    goomba.mirando_derecha = true;
                    goomba.position.x = block->rect.x + block->rect.width + 1; // separarlo
                }

                // Si choca con la parte izquierda de un bloque (esquina derecha del Goomba)
                if (CheckCollisionRecs(goomba.derecha, block->rect)) {
                    goomba.mirando_derecha = false;
                    goomba.position.x = block->rect.x - goomba.position.width - 1; // separarlo
                }
            }

            // Si Mario pisa al Goomba (solo si viene cayendo)
            if (!mario->isDead && CheckCollisionRecs(mario->pies, goomba.cabeza) && mario->velocidad > 0)
            {
                goomba.activo = false;
                PlaySound(Squish);  // sonido al aplastar
                score += 200;
                mario->velocidad = -PLAYER_JUMP_SPD / 1.5f;  // rebota un poco hacia arriba
                continue;
            }

            // Si Mario colisiona por cualquier otro lado
            if (!mario->isDead && CheckCollisionRecs(mario->position, goomba.position))
            {
                if (mario->poder == Mario::BASE) {
                    mario->isDead = true;
                    mario->sprite_status = 116;

                    mario->deathAnimationInProgress = true;
                    mario->velocidad = mario->deathVelocity;
                    StopMusicStream(music);

                    if (!playDeathSound)
                    {
                        PlaySound(Die);           
                        playDeathSound = true;   
                    }
                }
                else{
                    PlaySound(Warp);
                    mario->estado = Mario::TRANSFORMANDOSE;
                }


                return;
            }
            
        }
        // Si Mario ha muerto, procesamos la animación de muerte
        if (mario->isDead && mario->deathAnimationInProgress) {
            mario->deathAnimTimer += delta;

            if (mario->deathAnimTimer >= 2.0f)
            {
                vidas--;

                if (vidas <= 0)
                { //Esperamos a que termine die
                    waitingforgameover = true;
                    gameoverwaitimer = 0.0f;
                    PlaySound(Die);
                    StopMusicStream(music);

                    return;

                }
                else
                {
                    Reset(mario, goombas, koopas);  // Reinicia nivel y reposiciona a Mario
                    gameOver = false;
                    musicRestarted = false;
                    playDeathSound = false;
                }

                mario->deathAnimationInProgress = false;
                mario->isDead = false;
                mario->deathAnimTimer = 0.0f;

                return;
            }

            // Aplicar movimiento de caída
            mario->MoveY(mario->velocidad * delta);
            mario->velocidad += GRAVEDAD * delta;

            return; // Salir del UpdateGame mientras Mario está muriendo
        }
        if (waitingforgameover) {
            gameoverwaitimer += delta;

            if (gameoverwaitimer >= 3.0f) { // Espera después del sonido Die.wav
                gameState = GAME_OVER;
                waitingforgameover = false;

                if (!gameovermusicplayed) {
                    StopMusicStream(music);
                    PlayMusicStream(Gameover);
                    gameovermusicplayed = true;
                }
            }
            return;
        }

        for (Koopa& koopa : koopas)
        {
            if (!koopa.activo) continue;

            for (int i = 0; i < envItems; i++) {
                Hitbox* block = &hitboxes[i];

                // Si choca con la parte derecha de un bloque (esquina izquierda del Koopa)
                if (CheckCollisionRecs(koopa.izquierda, block->rect)) {
                    koopa.mirando_derecha = true;
                    koopa.position.x = block->rect.x + block->rect.width + 1; // separarlo
                }

                // Si choca con la parte izquierda de un bloque (esquina derecha del Koopa)
                if (CheckCollisionRecs(koopa.derecha, block->rect)) {
                    koopa.mirando_derecha = false;
                    koopa.position.x = block->rect.x - koopa.position.width - 1; // separarlo
                }
            }

            // Si Mario pisa al Koopa (solo si viene cayendo)
            if (!mario->isDead && CheckCollisionRecs(mario->pies, koopa.cabeza) && mario->velocidad > 0)
            {
                if (koopa.estado == Koopa::CAMINANDO) {
                    score += 200;
                    koopa.estado = Koopa::CONCHA_QUIETA;
                    koopa.velocidad = 0;
                }
                else if (koopa.estado == Koopa::CONCHA_QUIETA) {
                    koopa.estado = Koopa::CONCHA_MOVIENDOSE;
                    koopa.mirando_derecha = mario->mirando_derecha;
                }
                else if (koopa.estado == Koopa::CONCHA_MOVIENDOSE) {
                    koopa.estado = Koopa::CONCHA_QUIETA;
                }

                PlaySound(Squish);
                mario->velocidad = -PLAYER_JUMP_SPD / 1.5f;
            }
            else if (!mario->isDead && CheckCollisionRecs(mario->position, koopa.position))
            {
                if (koopa.estado == Koopa::CONCHA_QUIETA) {
                    // ACTIVAR concha desde un lado
                    if (CheckCollisionRecs(mario->derecha, koopa.izquierda)) {
                        koopa.estado = Koopa::CONCHA_MOVIENDOSE;
                        koopa.mirando_derecha = true; // Se mueve a la derecha
                        PlaySound(kick);
                    }
                    else if (CheckCollisionRecs(mario->izquierda, koopa.derecha)) {
                        koopa.estado = Koopa::CONCHA_MOVIENDOSE;
                        koopa.mirando_derecha = false; // Se mueve a la izquierda
                        PlaySound(kick);
                    }

                    mario->velocidad = -PLAYER_JUMP_SPD / 1.5f;  // Pequeño rebote opcional
                }
                else if (koopa.estado == Koopa::CONCHA_MOVIENDOSE) {
                    if (!CheckCollisionRecs(mario->pies, koopa.cabeza) || mario->velocidad <= 0) {
                        // Mario muere si le da una concha rodando desde un lado
                        
                        if (mario->poder == Mario::BASE) {
                            mario->isDead = true;
                            mario->sprite_status = 116;
                            mario->deathAnimationInProgress = true;
                            mario->velocidad = mario->deathVelocity;
                            StopMusicStream(music);

                            if (!playDeathSound) {
                                PlaySound(Die);
                                playDeathSound = true;
                            }

                        }
                        else {
                            PlaySound(Warp);
                            mario->estado = Mario::TRANSFORMANDOSE;
                        }
                        
                        return;
                    }
                    else {
                        // Pisa concha en movimiento → se detiene
                        koopa.estado = Koopa::CONCHA_QUIETA;
                        koopa.velocidad = 0;
                        mario->velocidad = -PLAYER_JUMP_SPD / 1.5f;
                        PlaySound(Squish);
                    }
                }
                else if (koopa.estado == Koopa::CAMINANDO) {
                    // Mueres al tocar Koopa caminando
                    if (mario->poder == Mario::BASE) {
                        mario->isDead = true;
                        mario->sprite_status = 116;
                        mario->deathAnimationInProgress = true;
                        mario->velocidad = mario->deathVelocity;
                        StopMusicStream(music);

                        if (!playDeathSound) {
                            PlaySound(Die);
                            playDeathSound = true;
                        }

                    }
                    else {
                        PlaySound(Warp);
                        mario->estado = Mario::TRANSFORMANDOSE;
                    }

                    return;
                }
            }

        }
        // Si Mario ha muerto, procesamos la animación de muerte
        if (mario->isDead && mario->deathAnimationInProgress) {
            mario->deathAnimTimer += delta;

            if (mario->deathAnimTimer >= 2.0f) {
                vidas--;

                if (vidas <= 0) {
                    gameState = GAME_OVER;
                    StopMusicStream(music);
                    PlayMusicStream(Gameover);
                    gameovermusicplayed = true;
                }
                else {
                    Reset(mario, goombas, koopas);  // Reinicia nivel y reposiciona a Mario
                    gameOver = false;
                    musicRestarted = false;
                    playDeathSound = false;
                }
            }

            // Aplicar movimiento de caída
            mario->MoveY(mario->velocidad * delta);
            mario->velocidad += GRAVEDAD * delta;

            return; // Salir del UpdateGame mientras Mario está muriendo
        }

        /* movimiento setas */
        for (int i = 0; i < lista_setas.size(); i++)
        {
            Mushroom* seta = &lista_setas[i];
            seta->hitGround = false;

            for (int i = 0; i < envItems; i++) {
                Hitbox* block = &hitboxes[i];

                //gravedad de la seta
                if (CheckCollisionRecs(seta->pies, block->rect)) {
                    seta->hitGround = true;
                    seta->velocidadY = 0;
                }
               
                if (CheckCollisionRecs(seta->izquierda, block->rect)) {
                    seta->mirando_derecha = true;
                    seta->position.x = block->rect.x + block->rect.width + 1;
                }

                if (CheckCollisionRecs(seta->derecha, block->rect)) {
                    seta->mirando_derecha = false;
                    seta->position.x = block->rect.x - seta->position.width - 1;
                }
            }

            if (CheckCollisionRecs(mario->position, seta->position)) {
                mario->estado = Mario::TRANSFORMANDOSE;
                PlaySound(Powerup);
                lista_setas.erase(lista_setas.begin() + i);
            }

            if (!seta->hitGround) {
                seta->velocidadY += GRAVEDAD * delta;
            }
        }


        /* Movimiento de Mario */
        if (mario->estado == Mario::NORMAL) {
            const float ANIM_FRAME_TIME = 0.05f; // tiempo entre frames de animación (0.1s = 10 FPS)

            if (IsKeyDown(KEY_RIGHT)) {
                mario->animTimer += delta;

                mario->mirando_derecha = true;
                mario->velocidadX += ACELERACION;
                if (mario->velocidadX > VELOCIDAD_MAXIMA)
                    mario->velocidadX = VELOCIDAD_MAXIMA;

                float velocidadActual = fabsf(mario->velocidadX);
                float proporciónVel = velocidadActual / VELOCIDAD_MAXIMA;
                float animSpeed = Lerp(ANIM_FRAME_MAX, ANIM_FRAME_MIN, proporciónVel); // inverso: más rápido → menor tiempo


                if (mario->canJump && velocidadActual > 0.0f && mario->sprite_status < 20) {
                    mario->sprite_status = 20;
                }

                if (mario->canJump && mario->animTimer >= animSpeed) {
                    mario->sprite_status += 18;
                    if (mario->sprite_status > 56)
                        mario->sprite_status = 20;

                   

                    mario->animTimer = 0.0f;
                }

                if (mario->velocidadX < 0 && mario->canJump)
                    mario->sprite_status = 76;

            }
            else if (IsKeyDown(KEY_LEFT)) {
                mario->animTimer += delta;

                mario->mirando_derecha = false;

                if (mario->position.x >= (GetScreenToWorld2D({ (1 - 0.16f) * 0.5f * screenWidth, (1 - 0.16f) * 0.5f * screenHeight }, camera).x) - 325) {
                    mario->velocidadX -= ACELERACION;
                    if (mario->velocidadX < -VELOCIDAD_MAXIMA)
                        mario->velocidadX = -VELOCIDAD_MAXIMA;

                    float velocidadActual = fabsf(mario->velocidadX);
                    float proporciónVel = velocidadActual / VELOCIDAD_MAXIMA;
                    float animSpeed = Lerp(ANIM_FRAME_MAX, ANIM_FRAME_MIN, proporciónVel);

                    if (mario->canJump && velocidadActual > 0.0f && mario->sprite_status < 20) {
                        mario->sprite_status = 20;
                    }

                    if (mario->canJump && mario->animTimer >= animSpeed) {
                        mario->sprite_status += 18;
                        if (mario->sprite_status > 56)
                            mario->sprite_status = 20;

                       

                        mario->animTimer = 0.0f;
                    }

                    if (mario->velocidadX > 0 && mario->canJump)
                        mario->sprite_status = 76;

                    if (mario->position.x >= (screenWidth / 2) - 12) {
                        worldPosition = mario->position.x;
                    }
                }
                else {
                    mario->velocidadX = 0;
                }
            }
            else {
                // Si no se mueve, aplicar fricción y reiniciar timer
                mario->animTimer = 0.0f;

                if (mario->velocidadX > 0) {
                    mario->velocidadX -= ACELERACION;
                    if (mario->velocidadX < 0) mario->velocidadX = 0;
                }
                else if (mario->velocidadX < 0) {
                    mario->velocidadX += ACELERACION;
                    if (mario->velocidadX > 0) mario->velocidadX = 0;
                }
            }

            cout << mario->velocidadX << endl;

            // Aplicar movimiento horizontal 
            mario->MoveX(mario->velocidadX);
       
            //cout << mario->position.x << endl;

            /* Salto */
            float jumpTime = 0.2f;

            // Inicio del salto
            if (IsKeyPressed(KEY_SPACE) && mario->canJump) {
                mario->isJumping = true;
                mario->canJump = false;
                mario->jumpTime = 0.0f;
                mario->velocidad = -PLAYER_JUMP_SPD;
                mario->sprite_status = 96;
                PlaySound(JumpSound);
                hitGround = false;
            }

            // Mientras se mantenga pulsado, se sigue aplicando fuerza hacia arriba
            if (mario->isJumping && IsKeyDown(KEY_SPACE)) {
                mario->jumpTime += delta;
                if (mario->jumpTime < mario->maxjumptime) {
                    mario->velocidad = -PLAYER_JUMP_SPD;
                }
                else {
                    mario->isJumping = false;  // se agota el tiempo máximo
                }
            }

            // Si se suelta la tecla antes, se corta el salto
            if (IsKeyReleased(KEY_SPACE)) {
                mario->isJumping = false;
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
                Reset(mario, goombas, koopas);
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
    if ((mario->position.y > 580 || mario->position.x >= 6336) && mario->estado != Mario::CAMINANDO_CASTILLO && mario->estado != Mario::TOCANDO_BANDERA && mario->estado != Mario::BAJANDO_BANDERA) {
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
    
    Goomba(500, 384),
    Goomba(700, 384),
    Goomba(1100, 384),
    Goomba(1800, 384)

    };

    vector<Koopa> koopas = {
    Koopa(1300, 384)
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
        if (stageClearMusicStarted && !stageClearMusicFinished) {
            UpdateMusicStream(StageClear);
            if (GetMusicTimePlayed(StageClear) >= GetMusicTimeLength(StageClear) - 0.1f) {
                StopMusicStream(StageClear);
                stageClearMusicFinished = true;
                gameState = LEVEL_COMPLETED;
            }
        }

        float deltaTime = GetFrameTime();

        UpdateGameState(deltaTime);

        ClearBackground(RAYWHITE);


        if (gameState == INTRO) {
            DrawIntroScreen();
         
        }
        else if (gameState == GAME_OVER) {
            Drawgameintoscreen();
            gameoverwaitimer += deltaTime;
            // ⏱ Reproducir música Game Over después de un pequeño retardo para no solaparse con Die.wav
            if (!gameovermusicplayed && gameoverwaitimer >= 5.0f) {
               /* StopMusicStream(music); */       // Por si acaso
                PlayMusicStream(Gameover);
                gameovermusicplayed = true;
            }


            // Permitir reiniciar con ENTER
            if (IsKeyPressed(KEY_ENTER)) {
                StopMusicStream(Gameover);  // Detenemos música de game over

                // Reset de variables de estado
                vidas = 3;
                score = 0;
                monedas = 0;
                worldPosition = 0;
                gameState = INICIAL;

                mario = Mario(316, 414);

                // 🔁 Reiniciamos flags
                playDeathSound = false;
                musicRestarted = false;
                gameovermusicplayed = false;
                waitingforgameover = false;
                gameoverwaitimer = 0.0f;
                diesoundplayed = false;
            }
        }
        else if (gameState == LEVEL_COMPLETED) {
            DrawLevelCompletedScreen();

            if (IsKeyPressed(KEY_ENTER)) {
                // 🔁 Reset de todo al inicio
                vidas = 3;
                score = 0;
                monedas = 0;
                worldPosition = 0;
                gameState = INICIAL;

                mario = Mario(316, 414);

                playDeathSound = false;
                musicRestarted = false;
                gameovermusicplayed = false;
                waitingforgameover = false;
                gameoverwaitimer = 0.0f;
                diesoundplayed = false;

                StopMusicStream(StageClear);
            }
        }
        else if (gameState == INICIAL) {
            DrawIntro();
         
        }
        else {

            if (gameOver&&vidas > 0) 
            {
                // Detiene música, reinicia estado y muestra pantalla de intro
                gameState = INTRO;
                StopMusicStream(music);
                PlayMusicStream(music);  // Reinicia música desde el principio
                Reset(&mario, goombas, koopas);           // Recoloca a Mario, reinicia cámaras y estados
                gameOver = false;
                musicPlaying = false;    // Para permitir reproducirla de nuevo en el estado INTRO
               
            }

            for (Goomba& goomba : goombas) goomba.Update(deltaTime);
            for (Koopa& koopa : koopas) koopa.Update(deltaTime);
            for (Mushroom& seta : lista_setas) seta.Update(deltaTime);

            UpdateGame(&mario, goombas,koopas, &lista_hitboxes[0], deltaTime, lista_hitboxes.size());
            DrawGame(&mario, goombas,koopas, lista_hitboxes);
        }

        if (vidas <= 0) 
        {
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
