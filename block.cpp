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
    int row[15][10];
//    int rowOne[10];
//    int rowTwo[10];
//    int rowThree[10];
//    int rowFour[10];
//    int rowFive[10];
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
        // position & size
        int playerWidth = 75;
        int playerHeight = 6;
        int playerX = WIN_WIDTH/2 - playerWidth;
        int playerY = WIN_HEIGHT - playerHeight - 10;
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
        // position & size
        int ballRadius = 5;
        int ballX = playerCenter - ballRadius;
        int ballY = playerY - ballRadius - 3;
        // collision
        int ball_l_x = ballX - ballRadius;
        int ball_r_x = ballX + ballRadius;
        int ball_u_y = ballY - ballRadius;
        int ball_b_y = ballY + ballRadius;

    // levels
    int currentLevel = 1;
    level levels[1];
        levels[0] = level{{{},
                           {},
                           {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                           {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                           {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}};

    // build bricks
    int brickWidth = 117;
    int brickHeight = 20;
    int brickCnt = 0;
    brick bricks[150];
    for (int j = 0; j < 15; j++)
    {
        for (int k = 0; k < 10; k++)
        {
            bricks[brickCnt].power = levels[currentLevel - 1].row[j][k];
            bricks[brickCnt].width = brickWidth;
            bricks[brickCnt].height = brickHeight;
            bricks[brickCnt].x = 10 + (10 * k) + (bricks[brickCnt].width * k);
            bricks[brickCnt].y = 30 + (10 * j) + (bricks[brickCnt].height * j);
            bricks[brickCnt].l_x = bricks[brickCnt].x;
            bricks[brickCnt].r_x = bricks[brickCnt].x + bricks[brickCnt].width;
            bricks[brickCnt].u_y = bricks[brickCnt].y;
            bricks[brickCnt].b_y = bricks[brickCnt].y + bricks[brickCnt].height;
            brickCnt++;
        }
    }
    
    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        BeginDrawing();

        const float dT{ GetFrameTime() };
        runningTime += dT;

        ClearBackground(WHITE);

        // draw bricks
        for (int i = 0; i < 150; i++)
        {
            if (bricks[i].power > 0)
            {
                DrawRectangle(bricks[i].x, bricks[i].y, bricks[i].width, bricks[i].height, RED);
            }
        }

        if (runningTime >= updateTime)
        {
            // TODO: Game Code

            runningTime = 0.0;
        }
        EndDrawing();
    }
    CloseWindow();
}
