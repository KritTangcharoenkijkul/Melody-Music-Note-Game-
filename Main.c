/* ******************************************************************************************
*
*   Melody (Version 1.0)
*
*   This game will never be completed without Raylib
*
*   from www.raylib.com
*
*   Special Thanks to Ramon Santamaria and many other indivual for Raylib
*
*   For the help of making this SDL Library
*
******************************************************************************************* */


#include <raylib.h>
#include <stdio.h>
#include <math.h>


///////////////////////////////////////////
////////Defining Value/////////////////////
///////////////////////////////////////////

#define Total_Base 3

#define noteaspeed 3
#define notebspeed 5
#define notecspeed 7

#define MAX_NOTES_A 6
#define MAX_NOTES_B 12
#define MAX_NOTES_C 18
#define MAX_NOTES 18
///////////////////////////////////////////
/////////Structs///////////////////////////
///////////////////////////////////////////

/* This is use to define Wave in Different Section of Song */
typedef enum { FIRST = 0, SECOND, THIRD } Section;

/* This is the base where player have to press when key come*/
typedef struct Base{
    float radius;
    Vector2 position;
    Color color;
    bool active;
} Base;

/* This is the note that will come down and player have to press */
typedef struct Note {
    float radius;
    Vector2 speed;
    Vector2 position;
    Color color;
    bool active;
} Note;

///////////////////////////////////////////
////////Global Variable Declaration////////
///////////////////////////////////////////

//Screen Size
static int screenWidth = 1280;
static int screenLength = 720;

//Status Variable
static bool clear; // Game Clear or Not
static bool pause; // Pause or Not
static int score; // Current Score
static int combo; // Combo Or Not
static int tcombo; // Combo At Current point
static int hit; // Check whether hit the note or not
static int maxcombo; // Max Combo Player Got
static int hiscore; // Hi-Score For Only My Railgun
static int hiscoremymai; // Hi-Score For My Mai Tonight
static int number; // Number For Randoming Note Position
static Base base[Total_Base]; // Base that user control
static Note note[MAX_NOTES]; // Maximum Note available
static int activeNote; // All current active note in Melody
static Section section; // Current Section (Speed / Note amount)
static int frames; // Count Frame for song
static int song; // Current song
static Music music; // Music
static Color color[10] = {ORANGE, GOLD, PINK, LIME, SKYBLUE, PURPLE, VIOLET, BEIGE, BROWN, MAGENTA}; // Note Color


///////////////////////////////////
///////Main Menu Section///////////
///////////////////////////////////


///////////////////////////////////
///////INTRODUCTION SCREEN/////////
///////////////////////////////////

void intro(void)
{
    while (!IsKeyPressed(KEY_SPACE)){

        BeginDrawing();

        ClearBackground(BLACK);

        DrawText("You are have chose to play", 360, 120, 40, BLUE);
        DrawText("MELODY", 430, 200, 100, PINK);
        DrawText("C Project made by Krit Tangcharoenkijkul", 210, 330, 40, GREEN);
        DrawText("with assistance of raylib.com", 480, 380, 20, GREEN);
        DrawText("This Game contain no microtransaction", 430, 420, 20, BLUE);
        DrawText("Press SpaceBar to Continue", 70, 500, 80, WHITE);

        EndDrawing();
    }

}

///////////////////////////////////
////////////HIGH SCORE/////////////
///////////////////////////////////

///////////////////////////////////
/////LOAD HIGHSCORE////////////////
///////////////////////////////////

void loadHiScore()
{
    FILE *saveFile;
    int point;

    ///////////////////
    //ONLY MY RAILGUN//
    ///////////////////
    saveFile = fopen("hiscore.sav", "r");
    fscanf(saveFile,"%d", &point);

    if(saveFile == NULL)
        hiscore = 0;
    else
        hiscore = point;

    ///////////////////
    //MY MAI TONIGHT //
    ///////////////////
    saveFile = fopen("hiscoremymai.sav", "r");
    fscanf(saveFile, "%d", &point);

    if(saveFile == NULL)
        hiscoremymai = 0;
    else
        hiscoremymai = point;
    fclose(saveFile);
    return;
}

