#include "raylib.h"
#include <string>

struct brick
{
    // position, size & power
    int power;
    int width;  // 117
    int height; // 20
    int x;
    int y;
    // collision
    int l_x;
    int r_x;
    int u_y;
    int b_y;
};

struct level
{
    int row[15][14];
};

int main(int argc, char const *argv[])
{
    // initialize window
    const int WIN_WIDTH = 1280;
    const int WIN_HEIGHT = 800;
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Block Breaker");

    // universal timer
    const float updateTime = 1.0/60.0;
    float runningTime = 0.0;

    // player
        // position, size & movement
        int playerWidth = 75;
        int playerHeight = 6;
        int playerX = WIN_WIDTH/2 - playerWidth;
        int playerY = WIN_HEIGHT - playerHeight - 10;
        int playerVel = 10;
        // collision
        int playerCenter = playerX + playerWidth/2;
        int player_l_x = playerX;
        int player_r_x = playerX + playerWidth;
        int player_u_y = playerY;
        int player_b_y = playerY + playerHeight;
        // score
        int playerScore = 0;
        char playerScoreText[5];
        std::sprintf(playerScoreText, "%d", playerScore);

    // ball
        // position, size & movement
        int ballRadius = 5;
        int ballX = playerCenter;
        int ballY = playerY - ballRadius - 3;
        bool ballInPlay{false};
        int ballVel = 7;
        int ballXDir = 1;
        int ballYDir = -1;
        // collision
        int ball_l_x = ballX - ballRadius;
        int ball_r_x = ballX + ballRadius;
        int ball_u_y = ballY - ballRadius;
        int ball_b_y = ballY + ballRadius;

    // levels
    int currentLevel = 1;
    level levels[1];
        levels[0] = level{{{5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
                           {1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1},
                           {1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1},
                           {1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1},
                           {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                           {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
                           {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
                           {},
                           {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
                           {},
                           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                           {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
                           {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
                           {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}}};

    // build bricks
    int brickWidth = 90;
    int brickHeight = 20;
    int brickCnt = 0;
    brick bricks[210];
    for (int j = 0; j < 15; j++)
    {
        for (int k = 0; k < 14; k++)
        {
            bricks[brickCnt].power = levels[currentLevel - 1].row[j][k];
            bricks[brickCnt].width = brickWidth;
            bricks[brickCnt].height = brickHeight;
            bricks[brickCnt].x = 3 + k + (bricks[brickCnt].width * k);
            bricks[brickCnt].y = 50 + j + (bricks[brickCnt].height * j);
            bricks[brickCnt].l_x = bricks[brickCnt].x;
            bricks[brickCnt].r_x = bricks[brickCnt].x + bricks[brickCnt].width;
            bricks[brickCnt].u_y = bricks[brickCnt].y;
            bricks[brickCnt].b_y = bricks[brickCnt].y + bricks[brickCnt].height;
            brickCnt++;
        }
    }

    /******************************
     *        Power  Chart        *
     * ---------------------------*
     * 0 = No Brick               *
     * 1 = RED      - 1 hit       *
     * 2 = DARKBLUE - 2 hits      *
     * 3 = GREEN    - 3 hits      *
     * 4 = PURPLE   - 4 hits      *
     * 5 = GRAY     - unbreakable *
     ******************************/
    Color brickColors[5]{RED, DARKBLUE, GREEN, PURPLE, GRAY};
    
    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        // update edges
            // player
            player_l_x = playerX;
            player_r_x = playerX + playerWidth;
            playerCenter = playerX + playerWidth/2;
            // ball
            ball_l_x = ballX - ballRadius;
            ball_r_x = ballX + ballRadius;
            ball_u_y = ballY - ballRadius;
            ball_b_y = ballY + ballRadius;

        BeginDrawing();

        const float dT{ GetFrameTime() };
        runningTime += dT;

        ClearBackground(WHITE);

        // draw bricks
        for (int i = 0; i < 210; i++)
        {
            if (bricks[i].power > 0)
            {
                DrawRectangle(bricks[i].x, bricks[i].y, bricks[i].width, bricks[i].height, brickColors[bricks[i].power - 1]);
            }
        }

        // draw player and ball
        DrawRectangle(playerX, playerY, playerWidth, playerHeight, BLUE);
        DrawCircle(ballX, ballY, ballRadius, BLACK);

        if (runningTime >= updateTime)
        {
            // player moves left or right
            if (IsKeyDown(KEY_LEFT)) {
                playerX -= playerVel;
            } else if (IsKeyDown(KEY_RIGHT)) {
                playerX += playerVel;
            }

            // ball movement
            if (!ballInPlay) { ballX = playerCenter; }
            if (!ballInPlay && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE))) { ballInPlay = true; }
            if (ballInPlay) {
                ballX += ballVel * ballXDir;
                ballY += ballVel * ballYDir;
                if (ballX < 0 + ballRadius) {
                    ballX = 0 + ballRadius;
                    ballXDir = -ballXDir;
                } else if (ballX > WIN_WIDTH - ballRadius) {
                    ballX = WIN_WIDTH - ballRadius;
                    ballXDir = -ballXDir;
                } else if (ballY < 50 + ballRadius) {
                    ballY = 50 + ballRadius;
                    ballYDir = -ballYDir;
                }
            }

            runningTime = 0.0;
        }
        EndDrawing();
    }
    CloseWindow();
}
