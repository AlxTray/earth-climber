// main.cpp : This file contains the 'main' function. Program execution begins and ends there.

//Earth Climber

#include <raylib.h>
#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include <cstring>

#include "ButtonGen.h"
#include "LevelObject.h"
#include "Player.h"

using namespace std;

// global variable definitions
float coinRadius = 35;
bool close = false;
int coinsCollected = 0;
bool levelComplete = false;
int levelSelected = 1;

int levelObjectsLength;
int levelCoinsLength;

int key = 0;
string line;
string tempText = "\0";
string leftText = "\0";
string rightText = "\0";
string jumpText = "\0";
int leftBind = 65;
int rightBind = 68;
int jumpBind = 32;

//Camera code adapted from raylib.com
void UpdateCameraEven(Camera2D* camera, Player* player, float delta)
{
    Vector2 playerPos = player->getPosition();
    // Sets varibles on first enter into subroutine
    static float evenOutSpeed = 1700;
    static bool even = false;
    static float evenOutTarget = playerPos.y;

    camera->target.x = playerPos.x;

    // if player has changed vertical coordinate, then set vertical camera target to that
    if (even) {
        if (evenOutTarget > camera->target.y) {
            camera->target.y += evenOutSpeed * delta;

            if (camera->target.y > evenOutTarget) {
                camera->target.y = evenOutTarget;
                even = false;
            }
        }
        else {
            camera->target.y -= evenOutSpeed * delta;

            if (camera->target.y < evenOutTarget) {
                camera->target.y = evenOutTarget;
                even = false;
            }
        }
    }
    else {
        // Only set even to true if the player can jump and it colliding with the top of an object (has landed)
        if (player->getCanJump() && (player->getSpeed() == 0) && (playerPos.y != camera->target.y)) {
            even = true;
            evenOutTarget = playerPos.y;
        }
    }
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(0, 0, "Earth Climber");
    ToggleFullscreen();

    //Gets the mutiplier for cross resolution compataibility,
    //so I am able to define location using percentage instead of exact coordinates
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int xMultiplier = screenWidth / 100;
    int yMultiplier = screenHeight / 100;

    // Enum for match case
    typedef enum Screen { MAIN = 0, LEVELSELECT, GAME, SETTINGS } Screen;
    Screen currentScreen = MAIN;

    //Default values for the player
    Player player({100, 1700}, {100, 1700, 80, 80}, 0, false, false, true, true);

    //Default values for the camera
    Camera2D camera = { 0 };
    camera.target = player.getPosition();
    camera.offset = { screenWidth / 2.0f, screenHeight / 1.85f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    bool gamePaused = false;
    Color pauseMenuAlpha = Fade(BLACK, 0.9);

    Flag levelOneFlag({ 7075, 3900, 200, 85 }, { 7000, 3900, 75, 620 });
    Flag levelTwoFlag({ 8275, 1480, 200, 85 }, { 8200, 1480, 75, 620 });
    Flag levelThreeFlag({ 6975, 780, 200, 85 }, { 6950, 780, 75, 620 });

    //Instantisisation for struct arrays for objects in certain levels
     MapObject levelOneObjects[] = {
        {{0, 1800, 1000, 200}, true, false, false, LIGHTGRAY},
        {{3100, 2800, 2500, 1200}, false, false, false, DARKGRAY},
        {{1500, 1800, 700, 200}, true, false, false, LIGHTGRAY},
        {{0, 5000, 10000, 100}, true, false, true, RAYWHITE},
        {{3000, 400, 1450, 2900}, true, false, false, LIGHTGRAY},
        {{4450, 2800, 1500, 400}, true, false, false, LIGHTGRAY},
        {{2500, 900, 500, 690}, true, false, false, LIGHTGRAY},
        {{2500, 2190, 500, 550}, true, false, false, LIGHTGRAY},
        {{2000, 2400, 300, 200}, true, false, false, LIGHTGRAY},
        {{1750, 2700, 100, 200}, true, false, false, LIGHTGRAY},
        {{2100, 3100, 100, 200}, true, false, false, LIGHTGRAY},
        {{1700, 3250, 100, 200}, true, false, false, BROWN},
        {{2700, 4050, 3000, 695}, true, false, false, LIGHTGRAY},
        {{4450, 4745, 1250, 700}, true, false, false, LIGHTGRAY},
        {{2700, 3900, 900, 150}, true, false, false, LIGHTGRAY},
        {{3600, 3950, 2170, 100}, true, false, true, ORANGE},
        {{5700, 4050, 70, 2000}, true, false, true, ORANGE},
        {{4000, 3800, 100, 50}, true, false, false, LIGHTGRAY},
        {{4600, 3730, 100, 50}, true, false, false, LIGHTGRAY},
        {{5250, 3780, 100, 50}, true, false, false, LIGHTGRAY},
        {{6050, 4520, 1200, 1000}, true, false, false, LIGHTGRAY}
    };

     MapObject levelTwoObjects[] = {
         {{0, 1800, 1000, 200}, true, false, false, LIGHTGRAY},
         {{1500, 1800, 100, 200}, true, false, false, BROWN},
         {{1800, 1800, 100, 200}, true, false, false, BROWN},
         {{2300, 1900, 100, 200}, true, false, false, BROWN},
         {{2800, 2100, 100, 200}, true, false, false, BROWN},
         {{3500, 2400, 300, 100}, true, false, false, BROWN},
         {{4100, 2050, 100, 200}, true, false, false, BROWN},
         {{4500, 1850, 100, 200}, true, false, false, BROWN},
         {{4900, 1650, 100, 200}, true, false, false, BROWN},
         {{5550, 1800, 800, 150}, true, false, false, BROWN},
         {{6750, 1950, 100, 200}, true, false, false, BROWN},
         {{7500, 2100, 1000, 1200}, true, false, false, LIGHTGRAY},
         {{0, 3000, 8000, 100}, true, false, true, RAYWHITE}
     };

     MapObject levelThreeObjects[] = {
        {{0, 1800, 1000, 200}, true, false, false, LIGHTGRAY},
        {{1625, 1750, 75, 50}, true, false, false, BROWN},
        {{1625, 1750, 75, 50}, true, false, false, BROWN},
        {{2100, 1650, 75, 50}, true, false, false, BROWN},
        {{2300, 1450, 40, 50}, true, false, false, BROWN},
        {{2500, 1450, 40, 50}, true, false, false, BROWN},
        {{2700, 1450, 40, 50}, true, false, false, BROWN},
        {{2900, 1450, 40, 50}, true, false, false, BROWN},
        {{3100, 1275, 100, 500}, true, false, true, ORANGE},
        {{3300, 1450, 40, 50}, true, false, false, BROWN},
        {{3500, 1450, 40, 50}, true, false, false, BROWN},
        {{3900, 1275, 1200, 25}, true, false, false, BROWN},
        {{5900, 1550, 25, 25}, true, false, false, BROWN},
        {{6250, 1400, 1000, 1200}, true, false, false, LIGHTGRAY},
        {{0, 3000, 8000, 100}, true, false, true, RAYWHITE}
     };

     // Instantiation arrays for coin placemnts in all 3 levels
    Coin levelOneCoins[] = {
        {{1250, 1550}, false, YELLOW},
        {{2150, 2200}, false, YELLOW},
        {{1750, 3100}, false, YELLOW},
        {{4050, 3650}, false, YELLOW},
        {{4650, 3580}, false, YELLOW},
        {{5300, 3630}, false, YELLOW}
    };

    Coin levelTwoCoins[] = {
        {{1250, 1550}, false, YELLOW},
        {{1850, 1700}, false, YELLOW},
        {{2850, 2000}, false, YELLOW},
        {{4150, 1950}, false, YELLOW},
        {{6800, 1850}, false, YELLOW}
    };

    Coin levelThreeCoins[] = {
        {{1250, 1550}, false, YELLOW},
        {{2137, 1550}, false, YELLOW},
        {{2520, 1350}, false, YELLOW},
        {{3150, 1075}, false, YELLOW},
        {{5800, 1075}, false, YELLOW}
    };


    //Loads texture for the bg image for the main menu
    Texture2D mainMenuTexture = LoadTexture("mainMenuImage.png");
    Vector2 menuTexturePosition = { (screenWidth - mainMenuTexture.width), (screenHeight - mainMenuTexture.height) };

    // Instantiation of text boxes for key bind settings
    Rectangle leftTextBox = { screenWidth / 2.0f - 100, 800, 500, 100 };
    bool leftMouseOn = false;

    Rectangle rightTextBox = { screenWidth / 2.0f - 100, 1000, 500, 100 };
    bool rightMouseOn = false;

    Rectangle jumpTextBox = { screenWidth / 2.0f - 100, 1200, 500, 100 };
    bool jumpMouseOn = false;

    // Reads saved key binds from file
    ifstream BindsFile("binds.txt");
    BindsFile >> leftBind;
    BindsFile >> rightBind;
    BindsFile >> jumpBind;
    BindsFile.close();

    //Instantisation for all buttons using the Button class
    Button quitButton("quitButtonUnpressedGray.png", "quitButtonUnpressed.png", "quitButtonPressed.png", 2, 1.3);
    Button settingsButton("settingsButtonUnpressedGray.png", "settingsButtonUnpressed.png", "settingsButtonPressed.png", 2, 1.8);
    Button playButton("greenButtonUnpressedGray.png", "greenButtonUnpressed.png", "greenButtonPressed.png", 2, 2.25);
    Button levelOneButton("greenButtonUnpressedGray.png", "greenButtonUnpressed.png", "greenButtonPressed.png", 3, 1.8);
    Button levelTwoButton("greenButtonUnpressedGray.png", "greenButtonUnpressed.png", "greenButtonPressed.png", 2, 1.8);
    Button levelThreeButton("greenButtonUnpressedGray.png", "greenButtonUnpressed.png", "greenButtonPressed.png", 1.5, 1.8);
    Button returnSettingsButton("quitButtonUnpressedGray.png", "quitButtonUnpressed.png", "quitButtonPressed.png", 20, 1.05);
    Button returnPauseButton("quitButtonUnpressedGray.png", "quitButtonUnpressed.png", "quitButtonPressed.png", 2, 1.2);
    Button returnGameButton("returnButtonUnpressedGray.png", "returnButtonUnpressed.png", "returnButtonPressed.png", 2, 1.5);

    SetTargetFPS(60);

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!close)    // Detect press of close button
    {
        float delta = GetFrameTime();

        //Hides the cursor when playing a level of the game
        if (currentScreen == GAME && !gamePaused) {
            HideCursor();
            DisableCursor();
        }
        else 
        {
            ShowCursor();
            EnableCursor();
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // switch case dependant around what screen should be drawn
            switch (currentScreen)
            {
                case MAIN:
                {
                    DrawTextureV(mainMenuTexture, menuTexturePosition, WHITE);
                    // Draws all main screen buttons
                    close = quitButton.draw();
                    if (settingsButton.draw()) currentScreen = SETTINGS;
                    if (playButton.draw())
                    {
                        currentScreen = LEVELSELECT;
                    }

                    DrawText("Play!", 1450, 875, 45, BLACK);
                    DrawText("Earth Climber", (29 * xMultiplier), (10 * yMultiplier), 200, LIGHTGRAY);
                } break;

                case LEVELSELECT:
                {
                    DrawTextureV(mainMenuTexture, menuTexturePosition, WHITE);
                    DrawText("Select Level", (30 * xMultiplier), (10 * yMultiplier), 200, LIGHTGRAY);
                    if (returnSettingsButton.draw()) currentScreen = MAIN;

                    // Selects level dependant on what button was selected
                    if (levelOneButton.draw())
                    {
                        levelSelected = 1;
                        currentScreen = GAME;
                    }
                    if (levelTwoButton.draw())
                    {
                        levelSelected = 2;
                        currentScreen = GAME;
                    }
                    if (levelThreeButton.draw())
                    {
                        levelSelected = 3;
                        currentScreen = GAME;
                    }

                    DrawText("1", 1025, 1070, 75, BLACK);
                    DrawText("2", 1480, 1070, 75, BLACK);
                    DrawText("3", 1950, 1070, 75, BLACK);

                    // Gets the size of the level array dependant on what level is selected
                    if (levelSelected == 1)
                    {
                        levelObjectsLength = sizeof(levelOneObjects) / sizeof(levelOneObjects[0]);
                        levelCoinsLength = sizeof(levelOneCoins);
                    }
                    else if (levelSelected == 2)
                    {
                        levelObjectsLength = sizeof(levelTwoObjects) / sizeof(levelTwoObjects[0]);
                        levelCoinsLength = sizeof(levelTwoCoins);
                    }
                    else
                    {
                        levelObjectsLength = sizeof(levelThreeObjects) / sizeof(levelThreeObjects[0]);
                        levelCoinsLength = sizeof(levelThreeCoins);
                    }

                } break;

                case GAME:
                {   
                    if (!gamePaused) {

                        //Moving left
                        if (IsKeyDown(leftBind))
                        {
                            player.moveLeft(true, delta);
                        }
                        else
                        {
                            player.moveLeft(false, delta);
                        }

                        //Moving right
                        if (IsKeyDown(rightBind))
                        {
                            player.moveRight(true, delta);
                        }
                        else
                        {
                            player.moveRight(false, delta);
                        }

                        //Jumping
                        if (IsKeyDown(jumpBind))
                        {
                            player.jump(true, delta);
                        }
                        else
                        {
                            player.jump(false, delta);
                        }

                        bool collide = false;
                        bool lowerThanTop = false;
                        bool collideFloor = false;
                        bool collideWall = false;

                        // Sets player postion and bounds so they dont have to be retrieved later
                        Vector2 playerPos = player.getPosition();
                        Rectangle playerBounds = player.getBounds();
                       
                        // use point and line collision line of game object and point on player, one point in bottom middle, left middle, right middle and left middle
                        for (int i = 0; i < levelObjectsLength; i++)
                        {
                            MapObject* v;
                            Coin* j;
                            if (levelSelected == 1)
                            {
                                v = levelOneObjects + i;
                                j = levelOneCoins + i;
                            }
                            else if (levelSelected == 2)
                            {
                                v = levelTwoObjects + i;
                                j = levelTwoCoins + i;
                            }
                            else
                            {
                                v = levelThreeObjects + i;
                                j = levelThreeCoins + i;
                            }

                            if (CheckCollisionCircleRec(j->getPosition(), coinRadius, playerBounds))
                            {
                                // Removes coin from level if collided with
                                if (!j->getBroken()) coinsCollected++;
                                j->setBroken(true);
                            }
                            if (v->getBlocks())
                            {
                                Rectangle objectBounds = v->getBounds();
                                //Checks if the player and current object are colliding
                                if (v->checkCollision(playerBounds)) 
                                {
                                    Rectangle collisionRect = GetCollisionRec(playerBounds, objectBounds);
                                    collide = true;
                                    if (playerBounds.y <= objectBounds.y) lowerThanTop = true;
                                    player.setSpeed(0.0);

                                    //If rectangle is breakable it will wait 20 frames until it breaks
                                    if (v->getColor().r == BROWN.r) {
                                        v->incrementBreakCounter();
                                        if (v->getBreakCounter() == 15) {
                                            v->setBroken(true);
                                            v->setBlocks(false);
                                            v->resetBreakCounter();
                                        }
                                    }

                                    //Collision with top side of an object
                                    if (collisionRect.y == objectBounds.y && lowerThanTop == true)
                                    {
                                        collideFloor = true;
                                        player.setPosition(playerPos.x, objectBounds.y - playerBounds.height);
                                    }
                                    //Collision with bottom side of an object
                                    if (collisionRect.y + collisionRect.height == objectBounds.y + objectBounds.height && lowerThanTop == true)
                                    {
                                        player.setPosition(playerPos.x, objectBounds.y + objectBounds.height);
                                    }
                                    //Collision with left side of an object
                                    if (collisionRect.x >= objectBounds.x && lowerThanTop == false)
                                    {
                                        collideWall = true;
                                        player.setCanRight(false);
                                        player.setPosition(objectBounds.x - playerBounds.width, playerPos.y);
                                    }
                                    //Collision with right side of an object
                                    if (collisionRect.x + collisionRect.width == objectBounds.x + objectBounds.width && lowerThanTop == false)
                                    {
                                        collideWall = true;
                                        player.setCanLeft(false);
                                        player.setPosition(objectBounds.x + objectBounds.width, playerPos.y);
                                    }

                                    //Check if the level object is meant to kill the player on contact
                                    if (v->getAttackOnCollision())
                                    {
                                        // Resets player
                                        player.setPosition(100, 1700);
                                        player.updateBounds();

                                        // Resets broken objects in level
                                        for (int i = 0; i < levelObjectsLength; i++) 
                                        {
                                            if (levelSelected == 1)
                                            {
                                                if (levelOneObjects[i].getBroken()) levelOneObjects[i].setBroken(false);
                                            }
                                            else if (levelSelected == 2)
                                            {
                                                if (levelTwoObjects[i].getBroken()) levelTwoObjects[i].setBroken(false);
                                            }
                                            else
                                            {
                                                if (levelThreeObjects[i].getBroken()) levelThreeObjects[i].setBroken(false);
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        // Completes level if flag collided with
                        if (levelOneFlag.checkCollision(playerBounds) || levelTwoFlag.checkCollision(playerBounds) || levelThreeFlag.checkCollision(playerBounds))
                        {
                            gamePaused = true;
                            levelComplete = true;
                        }

                        //Only goes here if the player isnt colliding with any objects,
                        //such as when it is in the air
                        if (!collide) player.gravity(delta);

                        else if (collideWall) player.resetAccellerator(2);

                        else if (collideFloor) {
                            player.resetAccellerator(1);
                            player.setCanJump(true);
                            player.setCanLeft(true);
                            player.setCanRight(true);
                        }

                        //updates the bounds of the player after all player position updates
                        player.updateBounds();
                    }

                    // Allows for drawing within the camera space
                    BeginMode2D(camera);
                        
                    // Iterator for drawing objects dependant on level, does not draw the objects if player is too far away
                        for (int i = 0; i < levelObjectsLength; i++) {
                            if (levelSelected == 1)
                            {
                                levelOneCoins[i].draw();
                                if (levelOneObjects[i].getBounds().x - player.getPosition().x < 1475
                                    && -(levelOneObjects[i].getBounds().x + levelOneObjects[i].getBounds().width) + player.getPosition().x < 1475
                                    || levelOneObjects[i].getBounds().y - player.getPosition().y < 300
                                    && -(levelOneObjects[i].getBounds().y + levelOneObjects[i].getBounds().height) + player.getPosition().y < 300)
                                {
                                    levelOneObjects[i].draw();
                                }
                            }
                            else if (levelSelected == 2)
                            {
                                levelTwoCoins[i].draw();
                                if (levelTwoObjects[i].getBounds().x - player.getPosition().x < 1475
                                    && -(levelTwoObjects[i].getBounds().x + levelTwoObjects[i].getBounds().width) + player.getPosition().x < 1475
                                    || levelTwoObjects[i].getBounds().y - player.getPosition().y < 300
                                    && -(levelTwoObjects[i].getBounds().y + levelTwoObjects[i].getBounds().height) + player.getPosition().y < 300)
                                {
                                    levelTwoObjects[i].draw();
                                }
                            }
                            else
                            {
                                levelThreeCoins[i].draw();
                                if (levelThreeObjects[i].getBounds().x - player.getPosition().x < 1475
                                    && -(levelThreeObjects[i].getBounds().x + levelThreeObjects[i].getBounds().width) + player.getPosition().x < 1475
                                    || levelThreeObjects[i].getBounds().y - player.getPosition().y < 300
                                    && -(levelThreeObjects[i].getBounds().y + levelThreeObjects[i].getBounds().height) + player.getPosition().y < 300)
                                {
                                    levelThreeObjects[i].draw();
                                }
                            }
                        }
                        // Draws flag depedant on level
                        if (levelSelected == 1)
                        {
                            levelOneFlag.draw();
                        }
                        else if (levelSelected == 2)
                        {
                            levelTwoFlag.draw();
                        }
                        else
                        {
                            levelThreeFlag.draw();
                        }
                        player.draw();
                        // Call for camera position to be updated
                        UpdateCameraEven(&camera, &player, delta);

                    EndMode2D();

                        if (IsKeyPressed(KEY_ESCAPE) && !levelComplete) {
                            gamePaused = !gamePaused;
                        }
                        if (gamePaused || levelComplete) {
                            DrawRectangle(0, 0, 3000, 2000, pauseMenuAlpha);

                            if (returnGameButton.draw()) 
                            {   
                                // Level replay
                                if (levelComplete)
                                {
                                    player.setPosition(100, 1700);
                                    player.updateBounds();
                                    coinsCollected = 0;

                                    // Resets level
                                    for (int i = 0; i < levelObjectsLength; i++)
                                    {
                                        if (levelSelected == 1)
                                        {
                                            if (levelOneObjects[i].getBroken()) levelOneObjects[i].setBroken(false);
                                            if (levelOneCoins[i].getBroken()) levelOneCoins[i].setBroken(false);
                                        }
                                        else if (levelSelected == 2)
                                        {
                                            if (levelTwoObjects[i].getBroken()) levelTwoObjects[i].setBroken(false);
                                            if (levelTwoCoins[i].getBroken()) levelTwoCoins[i].setBroken(false);
                                        }
                                        else
                                        {
                                            if (levelThreeObjects[i].getBroken()) levelThreeObjects[i].setBroken(false);
                                            if (levelThreeCoins[i].getBroken()) levelThreeCoins[i].setBroken(false);
                                        }
                                    }
                                }
                                gamePaused = false;
                                levelComplete = false;
                            }

                            // Returns to main menu
                            if (returnPauseButton.draw()) 
                            {
                                gamePaused = false;
                                levelComplete = false;
                                coinsCollected = 0;
                                currentScreen = MAIN;

                                // Resets level
                                for (int i = 0; i < levelObjectsLength; i++) 
                                {
                                    if (levelSelected == 1)
                                    {
                                        if (levelOneObjects[i].getBroken()) levelOneObjects[i].setBroken(false);
                                        if (levelOneCoins[i].getBroken()) levelOneCoins[i].setBroken(false);
                                    }
                                    else if (levelSelected == 2)
                                    {
                                        if (levelTwoObjects[i].getBroken()) levelTwoObjects[i].setBroken(false);
                                        if (levelTwoCoins[i].getBroken()) levelTwoCoins[i].setBroken(false);
                                    }
                                    else
                                    {
                                        if (levelThreeObjects[i].getBroken()) levelThreeObjects[i].setBroken(false);
                                        if (levelThreeCoins[i].getBroken()) levelThreeCoins[i].setBroken(false);
                                    }
                                }
                                
                                player.setPosition(100, 1700);
                                player.updateBounds();
                            }
                            // Draws title dependant on either game paused or level complete
                            if (gamePaused && !levelComplete)
                            {
                                DrawText("Paused", 1150, 200, 200, LIGHTGRAY);
                            }
                            else
                            {
                                DrawText("Complete", 1050, 200, 200, GREEN);
                            }
                        }
                        else 
                        {
                            // Coins HUD at the top of the level
                            DrawRectangle(0, 0, 3000, 200, pauseMenuAlpha);

                            DrawText(TextFormat("Coins: %02i", coinsCollected), 1400, 65, 60, LIGHTGRAY);
                        } 
                } break;

                case SETTINGS:
                {
                    DrawTextureV(mainMenuTexture, menuTexturePosition, WHITE);
                    DrawText("Settings", (37 * xMultiplier), (10 * yMultiplier), 200, LIGHTGRAY);
                    if (returnSettingsButton.draw()) currentScreen = MAIN;

                    // Reads key ascii value key binds from file
                    ifstream BindsFile("binds.txt");
                    BindsFile >> leftBind;
                    BindsFile >> rightBind;
                    BindsFile >> jumpBind;
                    BindsFile.close();

                    // Translates ascii values into character so it can be displayed to the user in the text box
                    leftText = (char)leftBind;
                    rightText = (char)rightBind;
                    jumpText = (char)jumpBind;

                    if (CheckCollisionPointRec(GetMousePosition(), leftTextBox)) leftMouseOn = true;
                    else leftMouseOn = false;

                    if (CheckCollisionPointRec(GetMousePosition(), rightTextBox)) rightMouseOn = true;
                    else rightMouseOn = false;

                    if (CheckCollisionPointRec(GetMousePosition(), jumpTextBox)) jumpMouseOn = true;
                    else jumpMouseOn = false;

                    if (leftMouseOn || rightMouseOn || jumpMouseOn)
                    {
                        SetMouseCursor(MOUSE_CURSOR_IBEAM);

                        key = GetCharPressed();
                        if (key > 0)
                        {
                            // Only if it is a lowercase letter
                            if (key >= 97 && key <= 122)
                            {
                                // Makes it uppercase
                                key = key - 32;
                                tempText = (char)key;
                            }

                            if (leftMouseOn)
                            {
                                leftText = tempText;
                                leftBind = key;
                            }
                            if (rightMouseOn)
                            {
                                rightText = tempText;
                                rightBind = key;
                            }
                            if (jumpMouseOn)
                            {
                                jumpText = tempText;
                                jumpBind = key;
                            }
                        }

                        // Writes new binds to file
                        ofstream WriteFile("binds.txt");
                        WriteFile << leftBind << "\n";
                        WriteFile << rightBind << "\n";
                        WriteFile << jumpBind << "\n";
                        WriteFile.close();
                    }
                    else 
                    {
                        SetMouseCursor(1);
                        tempText = "\0";
                    }

                    // Drawing for input boxes
                    DrawRectangleRec(leftTextBox, WHITE);
                    if (leftMouseOn) DrawRectangleLinesEx(leftTextBox, 5, GREEN);
                    else DrawRectangleLinesEx(leftTextBox, 5, DARKGRAY);
                    DrawText(leftText.c_str(), leftTextBox.x + 220, leftTextBox.y + 15, 80, DARKGRAY);

                    DrawRectangleRec(rightTextBox, WHITE);
                    if (rightMouseOn) DrawRectangleLinesEx(rightTextBox, 5, GREEN);
                    else DrawRectangleLinesEx(rightTextBox, 5, DARKGRAY);
                    DrawText(rightText.c_str(), rightTextBox.x + 220, rightTextBox.y + 15, 80, DARKGRAY);

                    DrawRectangleRec(jumpTextBox, WHITE);
                    if (jumpMouseOn) DrawRectangleLinesEx(jumpTextBox, 5, GREEN);
                    else DrawRectangleLinesEx(jumpTextBox, 5, DARKGRAY);
                    if (jumpBind == 32) DrawText("SPACE", jumpTextBox.x + 110, jumpTextBox.y + 15, 80, DARKGRAY);
                    else DrawText(jumpText.c_str(), jumpTextBox.x + 220, jumpTextBox.y + 15, 80, DARKGRAY);

                    DrawText("Left:", leftTextBox.x - 300, leftTextBox.y + 10, 100, LIGHTGRAY);
                    DrawText("Right:", rightTextBox.x - 300, rightTextBox.y + 10, 100, LIGHTGRAY);
                    DrawText("Jump:", jumpTextBox.x - 300, jumpTextBox.y + 10, 100, LIGHTGRAY);
                } break;
                default: break;
            }
             
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}