///////////////////////////////////
/////UPDATE / SAVE HIGHSCORE///////
///////////////////////////////////
void updateHiScore()
{
    FILE *saveFile;

    ///////////////////
    //ONLY MY RAILGUN//
    ///////////////////
    if (song == 1)
    {
        saveFile = fopen("hiscore.sav", "w");

        if(score > hiscore)
            fprintf(saveFile,"%d",score);
        else
            fprintf(saveFile,"%d",hiscore);
    }

    ///////////////////
    //MY MAI TONIGHT //
    ///////////////////
    if (song == 2)
    {
        saveFile = fopen("hiscoremymai.sav", "w");

        if(score > hiscore)
            fprintf(saveFile,"%d",score);
        else
            fprintf(saveFile,"%d",hiscoremymai);
    }

    fclose(saveFile);
    return;
}

///////////////////////////////////
/////////MAIN MENU SCREEN//////////
///////////////////////////////////

void menu(void)
{

    // Initialing Audio Device
    InitAudioDevice();

    // Load Main Menu Music
    Music music = LoadMusicStream("Music/menu.ogg");

    // Play Main Menu Music
    PlayMusicStream(music);

    //Checking Whether the music is playing or not
    if (!IsMusicPlaying(music))
        printf("Main Menu Music do not work"); //If music do not work print this

    // Set Frame Per Second to 60
    SetTargetFPS(60);

    // Load Hi-Score for both variable
    loadHiScore();

    // Main Menu Function
    while (!WindowShouldClose())
    {

        BeginDrawing();

        // Background Color to Maroon
        ClearBackground(MAROON);

        // Format Text in Main Menu
        DrawText("MELODY", 290, 20, 230, RAYWHITE);
        DrawText("Hi-Score", 10, 10, 30, GREEN);
        DrawText("Only My Railgun", 10, 50, 30, BLUE);
        DrawText(FormatText("%i", hiscore), 10, 90, 30, WHITE);
        DrawText("My Mai Tonight", 10, 130, 30, BLUE);
        DrawText(FormatText("%i", hiscoremymai), 10, 170, 30, WHITE);
        DrawText("PRESS F TO Start", 0, 320, 70, GOLD);
        DrawText("PRESS J TO View Instruction", 0, 420, 70, GREEN);
        DrawText("PRESS SPACEBAR TO QUIT", 0, 520, 70, BLUE);

        EndDrawing();

        // Update Music Stream in Every Single Frame
        UpdateMusicStream(music);

        // Closing the Program
        if (IsKeyPressed(KEY_SPACE))
        {
            CloseWindow(); // Pressing Spacebar will close window
        }

        // Transit to Select Song
        if (IsKeyPressed(KEY_F))
        {
            UnloadMusicStream(music); // Close audio device

            CloseAudioDevice(); // Close audio device

            choosesong(); // Going to Choose Song Menu

            break;
        }

        // Main Menu Music Restarting for Fun
        if (IsKeyPressed(KEY_G))
        {
            StopMusicStream(music);
            PlayMusicStream(music); // Stop Music and Start them Again (Easter Egg)
        }

        // Pause/Resume Main Menu Music
        if (IsKeyPressed(KEY_H))
        {
            pause = !pause;

            if (pause == true)
                PauseMusicStream(music); // If music is not pause then pause music
            else
                ResumeMusicStream(music); // Else resume the music once again
        }


        // Transit To Instruction Menu
        if (IsKeyPressed(KEY_J))
        {
            UnloadMusicStream(music); // Close audio device

            CloseAudioDevice(); // Close audio device

            Instruction(); // Going to Instruction Menu
        }
    }
}

///////////////////////////////////////
//////////INSTRUCTION SCREEN///////////
///////////////////////////////////////

