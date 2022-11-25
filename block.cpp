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
        bool collision_with_player =
            (player_b_y >= ball_u_y) &&
            (player_u_y <= ball_b_y) &&
            (player_r_x >= ball_l_x) &&
            (player_l_x <= ball_r_x);

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

    // brick collision information
    int tempBrickSide = 0; // 0 = left, 1 = top, 2 = right, 3 = bottom
    int tempX = 0;
    int tempY = 0;
    bool collision_with_brick{false};
    
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

        // update collisions
            // player
            collision_with_player =
                (player_b_y >= ball_u_y) &&
                (player_u_y <= ball_b_y) &&
                (player_r_x >= ball_l_x) &&
                (player_l_x <= ball_r_x);
            // bricks
            for (int i = 0; i < 210; i++) {
                if (!collision_with_brick && bricks[i].power != 0 && bricks[i].power != 5) {
                    collision_with_brick =
                        (bricks[i].b_y >= ball_u_y) &&
                        (bricks[i].u_y <= ball_b_y) &&
                        (bricks[i].r_x >= ball_l_x) &&
                        (bricks[i].l_x <= ball_r_x);
                    
                    if (collision_with_brick) {
                        bricks[i].power--;
                        if (ballY >= bricks[i].u_y && ballY <= bricks[i].b_y) {
                            if (ballX < bricks[i].l_x + bricks[i].width/2) {
                                tempBrickSide = 0;
                                tempX = bricks[i].l_x;
                            } else {
                                tempBrickSide = 2;
                                tempX = bricks[i].r_x;
                            }
                        } else {
                            if (ballY < bricks[i].u_y + bricks[i].height/2) {
                                tempBrickSide = 1;
                                tempY = bricks[i].u_y;
                            } else {
                                tempBrickSide = 3;
                                tempY = bricks[i].b_y;
                            }
                        }
                    }
                }
            }

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
                if (playerX < 0) { playerX = 0; }
            } else if (IsKeyDown(KEY_RIGHT)) {
                playerX += playerVel;
                if (playerX > WIN_WIDTH - playerWidth) { playerX = WIN_WIDTH - playerWidth; }
            }

            // ball movement
            if (!ballInPlay) { ballX = playerCenter; } // ball stays with paddle when not in play
            if (!ballInPlay && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE))) { ballInPlay = true; } // ball in play

                // ball collision
                if (collision_with_player) { // with player
                    collision_with_player = false;
                    ballY = playerY - ballRadius;
                    if (ball_l_x > playerCenter) {
                        ballXDir = 1;
                    } else {
                        ballXDir = -1;
                    }
                    ballYDir = -ballYDir;
                } else if (collision_with_brick) { // with brick
                    if (tempBrickSide == 0 || tempBrickSide == 2) {
                        ballXDir = -ballXDir;
                        if (tempBrickSide == 0) { ballX = tempX - ballRadius; 
                        } else { ballX = tempX + ballRadius; }
                    } else {
                        ballYDir = -ballYDir;
                        if (tempBrickSide == 1) { ballY = tempY - ballRadius;
                        } else { ballY = tempY + ballRadius; }
                    }
                    collision_with_brick = false;
                }

            if (ballInPlay) {
                ballX += ballVel * ballXDir;
                ballY += ballVel * ballYDir;
                if (ballX < ballRadius) { // bounce off left wall
                    ballX = ballRadius;
                    ballXDir = -ballXDir;
                } else if (ballX > WIN_WIDTH - ballRadius) { // bounce off right wall
                    ballX = WIN_WIDTH - ballRadius;
                    ballXDir = -ballXDir;
                } else if (ballY < 50 + ballRadius) { // bounce off ceilling
                    ballY = 50 + ballRadius;
                    ballYDir = -ballYDir;
                } else if (ballY > WIN_HEIGHT + ballRadius*2) { // reset ball if it goes out of play
                    ballX = playerCenter;
                    ballY = playerY - ballRadius - 3;
                    ballInPlay = false;
                    ballXDir = 1;
                    ballYDir = -1;
                }
            }

            runningTime = 0.0;
        }
        EndDrawing();
    }
    CloseWindow();
}
