#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;

#include "olcConsoleGameEngine.h"

class OneLoneCoder_Asteroids : public olcConsoleGameEngine
{
public:
    enum class Difficulty
    {
        Easy,
        Medium,
        Hard
    };

    OneLoneCoder_Asteroids()
    {
        m_sAppName = L"Asteroids";
        difficulty = Difficulty::Medium;
        nAsteroidsCreated = 0;
    }

    void SetDifficulty(Difficulty newDifficulty)
    {
        difficulty = newDifficulty;
    }

private:
    Difficulty difficulty;
    enum class GameState
    {
        StartScreen,
        Playing,
        GameOver
    };

    GameState gameState;

    struct sSpaceObject
    {
        int nSize;
        float x;
        float y;
        float dx;
        float dy;
        float angle;
        bool destroyed;
    };

    vector<sSpaceObject> vecAsteroids;
    vector<sSpaceObject> vecBullets;
    sSpaceObject player;
    bool bDead = false;
    int nScore = 0;
    int nLives;
    int nLevel = 1;
    int nAsteroidsLeft;
    int nAsteroidsCreated;

    vector<pair<float, float>> vecModelShip;
    vector<pair<float, float>> vecModelAsteroid;

protected:
    virtual bool OnUserCreate()
    {
        gameState = GameState::StartScreen;
        nAsteroidsLeft = 0;

        vecModelShip =
        {
            {0.0f, -5.0f},
            {-2.5f, +2.5f},
            {+2.5f, +2.5f}
        };

        int verts = 20;
        for (int i = 0; i < verts; i++)
        {
            float noise = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;
            vecModelAsteroid.push_back(make_pair(noise * sinf(((float)i / (float)verts) * 6.28318f),
                noise * cosf(((float)i / (float)verts) * 6.28318f)));
        }

        return true;
    }

    void ResetGame()
    {
        player.x = ScreenWidth() / 2.0f;
        player.y = ScreenHeight() / 2.0f;
        player.dx = 0.0f;
        player.dy = 0.0f;
        player.angle = 0.0f;

        vector<sSpaceObject> initialAsteroids = vecAsteroids;

        vecBullets.clear();
        vecAsteroids.clear();

        int numAsteroids = 0;
        int asteroidSpeed = 0;

        switch (difficulty)
        {
        case Difficulty::Easy:
            numAsteroids = 2 + nLevel * 0.05;
            asteroidSpeed = 8 + nLevel * 0.05;
            break;

        case Difficulty::Medium:
            numAsteroids = 3 + nLevel * 0.05;
            asteroidSpeed = 12 + nLevel * 0.05;
            break;

        case Difficulty::Hard:
            numAsteroids = 6 + nLevel * 0.05;
            asteroidSpeed = 16 + nLevel * 0.05;
            break;
        }

        for (const auto& asteroid : initialAsteroids)
        {
            if (!asteroid.destroyed)
            {
                vecAsteroids.push_back(asteroid);
            }
        }

        int initialAsteroidsCount = vecAsteroids.size();
        for (int i = 0; i < numAsteroids - initialAsteroidsCount; i++)
        {
            float asteroidX = rand() % ScreenWidth();
            float asteroidY = rand() % ScreenHeight();
            while (IsPointInsideCircle(player.x, player.y, 50, asteroidX, asteroidY))
            {
                asteroidX = rand() % ScreenWidth();
                asteroidY = rand() % ScreenHeight();
            }
            vecAsteroids.push_back({ 16, asteroidX, asteroidY, (float)asteroidSpeed, -(float)asteroidSpeed, 0.0f, false });

            // Increment the count of created asteroids
            nAsteroidsCreated++;
        }

        bDead = false;
        nScore = 0;
        nLives = 3;
        nAsteroidsLeft = vecAsteroids.size();
    }

    void WrapCoordinates(float ix, float iy, float& ox, float& oy)
    {
        ox = ix;
        oy = iy;
        if (ix < 0.0f)
            ox = ix + (float)ScreenWidth();
        if (ix >= (float)ScreenWidth())
            ox = ix - (float)ScreenWidth();
        if (iy < 0.0f)
            oy = iy + (float)ScreenHeight();
        if (iy >= (float)ScreenHeight())
            oy = iy - (float)ScreenHeight();
    }

    virtual void Draw(int x, int y, wchar_t c = 0x2588, short col = 0x000F)
    {
        float fx, fy;
        WrapCoordinates(x, y, fx, fy);
        olcConsoleGameEngine::Draw(fx, fy, c, col);
    }