void Instruction(void)
{
    // Initialing Audio Device
    InitAudioDevice();

    // Load Main Menu Music
    Music music = LoadMusicStream("Music/instruction.ogg");

    // Play Main Menu Music
    PlayMusicStream(music);

    //Checking Whether the music is playing or not
    if (!IsMusicPlaying(music))
        printf("Main Menu Music do not work"); //If music do not work print this

    while (!WindowShouldClose())
    {

        BeginDrawing();

        // Set Background
        ClearBackground(DARKBLUE);

        // Add Title, text and Instruction
        DrawText("HOW TO PLAY", 200, 20, 120, WHITE);
        DrawText("MELODY", 430, 120, 100, ORANGE);

        DrawText("1st: Press F to go to song selection page", 0, 210, 30, WHITE);
        DrawText("Press J to change song, Press F to start the song", 0, 260, 30, WHITE);
        DrawText("2nd: Press Key according to color when note hit base to gain point and combo", 0, 310, 30, WHITE);
        DrawText("The Left side is Red, Please press the F Key", 0, 360, 30, RED);
        DrawText("The Middle is Green, Please press the Space Key", 0, 410, 30, GREEN);
        DrawText("The Right side is BLUE, Please press the J Key", 0, 460, 30, SKYBLUE);
        DrawText("3rd: After clearing the game, score will be shown", 0, 510, 30, WHITE);
        DrawText("4th: PLAY AGAIN OR QUIT JUST CHOOSE SOME OPTION", 0, 560, 30, WHITE);
        DrawText("TIPS: GET AS MUCH COMBO AND DO NOT MESS UP", 0, 620, 30, WHITE);
        DrawText("PRESS SPACE TO GO BACK", 0, 670, 30, ORANGE);

        EndDrawing();

        // Update Music every frame
        UpdateMusicStream(music);

        // Going back to the main Menu
        if (IsKeyPressed(KEY_SPACE))
        {
            UnloadMusicStream(music); // Close audio device

            CloseAudioDevice();

            menu(); // Go Back to the menu
        }

        if (IsKeyPressed(KEY_F))
        {
            UnloadMusicStream(music); // Close Audio Device

            CloseAudioDevice();

            choosesong(); // Go to Choose song
        }
    }

}

///////////////////////////////////
//////////SONG SELECTION///////////
///////////////&///////////////////
///////////MAIN GAME///////////////
///////////////////////////////////


///////////////////////////////////
//////ONLY MY RAILGUN//////////////
///////////////////////////////////
void choosesong(void)
{
    InitAudioDevice();

    // Load Song Music
    Music music = LoadMusicStream("Music/railgun.ogg");

    // Play Sample Music
    PlayMusicStream(music);

    // Load Song Cover Photo (INTO RAM)
    Image cover = LoadImage("Photo/railgun.png");


    // Turn image into Texture
    Texture2D texture = LoadTextureFromImage(cover);

    // Unload Cover Photo (IMAGE) OUT OF RAM
    UnloadImage(cover);

    while (!WindowShouldClose())
    {
        // Menu Background and Text Design
        BeginDrawing();

        // Create Background
        ClearBackground(BLACK);

        // Create Title
        DrawText("Choose the Music!", 360, 30, 60, WHITE);

        // Draw Cover Image using the Texture
        DrawTexture(texture, screenWidth/2 - texture.width/2, screenLength/2 - texture.height/2 - 110, WHITE);

        // Insert Text and Instruction
        DrawText("ONLY MY RAILGUN", 170, 400, 100, RED);
        DrawText("FripSide", 530, 500, 50, GREEN);
        DrawText("Cover by Silverick", 490, 550, 30, BLUE);
        DrawText("PRESS F TO START THE GAME", 300, 600, 40, WHITE);
        DrawText(" PRESS J TO CHANGE MUSIC ", 300, 630, 40, WHITE);
        DrawText("PRESS SPACEBAR TO GO BACK", 300, 660, 40, WHITE);

        EndDrawing();

        // Update Music every frame
        UpdateMusicStream(music);

        if (IsKeyPressed(KEY_J))
        {
            UnloadTexture(texture); // Remove Texture From File

            UnloadMusicStream(music); // Close audio device

            CloseAudioDevice();

            choosesongb(); // Get to My Mai Tonight
        }

        // Going Back to the Menu
        if (IsKeyPressed(KEY_SPACE))
        {
            UnloadTexture(texture); // Remove Texture From File

            UnloadMusicStream(music); // Close audio device

            CloseAudioDevice();

            menu(); // Go back to Menu
        }

        /////////////////////////////////////
        ////////////Start the game///////////
        /////////////////////////////////////
        if (IsKeyPressed(KEY_F))
        {

            // Clearing The Screen
            UnloadTexture(texture); // Remove Cover Image

            UnloadMusicStream(music);

            CloseAudioDevice(); // Close audio device

            ////////////////
            // Main Game////
            ////////////////
            song = 1;
            // Initialize Music
            InitAudioDevice();

            Music music = LoadMusicStream("Music/railgun.ogg");

            PlayMusicStream(music);

            // Initialize Game
            InitGame();

            // In each frames
            while (!WindowShouldClose())
            {

                frames++;

                UpdateMusicStream(music);

                UpdateDrawFrame();

            }

        }
    }
}


