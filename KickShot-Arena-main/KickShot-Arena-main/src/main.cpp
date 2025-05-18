#include <raylib.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include "GameObject.hpp"
#include "Ball.hpp"
#include "Palet.hpp"
using namespace std;
#define NUM_PLAYERS 10

// screen size
const int screenWidth = 1280;
const int screenHeight = 850;

// constants for start and selection screens
const int numTeams = 3;
const int numFormations = 3;

// Power bar constants
const int POWER_BAR_WIDTH = 200;
const int POWER_BAR_HEIGHT = 20;
const int POWER_BAR_MAX = 100;

float powerLevel = 50.0f; // Initial power level

bool player1_scored = false; // Flag to check if player1 scored
bool player2_scored = false; // Flag to check if player2 scored
float goal_display_time = 0; // Timer to display goal message
bool player1_wins = false;   // Flag to check if player1 wins
bool player2_wins = false;   // Flag to check if player2 wins

// For Attempt exchange
int attemptNo = 1;

enum ScreenState
{
    TITLE_SCREEN,
    SELECTION_SCREEN,
    GAME
};

struct Formation
{
    const char *designation;
    Vector2 teamAlpha[5];
    Vector2 teamBeta[5];
};

// array to store formation sets
Formation formations[] = {
    {"1-2-2",
     {{148, 400}, {345, 200}, {345, 600}, {541.5, 300}, {541.5, 500}},
     {{1132, 400}, {935, 200}, {935, 600}, {738.5, 300}, {738.5, 500}}},
    {"1-3-1",
     {{148, 400}, {345, 200}, {345, 400}, {345, 600}, {541.5, 400}},
     {{1132, 400}, {935, 200}, {935, 400}, {935, 600}, {738.5, 400}}},
    {"1-2-1-1",
     {{148, 400}, {246, 200}, {246, 600}, {394, 400}, {541.5, 400}},
     {{1132, 400}, {1034, 200}, {1034, 600}, {886, 400}, {738.5, 400}}}};

// Function to load user-selected Textures of palets
void LoadSelectedTextures(int teamATextureChoice, int teamBTextureChoice)
{
    // since we cant set the textures NULL therefore we initialized them with the default path
}
// Function to initialize players based on formation
void initializePlayers(vector<Palet> &players, Texture2D teamATexture, Texture2D teamBTexture, int teamAFormation, int teamBFormation)
{
    players.clear();

    for (int i = 0; i < 5; ++i)
    {
        players.push_back(Palet(formations[teamAFormation].teamAlpha[i].x, formations[teamAFormation].teamAlpha[i].y, 42.5, teamATexture));
    }

    for (int i = 0; i < 5; ++i)
    {
        players.push_back(Palet(formations[teamBFormation].teamBeta[i].x, formations[teamBFormation].teamBeta[i].y, 42.5, teamBTexture));
    }
}

// Collision detection function for goalposts
void CheckGoalPostCollision(Ball &ball, vector<Palet> &palets, Sound goalSound, Sound winningSound)
{
    // GoalPost on right boundary
    if (ball.position.x > (1260 - ball.radius)) // GoalPost Collision
    {
        PlaySound(goalSound);
        // reset positions of palets and ball
        ball.Reset(); // reset ball's position
        for (auto &palet : palets)
        {
            palet.Reset();
        }
        // update score of player 1 and its flags
        GameObject::player1_score++;
        player1_scored = true;    // Set flag
        goal_display_time = 3.0f; // Set timer to 3 seconds

        // check for win
        if (GameObject::player1_score == 3)
        {
            PlaySound(winningSound);
            player1_wins = true;
        }
    }
    if (ball.position.x < (ball.radius + 20)) // GoalPost Collision
    {
        PlaySound(goalSound);
        // reset positions of palets and ball
        ball.Reset(); // ball reset
        for (auto &palet : palets)
        {
            palet.Reset();
        }

        // update score of player 2 and its flags
        GameObject::player2_score++;
        player2_scored = true;    // Set flag
        goal_display_time = 3.0f; // Set timer to 3 seconds
        // check for win
        if (GameObject::player2_score == 3)
        {
            PlaySound(winningSound);
            player2_wins = true;
        }
    }
}