    bool IsPointInsideCircle(float cx, float cy, float radius, float x, float y)
    {
        return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) < radius;
    }

    virtual bool OnUserUpdate(float fElapsedTime)
    {
        switch (gameState)
        {
        case GameState::StartScreen:
            DrawStartScreen();
            if (m_keys[VK_RETURN].bPressed)
                gameState = GameState::Playing;
            break;

        case GameState::Playing:
            GameLogic(fElapsedTime);
            break;

        case GameState::GameOver:
            DrawGameOverScreen();
            if (m_keys[VK_RETURN].bPressed)
            {
                ResetGame();
                gameState = GameState::StartScreen;
            }
            break;
        }

        return true;
    }

    void GameLogic(float fElapsedTime) // Main game logic
    {
        if (bDead)
        {
            nLives--;
            if (nLives <= 0)
            {
                gameState = GameState::GameOver;
                return;
            }
            else
            {
                player.x = ScreenWidth() / 2.0f;
                player.y = ScreenHeight() / 2.0f;
                player.dx = 0.0f;
                player.dy = 0.0f;
                player.angle = 0.0f;

                vecBullets.clear();

                // Reset asteroids
                vecAsteroids.clear();
                int numAsteroids = 5 + nLevel * 2; // Increase the number of asteroids with each level
                int asteroidSpeed = 8 + nLevel;     // Increase the speed of asteroids with each level
                int asteroidSize = 16;              // Initial size of asteroids
                for (int i = 0; i < numAsteroids; i++)
                {
                    float asteroidX = rand() % ScreenWidth();
                    float asteroidY = rand() % ScreenHeight();
                    while (IsPointInsideCircle(player.x, player.y, 50, asteroidX, asteroidY))
                    {
                        asteroidX = rand() % ScreenWidth();
                        asteroidY = rand() % ScreenHeight();
                    }
                    vecAsteroids.push_back({ asteroidSize, asteroidX, asteroidY, (float)asteroidSpeed, -(float)asteroidSpeed, 0.0f });
                }

                bDead = false;
                return;
            }
        }

        Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, 0);

        if (m_keys[VK_LEFT].bHeld)
            player.angle -= 5.0f * fElapsedTime;
        if (m_keys[VK_RIGHT].bHeld)
            player.angle += 5.0f * fElapsedTime;

        if (m_keys[VK_UP].bHeld)
        {
            player.dx += sin(player.angle) * 20.0f * fElapsedTime;
            player.dy += -cos(player.angle) * 20.0f * fElapsedTime;
        }

        player.x += player.dx * fElapsedTime;
        player.y += player.dy * fElapsedTime;

        WrapCoordinates(player.x, player.y, player.x, player.y);

        for (auto& a : vecAsteroids)
            if (IsPointInsideCircle(a.x, a.y, a.nSize, player.x, player.y))
            {
                bDead = true;
            }

        if (m_keys[VK_SPACE].bReleased)
            vecBullets.push_back({ 0, player.x, player.y, 50.0f * sinf(player.angle), -50.0f * cosf(player.angle), 100.0f });

        for (auto& a : vecAsteroids)
        {
            a.x += a.dx * fElapsedTime;
            a.y += a.dy * fElapsedTime;
            a.angle += 0.5f * fElapsedTime;
            WrapCoordinates(a.x, a.y, a.x, a.y);
            DrawWireFrameModel(vecModelAsteroid, a.x, a.y, a.angle, (float)a.nSize, FG_YELLOW);
        }

        vector<sSpaceObject> newAsteroids;

        for (auto& b : vecBullets)
        {
            b.x += b.dx * fElapsedTime;
            b.y += b.dy * fElapsedTime;
            WrapCoordinates(b.x, b.y, b.x, b.y);
            b.angle -= 1.0f * fElapsedTime;

            for (auto& a : vecAsteroids)
            {
                if (IsPointInsideCircle(a.x, a.y, a.nSize, b.x, b.y))
                {
                    b.x = -100;
                    if (a.nSize > 4)
                    {
                        float angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
                        float angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
                        newAsteroids.push_back({ (int)a.nSize >> 1, a.x, a.y, 10.0f * sinf(angle1), 10.0f * cosf(angle1), 0.0f });
                        newAsteroids.push_back({ (int)a.nSize >> 1, a.x, a.y, 10.0f * sinf(angle2), 10.0f * cosf(angle2), 0.0f });
                    }
                    a.x = -100;
                    nAsteroidsLeft--; // Update count of asteroids left
                    nScore += 100;
                }
            }
        }

        for (auto a : newAsteroids)
            vecAsteroids.push_back(a);

        if (vecAsteroids.size() > 0)
        {
            auto i = remove_if(vecAsteroids.begin(), vecAsteroids.end(), [&](sSpaceObject o) { return (o.x < 0); });
            if (i != vecAsteroids.end())
                vecAsteroids.erase(i);
        }

        if (vecAsteroids.empty())
        {
            nScore += 1000 + (nLevel - 1) * 1000; // Add 1000 points for clearing asteroids, plus an additional 1000 points for each level after the first
            vecAsteroids.clear();
            vecBullets.clear();
            ResetGame(); // Reset the game for the next level
            nLevel++; // Increase the level when all asteroids are cleared

        }

        if (vecBullets.size() > 0)
        {
            auto i = remove_if(vecBullets.begin(), vecBullets.end(), [&](sSpaceObject o) { return (o.x < 1 || o.y < 1 || o.x >= ScreenWidth() - 1 || o.y >= ScreenHeight() - 1); });
            if (i != vecBullets.end())
                vecBullets.erase(i);
        }

        for (auto b : vecBullets)
            Draw(b.x, b.y);

        DrawWireFrameModel(vecModelShip, player.x, player.y, player.angle);

        DrawString(2, 2, L"SCORE: " + to_wstring(nScore));
        DrawString(2, 3, L"LIVES: " + to_wstring(nLives));
        DrawString(2, 4, L"LEVEL: " + to_wstring(nLevel)); // Display the current level
    }

    void DrawWireFrameModel(const vector<pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, short col = FG_WHITE) // Draw a wireframe model at a given position with rotation and scaling
    {
        vector<pair<float, float>> vecTransformedCoordinates;
        int verts = vecModelCoordinates.size();
        vecTransformedCoordinates.resize(verts);

        for (int i = 0; i < verts; i++)
        {
            vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
            vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
        }

        for (int i = 0; i < verts; i++)
        {
            vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
            vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
        }

        for (int i = 0; i < verts; i++)
        {
            vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
            vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
        }

        for (int i = 0; i < verts + 1; i++)
        {
            int j = (i + 1);
            DrawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second,
                vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, PIXEL_SOLID, col);
        }
    }

    void DrawStartScreen() // Draw the start screen
    {
        // Display start screen
        Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLUE);

        // Calculate center position for text
        int titleX = ScreenWidth() / 2 - 27;
        int titleY = ScreenHeight() / 4 - 1;
        int instructionX = ScreenWidth() / 2 - 25;
        int instructionY = ScreenHeight() / 3 + 1;

        // Display title and instruction
        DrawString(titleX, titleY, L"Attack of the flying space objects", FG_WHITE);
        DrawString(instructionX, instructionY, L"Press number key for difficulty then Enter to Start", FG_WHITE);

        // Calculate center position for difficulty selection
        int difficultyX = ScreenWidth() / 2 - 22;
        int difficultyY = ScreenHeight() / 2 - 3; // Move the block upwards by decreasing this value

        // Display difficulty selection
        DrawString(difficultyX, difficultyY, L"Select Difficulty:", FG_WHITE);
        DrawString(difficultyX + 2, difficultyY + 2, L"1. Easy", (m_keys[VK_NUMPAD1].bPressed || m_keys[L'1'].bPressed ? FG_YELLOW : FG_WHITE));
        DrawString(difficultyX + 2, difficultyY + 3, L"2. Medium", (m_keys[VK_NUMPAD2].bPressed || m_keys[L'2'].bPressed ? FG_YELLOW : FG_WHITE));
        DrawString(difficultyX + 2, difficultyY + 4, L"3. Hard", (m_keys[VK_NUMPAD3].bPressed || m_keys[L'3'].bPressed ? FG_YELLOW : FG_WHITE));

        // Display selected difficulty
        if (m_keys[VK_NUMPAD1].bPressed || m_keys[L'1'].bPressed || m_keys[VK_NUMPAD2].bPressed || m_keys[L'2'].bPressed || m_keys[VK_NUMPAD3].bPressed || m_keys[L'3'].bPressed)
        {
            DrawString(difficultyX + 2, difficultyY + 6, L"Difficulty Selected: ", FG_WHITE);
            if (m_keys[VK_NUMPAD1].bPressed || m_keys[L'1'].bPressed) DrawString(difficultyX + 24, difficultyY + 6, L"1. Easy", FG_YELLOW);
            else if (m_keys[VK_NUMPAD2].bPressed || m_keys[L'2'].bPressed) DrawString(difficultyX + 24, difficultyY + 6, L"2. Medium", FG_YELLOW);
            else if (m_keys[VK_NUMPAD3].bPressed || m_keys[L'3'].bPressed) DrawString(difficultyX + 24, difficultyY + 6, L"3. Hard", FG_YELLOW);
        }
    }

    void DrawGameOverScreen() // Draw the game over screen
    {
        // Display game over screen
        Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_RED);
        DrawString(ScreenWidth() / 2 - 4, ScreenHeight() / 2, L"GAME OVER", FG_WHITE);
        DrawString(ScreenWidth() / 2 - 10, ScreenHeight() / 2 + 2, L"Press Enter to Restart", FG_WHITE);
    }
};

int main()
{
    OneLoneCoder_Asteroids game;
    game.ConstructConsole(160, 100, 8, 8);

    // Set console window size to match game resolution
    HWND console = GetConsoleWindow();
    RECT rect;
    GetWindowRect(console, &rect);
    MoveWindow(console, rect.left, rect.top, 160 * 8, 100 * 8, TRUE);

    // Clear the console window before starting the game
    system("cls");

    game.Start(); // Start the game
    return 0;
}