//////////////////////////////////////
//////MY MAI TONIGHT//////////////////
//////////////////////////////////////
void choosesongb(void)
{
    InitAudioDevice();

    // Load Song Music (SAMPLE)
    Music music = LoadMusicStream("Music/mymai.ogg");

    // Play Sample Music
    PlayMusicStream(music);

    // Load Song Cover Photo (INTO RAM)
    Image cover = LoadImage("Photo/mymai.png");


    // Turn image into Texture
    Texture2D texture = LoadTextureFromImage(cover);

    // Unload Cover Photo (IMAGE) OUT OF RAM
    UnloadImage(cover);

    while (!WindowShouldClose())
    {
        // Menu Background and Text Design
        BeginDrawing();

        // Create Background
        ClearBackground(BLACK);

        // Create Title
        DrawText("Choose the Music!", 360, 30, 60, WHITE);

        // Draw Cover Image using the Texture
        DrawTexture(texture, screenWidth/2 - texture.width/2, screenLength/2 - texture.height/2 - 110, WHITE);

        // Insert Text and Instruction
        DrawText("MY MAI TONIGHT", 200, 400, 100, RED);
        DrawText("AQOURS", 530, 500, 50, GREEN);
        DrawText("Cover by Silverick", 490, 550, 30, BLUE);
        DrawText("PRESS F TO START THE GAME", 300, 600, 40, WHITE);
        DrawText(" PRESS J TO CHANGE MUSIC ", 300, 630, 40, WHITE);
        DrawText("PRESS SPACEBAR TO GO BACK", 300, 660, 40, WHITE);

        EndDrawing();

        // Update Music every frame
        UpdateMusicStream(music);

        // Choose Another Song
        if (IsKeyPressed(KEY_J))
        {
            UnloadTexture(texture); // Remove Texture From File

            UnloadMusicStream(music); // Close audio device

            CloseAudioDevice();

            choosesong(); // Go to Only My Railgun
        }

        // Going Back to the Menu
        if (IsKeyPressed(KEY_SPACE))
        {
            UnloadTexture(texture); // Remove Texture From File

            UnloadMusicStream(music); // Close audio device

            CloseAudioDevice();

            menu(); // Go back to Menu
        }

        /////////////////////////////////////
        ////////////Start the game///////////
        /////////////////////////////////////
        if (IsKeyPressed(KEY_F))
        {

            // Clearing The Screen
            UnloadTexture(texture); // Remove Cover Image

            UnloadMusicStream(music);

            CloseAudioDevice(); // Close audio device

            ////////////////
            // Main Game////
            ////////////////
            song = 2;
            // Initialize Music
            InitAudioDevice();

            Music music = LoadMusicStream("Music/mymai.ogg");

            PlayMusicStream(music);

            // Initialize Game
            InitGame();

            // Update every frame
            while (!WindowShouldClose())
            {

                frames++;

                UpdateMusicStream(music);

                UpdateDrawFrame();

            }


        }
    }
}

////////////////////////////
/////UPDATE EVERY FRAME/////
////////////////////////////
void UpdateDrawFrame(void)
{

    DrawGame();
    UpdateGame();

}

/////////////////////////////////
///////GAME INITIALISATION///////
/////////////////////////////////

