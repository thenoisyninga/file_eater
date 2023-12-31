#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#define GAMESCREEN_WIDTH 50
#define GAMESCREEN_HEIGHT 10

struct SnakeGame
{
    int vel_x;
    int vel_y;
    int food_x;
    int food_y;
    int score;
    int running;

    int length;
    int top;
    int snakeBody[GAMESCREEN_WIDTH * GAMESCREEN_HEIGHT][2];
};

int temp;
struct SnakeGame snakeGame;

char inputCh;
char snakeChar = '0';
char backgroundChar = ' ';
char borderChar = 'X';
char foodChar = 'a';

void putFood()
{
    snakeGame.food_x = (rand() % (GAMESCREEN_WIDTH - 2)) + 1;
    snakeGame.food_y = (rand() % (GAMESCREEN_HEIGHT - 2)) + 1;
}

void initialize()
{
    // Initialize all values of the array
    for (int i = 0; i < (GAMESCREEN_HEIGHT * GAMESCREEN_WIDTH); i++)
    {
        snakeGame.snakeBody[i][0] = -1;
        snakeGame.snakeBody[i][1] = -1;
    }

    snakeGame.score = 0;

    snakeGame.snakeBody[0][0] = (int)GAMESCREEN_WIDTH / 2;
    snakeGame.snakeBody[0][1] = (int)GAMESCREEN_HEIGHT / 2;

    snakeGame.snakeBody[0][0] = (int)(GAMESCREEN_WIDTH / 2) - 1;
    snakeGame.snakeBody[0][1] = (int)GAMESCREEN_HEIGHT / 2;

    snakeGame.snakeBody[0][0] = (int)(GAMESCREEN_WIDTH / 2) - 2;
    snakeGame.snakeBody[0][1] = (int)GAMESCREEN_HEIGHT / 2;

    snakeGame.length = 3;

    putFood();

    snakeGame.vel_x = 1;
    snakeGame.vel_y = 0;
}

int checkGameOver()
{
    if ((snakeGame.snakeBody[0][0] + snakeGame.vel_x) < (GAMESCREEN_WIDTH - 1) &&
        (snakeGame.snakeBody[0][0] + snakeGame.vel_x) > 0 &&
        (snakeGame.snakeBody[0][1] + snakeGame.vel_y) < (GAMESCREEN_HEIGHT - 1) &&
        (snakeGame.snakeBody[0][1] + snakeGame.vel_y) > 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void gameOver()
{
    snakeGame.running = 0;
}

int snakeIsHere(int x, int y)
{
    for (int i = 0; i < (GAMESCREEN_WIDTH * GAMESCREEN_HEIGHT); i++)
    {
        // check x and y
        if (x == snakeGame.snakeBody[i][0] && y == snakeGame.snakeBody[i][1])
        {
            return 1;
        }
    }
    return 0;
}

int hasEatenFood()
{
    for (int i = 0; i < (GAMESCREEN_HEIGHT * GAMESCREEN_WIDTH); i++)
    {
        if (snakeGame.food_x == (snakeGame.snakeBody[i][0] + snakeGame.vel_y) && snakeGame.food_y == (snakeGame.snakeBody[i][1] + snakeGame.vel_y))
        {
            return 1;
        }
    }
    return 0;
}

void update()
{
    if (checkGameOver())
    {
        gameOver();
    }

    if (hasEatenFood())
    {
        snakeGame.length++;
        snakeGame.score++;
        putFood();
    }

    // Shift all places to right
    for (int i = (GAMESCREEN_HEIGHT * GAMESCREEN_WIDTH) - 1; i >= 0; i--)
    {
        snakeGame.snakeBody[i + 1][0] = snakeGame.snakeBody[i][0];
        snakeGame.snakeBody[i + 1][1] = snakeGame.snakeBody[i][1];
    }

    snakeGame.snakeBody[0][0] += snakeGame.vel_x;
    snakeGame.snakeBody[0][1] += snakeGame.vel_y;

    snakeGame.top++;

    // Do a cleanup
    while (snakeGame.top + 1 > snakeGame.length)
    {
        snakeGame.snakeBody[snakeGame.top][0] = -1;
        snakeGame.snakeBody[snakeGame.top][1] = -1;
        snakeGame.top--;
    }
}

void render()
{
    for (int j = 0; j < GAMESCREEN_HEIGHT; j++)
    {
        for (int i = 0; i < GAMESCREEN_WIDTH; i++)
        {
            if (i == 0 || i == GAMESCREEN_WIDTH - 1 || j == 0 || j == GAMESCREEN_HEIGHT - 1)
            {
                printf("%c", borderChar);
            }
            else if (snakeIsHere(i, j))
            {
                printf("%c", snakeChar);
            }
            else if (i == snakeGame.food_x && j == snakeGame.food_y)
            {
                printf("%c", foodChar);
            }
            else
            {
                printf("%c", backgroundChar);
            }
        }
        printf("\n");
    }
    printf("SCORE: %d", snakeGame.score);
}

void updateVelocity()
{
    if (_kbhit())
    {
        inputCh = getch();
        if ((inputCh == 'w' || inputCh == 'W') && snakeGame.vel_y != 1)
        {
            snakeGame.vel_x = 0;
            snakeGame.vel_y = -1;
        }
        else if ((inputCh == 'a' || inputCh == 'A') && snakeGame.vel_x != 1)
        {
            snakeGame.vel_x = -1;
            snakeGame.vel_y = 0;
        }
        else if ((inputCh == 's' || inputCh == 'S') && snakeGame.vel_y != -1)
        {
            snakeGame.vel_x = 0;
            snakeGame.vel_y = 1;
        }
        else if ((inputCh == 'd' || inputCh == 'D') && snakeGame.vel_x != -1)
        {
            snakeGame.vel_x = 1;
            snakeGame.vel_y = 0;
        }
    }
}

void resetCursor()
{
    // Create a COORD structure and fill in its members.
    // This specifies the new position of the cursor that we will set.
    COORD coord;
    coord.X = 0;
    coord.Y = 0;

    // Obtain a handle to the console screen buffer.
    // (You're just using the standard console, so you can use STD_OUTPUT_HANDLE
    // in conjunction with the GetStdHandle() to retrieve the handle.)
    // Note that because it is a standard handle, we don't need to close it.
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Finally, call the SetConsoleCursorPosition function.
    if (!SetConsoleCursorPosition(hConsole, coord))
    {
        // Uh-oh! The function call failed, so you need to handle the error.
        // You can call GetLastError() to get a more specific error code.
        // ...
    }
}

void hideCursor()
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = 0; // set the cursor visibility to false
    SetConsoleCursorInfo(out, &cursorInfo);
}

void clearScreen()
{
    resetCursor();
    for (int i = 0; i < GAMESCREEN_HEIGHT + 3; i++)
    {
        for (int j = 0; j < GAMESCREEN_WIDTH + 3; j++)
        {
            printf(" ");
        }
        printf("\n");
    }
    resetCursor();
}

void main()
{
    snakeGame.running = 1;

    while (1)
    {
        // SnakeGame Starts
        hideCursor();
        initialize();
        clearScreen();

        // SnakeGame Runs
        while (snakeGame.running)
        {
            update();
            render();
            Sleep(250);
            updateVelocity();
            fflush(stdin);
            resetCursor(0, 0);
        }

        // SnakeGame Over
        clearScreen();
        printf("\nGAME OVER.");
        printf("\nPress any key to try again.");
        getch();
        snakeGame.running = 1;
        fflush(stdin);
    }
}