// Finalized Collision Physics b/w ball and palets
void collision(GameObject &object1, GameObject &object2, Sound collisionSound)
{
    float dx = object2.position.x - object1.position.x;
    float dy = object2.position.y - object1.position.y;
    float dist = sqrt(dx * dx + dy * dy);

    if (dist < object1.radius + object2.radius)
    {
        PlaySound(collisionSound);
        float angle = atan2(dy, dx);
        float sinAngle = sin(angle), cosAngle = cos(angle);

        // Rotate ball 1's position
        float x1 = 0, y1 = 0;
        // Rotate ball 2's position
        float x2 = dx * cosAngle + dy * sinAngle;
        float y2 = dy * cosAngle - dx * sinAngle;

        // Rotate ball 1's velocity
        float vx1 = object1.speed.x * cosAngle + object1.speed.y * sinAngle;
        float vy1 = object1.speed.y * cosAngle - object1.speed.x * sinAngle;
        // Rotate ball 2's velocity
        float vx2 = object2.speed.x * cosAngle + object2.speed.y * sinAngle;
        float vy2 = object2.speed.y * cosAngle - object2.speed.x * sinAngle;

        // Collision response with equal masses
        float vx1Final = vx2;
        float vx2Final = vx1;

        vx1 = vx1Final;
        vx2 = vx2Final;

        // Update positions to avoid overlap
        float absV = fabs(vx1) + fabs(vx2);
        float overlap = (object1.radius + object2.radius) - fabs(x1 - x2);
        x1 += vx1 / absV * overlap;
        x2 += vx2 / absV * overlap;

        // Rotate positions back
        float x1Final = x1 * cosAngle - y1 * sinAngle;
        float y1Final = y1 * cosAngle + x1 * sinAngle;
        float x2Final = x2 * cosAngle - y2 * sinAngle;
        float y2Final = y2 * cosAngle + x2 * sinAngle;

        // Update positions
        object2.position.x = object1.position.x + x2Final;
        object2.position.y = object1.position.y + y2Final;
        object1.position.x = object1.position.x + x1Final;
        object1.position.y = object1.position.y + y1Final;

        // Rotate velocities back
        object1.speed.x = vx1 * cosAngle - vy1 * sinAngle;
        object1.speed.y = vy1 * cosAngle + vx1 * sinAngle;
        object2.speed.x = vx2 * cosAngle - vy2 * sinAngle;
        object2.speed.y = vy2 * cosAngle + vx2 * sinAngle;
    }
}

// power bar to determine the power of ball at the time of release
void DrawPowerBar()
{
    int screenWidth = GetScreenWidth();
    int powerBarX = (screenWidth - POWER_BAR_WIDTH) / 2;
    int powerBarY = GetScreenHeight() - POWER_BAR_HEIGHT - 10; // 10 pixels margin

    DrawRectangle(powerBarX, powerBarY, POWER_BAR_WIDTH, POWER_BAR_HEIGHT, GRAY);
    DrawRectangle(powerBarX, powerBarY, (int)(POWER_BAR_WIDTH * (powerLevel / POWER_BAR_MAX)), POWER_BAR_HEIGHT, BLUE);
    DrawRectangleLines(powerBarX, powerBarY, POWER_BAR_WIDTH, POWER_BAR_HEIGHT, BLACK);
}