void InitGame(void)
{
    // Initialize Basic
    clear = false;
    section = FIRST;
    activeNote = MAX_NOTES_A;
    score = 0;
    combo = 0;
    tcombo = 0;
    maxcombo = 0;
    frames = 0;
    hit = 0;

    // Initialize The base
    for (int a = 0; a < Total_Base; a++)
    {
        base[a].radius = 50;
        base[a].position.x = (320 * (a + 1));
        base[a].position.y = 576;
        if (a == 0)
            base[a].color = RED;
        else if (a == 1)
            base[a].color = GREEN;
        else if (a == 2)
            base[a].color = BLUE;
        else
            printf("Error occur when initializing base color");
        base[a].active = true;
    }

    // Initialize The note
    for (int b = 0; b < MAX_NOTES; b++)
    {
        int x, y;

        note[b].radius = 50;
        note[b].speed.x = noteaspeed;
        note[b].speed.y = noteaspeed;
        x = GetRandomValue(1, 3);
        y = -(GetRandomValue(200, 600));
        note[b].position.x = (320 * x);
        note[b].position.y = (y);
        note[b].active = true;
        note[b].color = color[GetRandomValue(0, 9)];

    }

}


////////////////////////////////////
///////PER FRAME GAME UPDATE////////
////////////////////////////////////
void UpdateGame(void)
{
    // Until the Game Is Cleared
    if (!clear)
    {
        // Only My Railgun
        if (song == 1)
        {
            switch(section)
            {
                case FIRST:
                {

                    if (frames < 440 || frames < 2440)
                    {
                        activeNote = MAX_NOTES_A;
                        section = FIRST;
                    }

                    // Go to the SECOND SECTION
                    if (frames == 440 || frames == 2440)
                    {
                        for (int i = 0; i < MAX_NOTES; i++)
                            note[i].speed.y = notebspeed;

                        for (int i = 0; i < activeNote; i++)
                        {
                            if (!note[i].active)
                                note[i].active = true;
                        }
                        activeNote = MAX_NOTES_B;
                        section = SECOND;
                    }

                }break;

                case SECOND:
                {
                    if (frames < 1600)
                    {
                        activeNote = MAX_NOTES_B;
                        section = SECOND;
                    }

                    // Going back to the SLOW DOWN MUSIC (FIRST SECTION)
                    if (frames == 1600)
                    {
                        activeNote = MAX_NOTES_A;
                        section = FIRST;

                        for (int i = 0; i < MAX_NOTES; i++)
                            note[i].speed.y = noteaspeed;

                        for (int i = 0; i < activeNote; i++)
                        {
                            if (!note[i].active)
                            note[i].active = true;
                        }


                    }

                    if (frames > 2440 && frames < 3320)
                    {
                        activeNote = MAX_NOTES_B;
                        section = SECOND;

                        for (int i = 0; i < activeNote; i++)
                        {
                            if (!note[i].active)
                                note[i].active = true;
                        }
                    }

                    // Go to the Second Chorus (THIRD SECTION)
                    if (frames == 3320)
                    {
                        for (int i = 0; i < MAX_NOTES; i++)
                            note[i].speed.y = notecspeed;

                        for (int i = 0; i < activeNote; i++)
                        {
                            if (!note[i].active)
                                note[i].active = true;
                        }
                        activeNote = MAX_NOTES_C;
                        section = THIRD;
                    }

                }break;

                case THIRD:
                {
                    if (frames < 5350)
                    {
                        activeNote = MAX_NOTES_C;
                        section = THIRD;
                    }

                    // Game Cleared
                    if (frames == 5350)
                    {
                        UnloadGame();

                        clear = true;

                        DrawGame();

                        summary();


                    }
                }break;

                default: break;
            }
        }
        // My Mai Tonight
        if (song == 2)
        {
            switch(section)
            {
                case FIRST:
                {
                    if (frames < 1200)
                    {
                        activeNote = MAX_NOTES_A;
                        section = FIRST;
                    }
                    // Go to the SECOND SECTION
                    if (frames == 1200)
                    {
                        for (int i = 0; i < MAX_NOTES; i++)
                            note[i].speed.y = notebspeed;

                        for (int i = 0; i < activeNote; i++)
                        {
                            if (!note[i].active)
                                note[i].active = true;
                        }
                        activeNote = MAX_NOTES_B;
                        section = SECOND;
                    }

                }break;

                case SECOND:
                {

                    if (frames > 1200 && frames < 3650)
                    {
                        activeNote = MAX_NOTES_B;
                        section = SECOND;

                        for (int i = 0; i < activeNote; i++)
                        {
                            if (!note[i].active)
                                note[i].active = true;
                        }
                    }
                    // Go to the Second Chorus (THIRD SECTION)
                    if (frames == 3650)
                    {
                        for (int i = 0; i < MAX_NOTES; i++)
                            note[i].speed.y = notecspeed;

                        for (int i = 0; i < activeNote; i++)
                        {
                            if (!note[i].active)
                                note[i].active = true;
                        }
                        activeNote = MAX_NOTES_C;
                        section = THIRD;
                    }

                }break;

                case THIRD:
                {
                    if (frames < 5650)
                    {
                        activeNote = MAX_NOTES_C;
                        section = THIRD;
                    }
                    // Game Cleared
                    if (frames == 5650)
                    {
                        UnloadGame();

                        clear = true;

                        DrawGame();

                        summary();


                    }
                }break;

                default: break;
            }
        }





        //////////////////////////////////////
        /////////NOTE MOVEMENT////////////////
        //////////////////////////////////////

        for (int i = 0; i < activeNote; i++)
        {
            if (note[i].active == true)
            {
                note[i].position.y += note[i].speed.y;

                if (note[i].position.y > screenLength)
                {
                    number = (GetRandomValue(1, 3));
                    if (number == 1)
                        note[i].position.x = 320;
                    if (number == 2)
                        note[i].position.x = 640;
                    if (number == 3)
                        note[i].position.x = 960;
                    note[i].position.y = GetRandomValue(-1000, -100);

                    tcombo = 0;
                }
            }
        }


        /* ***********************************
        ********Key Pressing During Game *****
        ************************************** */


        for (int a = 0; a < Total_Base; a++)
        {
            ////////////////////
            ///////F KEY////////
            ////////////////////
            if (a == 0)
            {
                if (IsKeyPressed(KEY_F))
                {
                    /* Check for all Active Note */
                    for (int b = 0; b < activeNote; b++)
                    {

                        /* Check whether user (F Key) hit a note */
                        if (CheckCollisionCircles(base[a].position, base[a].radius, note[b].position, note[b].radius))
                        {
                            /* User (F Key) hit a note */
                            note[b].active = false; // Note Disappear

                            // Reposition Themback to Loop Again
                            number = (GetRandomValue(0, 2));
                                if (number == 0)
                                    note[b].position.x = 320;
                                if (number == 1)
                                    note[b].position.x = 640;
                                if (number == 2)
                                    note[b].position.x = 960;
                            note[b].position.y = GetRandomValue( -1000, -100);

                            score = score + 100 + (tcombo * 10); // Score is increase
                            combo = combo + 1; // Combo value increase by 1

                            hit = 1;
                            if (combo >= 1)
                                tcombo = tcombo + 1;

                            if (combo == 0)
                                tcombo = 0;

                            if (tcombo > maxcombo)
                                maxcombo = tcombo;

                            break;
                        }

                        /* In Situation that user (F Key) hit blank */
                        else
                        {
                            note[b].active = true;
                            combo = 0; // Reset combo to 1

                        }
                    }

                    // If hit nothing - combo on screen = 0 else reset
                    if (hit == 0)
                        tcombo = 0;

                    if (hit == 1)
                        hit = 0;

                }
            }

            ////////////////////////
            /////SPACE_KEY//////////
            ////////////////////////
            if (a == 1)
            {
                if (IsKeyPressed(KEY_SPACE))
                {

                    /* Check for all Active Note */
                    for (int b = 0; b < activeNote; b++)
                    {
                        /* Check whether user (Space Key) hit a note */
                        if (CheckCollisionCircles(base[a].position, base[a].radius, note[b].position, note[b].radius))
                        {
                            /* User (Space Key) hit a note */
                            note[b].active = false; // Note Disappear

                            // Reposition Themback to Loop Again
                            number = (GetRandomValue(0, 2));
                                if (number == 0)
                                    note[b].position.x = 320;
                                if (number == 1)
                                    note[b].position.x = 640;
                                if (number == 2)
                                    note[b].position.x = 960;
                            note[b].position.y = GetRandomValue(-1000, -100);

                            score = score + 100 + (tcombo * 10); // Score is increase
                            combo = combo + 1; // Combo value increase by 1

                            hit = 1;
                            if (combo >= 1)
                                tcombo = tcombo + 1;


                            if (tcombo > maxcombo)
                                maxcombo = tcombo;
                            break;
                        }

                        /* In Situation that user (Space Key) hit blank */
                        else
                        {
                            note[b].active = true;
                            combo = 0; // Reset combo to 1
                        }
                    }

                    // If hit nothing - combo on screen = 0 else reset
                    if (hit == 0)
                        tcombo = 0;

                    if (hit == 1)
                        hit = 0;

                }
            }

            ////////////////////////////
            ///////J_Key////////////////
            ////////////////////////////
            if (a == 2)
            {
                if (IsKeyPressed(KEY_J))
                {

                    /* Check for all Active Note */
                    for (int b = 0; b < activeNote; b++)
                    {
                        /* Check whether user (J Key) hit a note */
                        if (CheckCollisionCircles(base[a].position, base[a].radius, note[b].position, note[b].radius))
                        {
                            /* User (J Key) hit a note */
                            note[b].active = false; // Note Disappear

                            // Reposition Themback to Loop Again
                            number = (GetRandomValue(0, 2));
                            if (number == 0)
                                note[b].position.x = 320;
                            if (number == 1)
                                note[b].position.x = 640;
                            if (number == 2)
                                note[b].position.x = 960;
                            note[b].position.y = GetRandomValue( -1000, -100);

                            hit = 1;
                            score = score + 100 + (tcombo * 10); // Score is increase
                            combo = combo + 1; // Combo value increase by 1

                            if (combo >= 1)
                                tcombo = tcombo + 1;


                            if (tcombo > maxcombo)
                                maxcombo = tcombo;
                            break;
                        }

                        /* In Situation that user (J Key) hit blank */
                        else
                        {
                            note[b].active = true;
                            combo = 0; // Reset combo to 1
                        }
                    }
                    // If hit nothing - combo on screen = 0 else reset
                    if (hit == 0)
                        tcombo = 0;

                    if (hit == 1)
                        hit = 0;

                }
            }
        }

    }
    else
    {
        UnloadGame(); // Unload Loaded Data
        summary(); // GO TO SUMMARY
    }

}

