#include "MazeFramework/include/Maze.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;
int MAZE[100][100];
int WIDTH = 10, HEIGHT = 4;
int player_x = 0, player_y = 0;
int C = 0;
int Stars = 4;
int I[500], J[500];
int new_x, new_y;
int Exit_x, Exit_y;
int Neighbours(int x, int y)
{
    int neighbours = 0;
    if (MAZE[y-1][x]==Maze::Floor) ++neighbours;
    if (MAZE[y][x-1]==Maze::Floor) ++neighbours;
    if (MAZE[y+1][x]==Maze::Floor) ++neighbours;
    if (MAZE[y][x+1]==Maze::Floor) ++neighbours;
    return neighbours;
}

bool isCorner(int x, int y)
{
    bool top = MAZE[y-1][x] == Maze::Floor;
    bool bot = MAZE[y+1][x] == Maze::Floor;
    bool left = MAZE[y][x-1] == Maze::Floor;
    bool right = MAZE[y][x+1] == Maze::Floor;
    if(Neighbours(x, y) == 2)
    {
        return (top && left) ||
               (top && right) ||
               (bot && left) ||
               (bot && right);
    }
    return false;
}
bool isDeadend(int x, int y)
{
    return Neighbours(x, y) == 1;
}
void GenerateStar()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if(MAZE[i][j] == Maze::Floor &&( isCorner(j,i) || isDeadend(j, i)))
            {
                I[C] = i;
                J[C] = j;
                C++;
            }
        }

    }
    int need = Stars;
    while (need > 0)
    {
        int c = rand()%C;
        if(I[c] != -1)
        {
            MAZE[I[c]][J[c]]= Maze::Player;
            need--;
            I[c] = -1;
        }
    }
}
void Read()
{
    FILE *pfile;
    int i, j;
    pfile = fopen("maze.txt","r");
    if(!pfile)
        return;
    fscanf (pfile, "%d %d", &WIDTH, &HEIGHT);
    for (i = 0; i < HEIGHT; i++)
    {
        for(j = 0; j < WIDTH; j++)
        {
            fscanf(pfile, "%d", &MAZE[i][j]);
            if (MAZE[i][j] == Maze::Player)
            {
                player_x = j;
                player_y = i;
            }
            if (MAZE[i][j] == Maze::Exit)
            {
                Exit_x = j;
                Exit_y = i;
            }
        }

    }
    fclose(pfile);
}

//Функция в которую приходит код клавиши key
//Результ работы - булево значение, указывающее продолжать работу или нет
bool Loop(int key)
{
    new_x = player_x;
    new_y = player_y;
    switch(key)
    {
    case Maze::Key_W:
        new_y--;
        break;
    case Maze::Key_S:
        new_y++;
        break;
    case Maze::Key_D:
        new_x++;
        break;
    case Maze::Key_A:
        new_x--;
        break;

    }
    if (new_y < 0)
        new_y = 0;
    if (new_y > HEIGHT - 1)
        new_y = HEIGHT -1;
    if (new_x < 0)
        new_x = 0;
    if (new_x > WIDTH -1)
        new_x = WIDTH -1;

    if (MAZE[new_y][new_x] != Maze::Wall && MAZE[new_y][new_x] != Maze::Exit)
    {
        if (MAZE[new_y][new_x] == Maze::Player && (new_x != player_x || new_y != player_y))
            --Stars;
        MAZE[player_y][player_x]= Maze::Floor;
        player_x = new_x;
        player_y = new_y;
        MAZE[player_y][player_x]= Maze::Player;
    }
    if (Stars <= 0)
    {
        MAZE[Exit_y][Exit_x] = Maze::Player;
    }
    Maze::Update((int*)(MAZE), WIDTH, HEIGHT);

    if (player_x == Exit_x && player_y == Exit_y)
        return false;
    return true;
}

int main(int argc, char** argv)
{
    Read();
    //Предварительно нужно считать лабиринт в массив MAZE
    Maze::Initialize(argc, argv);
    Maze::CreateWindow(599, 599);
    GenerateStar();
    Maze::Start(Loop);
    Maze::DeInitialize();
    return 0;
}