int main()
{
    // Initialization
    InitWindow(screenWidth, screenHeight, "KickShot Arena: A 2D Game");

    InitAudioDevice(); // Initialize audio device

    // Load sound effects and music
    Sound collisionSound = LoadSound("Sounds/hitting ball.mp3");
    Sound collisionSoundPalets = LoadSound("Sounds/collision bw palletes.mp3");
    Music backgroundMusic = LoadMusicStream("Sounds/backgroundmusic.mp3");
    Sound goalSound = LoadSound("Sounds/Goal 1.mp3");
    Sound winningSound = LoadSound("Sounds/Winning.mp3");

    PlayMusicStream(backgroundMusic);
    

    // loading textures
    Texture2D bgLogo = LoadTexture("Graphics/logo.png");
    Texture2D ballTexture = LoadTexture("Graphics/ball.png");
    Texture2D arrowTexture = LoadTexture("Graphics/arrow.png");
    Texture2D bgTexture = LoadTexture("Graphics/bg.png");
    Texture2D goalTexture = LoadTexture("Graphics/goal.png");
    Texture2D teamAscore = LoadTexture("Graphics/teamA_scored.png");
    Texture2D teamBscore = LoadTexture("Graphics/teamB_scored.png");
    Texture2D teamAwins = LoadTexture("Graphics/teamA_wins.png");
    Texture2D teamBwins = LoadTexture("Graphics/teamB_wins.png");

    // Arrays to hold image paths
    array<string, numTeams> teamAImagePaths = {"Graphics/team_blue.png", "Graphics/team_lion.png", "Graphics/team_argentina.png"}; // for player 2
    array<string, numTeams> teamBImagePaths = {"Graphics/team_red.png", "Graphics/team_wolf.png", "Graphics/team_portugal.png"};   // for player 1
    array<string, numFormations> formationImagePaths = {"Graphics/1-2-2.png", "Graphics/1-3-1.png", "Graphics/1-2-1-1.png"};

    // Load images for teams and formations
    array<Texture2D, numTeams> teamAImages; // for team alpha
    array<Texture2D, numTeams> teamBImages; // for team beta
    array<Texture2D, numFormations> formationImages;

    for (int i = 0; i < numTeams; i++)
    { // loading images for team alpha
        teamBImages[i] = LoadTexture(teamBImagePaths[i].c_str());
    }

    for (int i = 0; i < numTeams; i++)
    { // loading images for team beta
        teamAImages[i] = LoadTexture(teamAImagePaths[i].c_str());
    }

    for (int i = 0; i < numFormations; i++)
    {
        formationImages[i] = LoadTexture(formationImagePaths[i].c_str());
    }

    int teamATextureChoice = -1, teamAFormation = -1;
    int teamBTextureChoice = -1, teamBFormation = -1;
    Texture2D teamATexture,teamBTexture;

    // initializing players using vectors
    vector<Palet> players;
    // setting the team formations

    Ball soccerBall(screenWidth / 2, 400, 22.5, ballTexture);
    // variables for controlling the palet
    int controlledPaletIndex = -1;
    bool isDragging = false;

    ScreenState currentScreen = TITLE_SCREEN;

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {

        UpdateMusicStream(backgroundMusic); // Update the music stream
        
        if (currentScreen == TITLE_SCREEN)
        {
            
            
            if (IsKeyPressed(KEY_SPACE))
            {
                currentScreen = SELECTION_SCREEN;
            }

            BeginDrawing();

            ClearBackground(LIME); // TITLE BACKGROUND

            // Draw the background image
            DrawTexture(bgLogo, 0, 0, WHITE);

            // Draw TITLE screen elements
            DrawText("PRESS Space To Enter The Game", 340, 800, 35, BLACK);

            EndDrawing();
        }
        else if (currentScreen == SELECTION_SCREEN)
        {
            BeginDrawing();
            ClearBackground(LIME); // selection background

            // Draw the partition line
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, BLACK);

            // Draw player labels
            DrawText("Player 1", screenWidth / 4 - MeasureText("Player 1", 40) / 2, 20, 40, BLACK);
            DrawText("Player 2", 3 * screenWidth / 4 - MeasureText("Player 2", 40) / 2, 20, 40, BLACK);

            // Draw "Select team:" and "Select formation:" for Player 1
            DrawText("Select team:", screenWidth / 8 - MeasureText("Select team:", 20) / 2, 60, 20, BLACK);
            DrawText("Select formation:", 420 - MeasureText("Select formation:", 20) / 2, 60, 20, BLACK);

            // Draw team images for Player 1
            for (int i = 0; i < numTeams; i++)
            {
                float x = screenWidth / 8 - teamAImages[i].width / 2;
                float y = 90 + i * (teamAImages[i].height + 10);
                DrawTexture(teamAImages[i], x, y, WHITE);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {x, y, (float)teamAImages[i].width, (float)teamAImages[i].height}))
                {
                    teamATextureChoice = i;
                }
            }

            // Draw formation images for Player 1
            for (int i = 0; i < numFormations; i++)
            {
                float x = 420;
                float y = 90 + i * (formationImages[i].height + 10);
                DrawTexture(formationImages[i], x, y, WHITE);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {x, y, (float)formationImages[i].width, (float)formationImages[i].height}))
                {
                    teamAFormation = i;
                }
            }

            // Draw "Select team:" and "Select formation:" for Player 2
            DrawText("Select team:", 5 * screenWidth / 8 - MeasureText("Select team:", 20) / 2, 60, 20, BLACK);
            DrawText("Select formation:", 1080 - MeasureText("Select formation:", 20) / 2, 60, 20, BLACK);

            // Draw team images for Player 2
            for (int i = 0; i < numTeams; i++)
            {
                float x = 5 * screenWidth / 8 - teamBImages[i].width / 2;
                float y = 90 + i * (teamBImages[i].height + 10);
                DrawTexture(teamBImages[i], x, y, WHITE);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {x, y, (float)teamBImages[i].width, (float)teamBImages[i].height}))
                {
                    teamBTextureChoice = i;
                }
            }

            // Draw formation images for Player 2
            for (int i = 0; i < numFormations; i++)
            {
                float x = 1080;
                float y = 90 + i * (formationImages[i].height + 10);
                DrawTexture(formationImages[i], x, y, WHITE);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {x, y, (float)formationImages[i].width, (float)formationImages[i].height}))
                {
                    teamBFormation = i;
                }
            }

            // Display selected team and formation images and text for Player 1
            if (teamATextureChoice != -1)
            {
                int team1X = screenWidth / 4 - teamAImages[teamATextureChoice].width / 2;
                int team1Y = screenHeight - teamAImages[teamATextureChoice].height - 150;
                DrawTexture(teamAImages[teamATextureChoice], 120, team1Y, WHITE);
                DrawText("Selected Team:", 100, team1Y - 20, 20, BLACK);

                if (teamAFormation != -1)
                {
                    DrawTexture(formationImages[teamAFormation], team1X + teamAImages[teamATextureChoice].width + 10, team1Y, WHITE);
                    DrawText("Selected Formation:", team1X + teamAImages[teamATextureChoice].width + 10 + formationImages[teamAFormation].width / 2 - MeasureText("Selected Formation", 20) / 2, team1Y - 20, 20, BLACK);
                }
            }

            // Display selected team and formation images and text for Player 2
            if (teamBTextureChoice != -1)
            {
                int team2X = 3 * screenWidth / 4 - teamBImages[teamBTextureChoice].width / 2;
                int team2Y = screenHeight - teamBImages[teamBTextureChoice].height - 150;
                DrawTexture(teamBImages[teamBTextureChoice], 720, team2Y, WHITE);
                DrawText("Selected Team:", 700, team2Y - 20, 20, BLACK);

                if (teamBFormation != -1)
                {
                    DrawTexture(formationImages[teamBFormation], team2X + teamBImages[teamBTextureChoice].width + 10, team2Y, WHITE);
                    DrawText("Selected Formation:", team2X + teamBImages[teamBTextureChoice].width + 10 + formationImages[teamBFormation].width / 2 - MeasureText("Selected Formation", 20) / 2, team2Y - 20, 20, BLACK);
                }
            }
            // Since we now have all the user choices therefore we initialize our palets
            teamATexture = LoadTexture("Graphics/team_blue.png");
            teamBTexture = LoadTexture("Graphics/team_red.png");

            // determining the teams textures according to player's choice
            switch (teamATextureChoice)
            {
            case 0:
                teamATexture = LoadTexture("Graphics/team_blue.png");
                break;

            case 1:
                teamATexture = LoadTexture("Graphics/team_lion.png");
                break;

            case 2:
                teamATexture = LoadTexture("Graphics/team_argentina.png");
                break;
            }

            switch (teamBTextureChoice)
            {
            case 0:
                teamBTexture = LoadTexture("Graphics/team_red.png");
                break;

            case 1:
                teamBTexture = LoadTexture("Graphics/team_wolf.png");
                break;

            case 2:
                teamBTexture = LoadTexture("Graphics/team_portugal.png");
                break;
            }
            // initialization of texture ends here
            initializePlayers(players, teamATexture, teamBTexture, teamAFormation, teamBFormation);
            // Draw Play Game button
            int buttonWidth = 150;
            int buttonHeight = 50;
            int buttonX = screenWidth - buttonWidth - 20;
            int buttonY = screenHeight - buttonHeight - 20;
            DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, GREEN);
            DrawText("Play Game", buttonX + buttonWidth / 2 - MeasureText("Play Game", 20) / 2, buttonY + buttonHeight / 2 - 10, 20, BLACK);

            // Check if Play Game button is clicked
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight}))
            {
                currentScreen = GAME; // Game Code Entered Here
            }
            EndDrawing();
        }

        else if (currentScreen == GAME)
        {
            
            
            // Update section of Game Loop starts here

            Vector2 mousePosition = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                for (size_t i = 0; i < players.size(); i++)
                {
                    // if attemptNo is odd -> team one turn else if attemptNo is odd -> team two turn
                    if (((attemptNo % 2 == 1) && (i >= 5)) || ((attemptNo % 2 == 0) && (i < 5)))
                    // total players 10 but 0,1,2,3,4 belongs to team 1 and 5,6,7,8,9 belongs to team 2
                    {
                        if (CheckCollisionPointCircle(mousePosition, {players[i].position.x, players[i].position.y}, players[i].radius))
                        {
                            controlledPaletIndex = i;
                            isDragging = true;
                            break;
                        }
                    }
                }
            }

            if (isDragging)
            {
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    // Launch the ball
                    float dx = mousePosition.x - players[controlledPaletIndex].position.x;
                    float dy = mousePosition.y - players[controlledPaletIndex].position.y;
                    float angle = atan2(dy, dx) + PI;
                    float speed = powerLevel / POWER_BAR_MAX * 50.0f; // Scale speed by power level
                    players[controlledPaletIndex].speed.x = cos(angle) * speed;
                    players[controlledPaletIndex].speed.y = sin(angle) * speed;
                    ;

                    // Set these variables to false for next move
                    isDragging = false;
                    controlledPaletIndex = -1;

                    // switch turns after the move
                    attemptNo++;
                }
            }

            // Update power level with the arrow keys
            if (IsKeyDown(KEY_RIGHT))
                powerLevel += 1.0f;
            if (IsKeyDown(KEY_LEFT))
                powerLevel -= 1.0f;

            // Clamp power level between 0 and POWER_BAR_MAX
            if (powerLevel > POWER_BAR_MAX)
                powerLevel = POWER_BAR_MAX;
            if (powerLevel < 0)
                powerLevel = 0;

            // Update the positions of ball and players
            soccerBall.Update();
            for (auto &player : players)
            {
                player.Update();
            }

            // Decrease goal display timer
            if (goal_display_time > 0)
            {
                goal_display_time -= GetFrameTime();
            }
            else
            {
                player1_scored = false;
                player2_scored = false;
            }

            // Check for collision b/w ball and goalpost
            CheckGoalPostCollision(soccerBall, players, goalSound, winningSound);

            // Check collisions between players and the soccer ball
            for (auto &player : players)
            {
                collision(player, soccerBall, collisionSound);
            }

            // Check collisions between players
            for (size_t i = 0; i < players.size(); ++i)
            {
                for (size_t j = i + 1; j < players.size(); ++j)
                {
                    collision(players[i], players[j], collisionSoundPalets);
                }
            }

            // Draw
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Draw the scaled background
            DrawTexturePro(bgTexture,
                           Rectangle{0, 0, (float)bgTexture.width, (float)bgTexture.height},
                           Rectangle{0, 0, (float)screenWidth, (float)screenHeight},
                           Vector2{0, 0},
                           0.0f,
                           WHITE);

            DrawPowerBar();

            soccerBall.Draw();
            for (auto &player : players)
            {
                player.Draw();
            }

            DrawTexturePro(goalTexture,
                           Rectangle{0, 0, (float)bgTexture.width, (float)bgTexture.height},
                           Rectangle{0, 0, (float)screenWidth, (float)screenHeight},
                           Vector2{0, 0},
                           0.0f,
                           WHITE);

            // Draw the arrow if a ball is selected
            if (isDragging)
            {
                float dx = mousePosition.x - players[controlledPaletIndex].position.x;
                float dy = mousePosition.y - players[controlledPaletIndex].position.y;
                float angle = atan2(dy, dx) + PI;
                float arrowLength = 100.0f;
                // Calculate the starting point of the arrow at the circumference of the ball
                Vector2 arrowStart = {
                    players[controlledPaletIndex].position.x + (players[controlledPaletIndex].radius + 5) * cos(angle),
                    players[controlledPaletIndex].position.y + (players[controlledPaletIndex].radius + 5) * sin(angle)};

                // Draw the arrow using the loaded texture
                DrawTexturePro(
                    arrowTexture,
                    Rectangle{0, 0, (float)arrowTexture.width, (float)arrowTexture.height},         // Source rectangle (full texture)
                    Rectangle{arrowStart.x, arrowStart.y, arrowLength, (float)arrowTexture.height}, // Destination rectangle
                    Vector2{0, (float)arrowTexture.height / 2},                                     // Origin (centered origin)
                    angle * RAD2DEG,                                                                // Rotation angle
                    WHITE);
            }
            // Draw player scores
            DrawText(TextFormat("%d", GameObject::player1_score), 420, 810, 30, WHITE);
            DrawText(TextFormat("%d", GameObject::player2_score), 840, 810, 30, WHITE);

            // Draw goal message
            if (player1_scored)
            {
                DrawTexture(teamAscore, 390, 269, WHITE);
            }
            else if (player2_scored)
            {
                DrawTexture(teamBscore, 390, 269, WHITE);
            }

            // Draw winning message
            if (player1_wins)
            {
                DrawTexture(teamAwins, 390, 269, WHITE);
            }
            else if (player2_wins)
            {
                DrawTexture(teamBwins, 390, 269, WHITE);
            }

            EndDrawing();
        }
    }

    // Cleanup
    for (int i = 0; i < numTeams; i++)
    {
        UnloadTexture(teamAImages[i]);
    }
    for (int i = 0; i < numTeams; i++)
    {
        UnloadTexture(teamBImages[i]);
    }
    for (int i = 0; i < numFormations; i++)
    {
        UnloadTexture(formationImages[i]);
    }
    UnloadTexture(bgLogo);
    UnloadTexture(teamATexture);
    UnloadTexture(teamBTexture);
    UnloadTexture(teamAscore);
    UnloadTexture(teamBscore);
    UnloadTexture(teamAwins);
    UnloadTexture(teamBwins);
    UnloadTexture(bgTexture);
    UnloadTexture(ballTexture);
    UnloadTexture(arrowTexture);

    UnloadSound(goalSound);
    UnloadSound(collisionSound);
    UnloadSound(collisionSoundPalets);
    UnloadSound(winningSound);
    CloseWindow();

    return 0;
}
//done and dusted