/////////////////////////
/////////MAIN GAME DRAW//
/////////////////////////
void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(LIGHTGRAY);

    if (!clear)
    {
        // Drawing the base

        for(int i = 0; i < Total_Base; i++)
        {
            DrawCircle(base[i].position.x, base[i].position.y, base[i].radius, base[i].color);
        }

        // Drawing the Note

        for(int i = 0; i < activeNote; i++)
        {
            DrawCircle(note[i].position.x, note[i].position.y, note[i].radius, note[i].color);
        }

        // Score and Combo Display
        DrawText("F", 305, 556, 50, BLACK);
        DrawText("J", 945, 556, 50, BLACK);

        DrawText(FormatText("%05i", score), 520, 80, 90, VIOLET);
        DrawText(FormatText("%03i", tcombo), 580, 350, 70, MAGENTA);
        DrawText(FormatText("%03i", frames), 0, 80, 30, LIME);

        if (tcombo > 0)
            DrawText("PERFECT", 450, 420, 80, SKYBLUE);
        else
            DrawText("MISS", 540, 420, 80, RED);

    }

    EndDrawing();

    // Move to summary after the song is finished
    if (song == 1)
        if (frames == 5350)
            summary();

    if (song == 2)
        if (frames == 5650)
            summary();
}


///////////////////////////////////////////
///////////GAME SUMMARY////////////////////
///////////////////////////////////////////

