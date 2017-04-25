#include <iostream>
#include <stdio.h>

#include "Maze.h"

using namespace std;

int MAZE[100][100];
int W = 0;
int H = 0;
int X = 0;
int Y = 0;
int EXIT_X = 0;
int EXIT_Y = 0;

void Read(const char* filename)
{
    FILE* f = fopen(filename, "r");
    if(!f)
        return;

    fscanf(f, "%d %d", &W, &H);
    for(int i = 0; i < H; ++i)
    {
         for(int j = 0; j < W; ++j)
         {
             fscanf(f, "%d", &(MAZE[i][j]));
             if(MAZE[i][j] == Maze::Player)
             {
                 Y = i;
                 X = j;
             }
             else if(MAZE[i][j] == Maze::Exit)
             {
                 EXIT_Y = i;
                 EXIT_X = j;
             }
         }
    }

    fclose(f);
}

bool Loop(int key)
{
    int newX = X, newY = Y;
    switch (key)
    {
        case Maze::Key_Up: newY > 0 ? --newY : 0; break;
        case Maze::Key_Down: ++newY; break;
        case Maze::Key_Left: newX > 0   ? --newX : 0; break;
        case Maze::Key_Right: ++newX; break;
    }

    if (MAZE[newY][newX] != Maze::Wall)
    {
        MAZE[Y][X] = Maze::Floor;
        X = newX;
        Y = newY;
        MAZE[Y][X] = Maze::Player;
    }

    Maze::Update((int*)(MAZE), W, H);

    if(X == EXIT_X && Y == EXIT_Y)
        return false;

    return true;
}

int main(int argc, char** argv)
{
    Read("maze.txt");
    Maze::Initialize(argc, argv);
    Maze::CreateWindow(800, 600);
    Maze::Start(Loop);
    Maze::DeInitialize();
    return 0;
}