void summary(void)
{
    // Initialing Audio Device
    InitAudioDevice();

    // Load Main Menu Music
    Music music = LoadMusicStream("Music/summary.ogg");

    // Play Main Menu Music
    PlayMusicStream(music);

    // Load Hi-Score Photo (INTO RAM)

    Image clear = LoadImage("Photo/railgun.png");

    if (song == 2)
    {
        UnloadImage(clear);
        Image clear = LoadImage("Photo/mymai.png");
    }



    // Turn image into Texture
    Texture2D texture = LoadTextureFromImage(clear);

    // Unload Cover Photo (IMAGE) OUT OF RAM
    UnloadImage(clear);

    while (!WindowShouldClose())
    {
        // Menu Background and Text Design
        BeginDrawing();

        //Set Background Color for Summary
        if (score > hiscore)
            ClearBackground(GOLD);
        else
            ClearBackground(LIGHTGRAY);

        //Different Title for Different Result
        if (hiscore >= score)
            DrawText("!!GAME CLEAR!!", 640, 50, 70, BLACK);
        else
            DrawText("NEW HIGHSCORE!", 640, 50, 70, BLACK);

        // Draw Image using the Texture
        DrawTexture(texture, 170, screenLength/2 - clear.height/2, WHITE);

        // Displaying Your Score
        DrawText("Your Score", 640, 200, 50, RED);
        DrawText(FormatText("%i", score), 640, 250, 50, RED);

        // Displaying MAXCOMBO
        DrawText("MAX COMBO", 640, 400, 50, DARKBLUE);
        DrawText(FormatText("%i", maxcombo), 640, 450, 50, DARKBLUE);

        // Additional Design
        if (song == 1)
        {
            DrawText("ONLY MY RAILGUN", 90, 130, 50, DARKBLUE);
            DrawText("FRIPSIDE / COVER BY SILVERICK", 70, 550, 30, DARKBLUE);

            // Displaying Hiscore
            if (score > hiscore)
                DrawText("The Previous highscore", 640, 300, 50, GREEN);
            else
                DrawText("The Highscore is still", 640, 300, 50, GREEN);
            DrawText(FormatText("%i", hiscore), 640, 350, 50, GREEN);
        }

        if (song == 2)
        {
            DrawText("MY MAI TONIGHT", 90, 130, 50, DARKBLUE);
            DrawText("AQOURS / COVER BY SILVERICK", 70, 550, 30, DARKBLUE);

            // Displaying Hiscore
            if (score > hiscore)
                DrawText("The Previous highscore", 640, 300, 50, GREEN);
            else
                DrawText("The Highscore is still", 640, 300, 50, GREEN);
            DrawText(FormatText("%i", hiscoremymai), 640, 350, 50, GREEN);
        }

        DrawText("PRESS G TO GO TO MENU", 640, 620, 30, BLACK);
        DrawText("PRESS H TO QUIT", 640, 670, 30, BLACK);

        EndDrawing();

        UpdateMusicStream(music);

        // Closing the Program
        if (IsKeyPressed(KEY_H))
        {
            updateHiScore(); // Update HiScore

            resetGame(); // Reset Variable

            UnloadTexture(texture); // Remove Texture From File

            UnloadMusicStream(music); // Close audio device

            CloseAudioDevice();

            CloseWindow(); // Pressing Spacebar will close window
        }

        // Going back to the MENU
        if (IsKeyPressed(KEY_G))
        {
            updateHiScore(); // Update HiScore

            resetGame(); // Reset Variable

            UnloadTexture(texture); // Remove Texture From File

            UnloadMusicStream(music); // Close audio device

            CloseAudioDevice();

            menu();
        }
    }

}

// Reset game for next round
void resetGame(void)
{
    score = 0;
    maxcombo = 0;
    combo = 0;
}

// Close Program
void endprogram()
{
    CloseWindow(); // Closing the program
}

void UnloadGame(void)
{

    UnloadMusicStream(music); // Close audio device

    CloseAudioDevice();

}


////////////////////////////////////
////////MAIN FUNCTION///////////////
////////////////////////////////////

int main(void)
{
    // Create Window with name Melody
    // With ScreenWidth = 1280 and ScreenLength = 720
    InitWindow(screenWidth, screenLength, "Melody");

    // Introduction Screen
    intro();

    // Main Menu Screen
    menu();

    // Initialize Music
    InitAudioDevice();

    Music music = LoadMusicStream("Music/railgun.ogg");

    PlayMusicStream(music);

    InitGame(); // Initialize the Game

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        // Update Game Every Single Frame and Draw them out
        UpdateDrawFrame();

        // Update Music Stream in Every Single Frame
        UpdateMusicStream(music);

    }

    // If Main Menu is Cleared
    UnloadGame();

    CloseWindow();

    return 0;
}

