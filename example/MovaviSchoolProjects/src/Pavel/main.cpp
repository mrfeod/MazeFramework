#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Maze.h"
#include "time.h"
using namespace std;
int MAZE[100][100];
int WIDTH = 10, HEIGHT = 4;
int m,n;
int Exit_y,Exit_x;
int x,y;
int new_x,new_y;
int I[500],J[500];
int CANDIDATS = 0;
int STARS = 5;
int Neighbours(int x, int y)
{
    int neighbours = 0;
    if(MAZE[y-1][x] == Maze::Floor) ++neighbours;
    if(MAZE[y+1][x] == Maze::Floor) ++neighbours;
    if(MAZE[y][x-1] == Maze::Floor) ++neighbours;
    if(MAZE[y][x+1] == Maze::Floor) ++neighbours;
    return neighbours;
}
bool isCorner(int x, int y)
{
    bool top =MAZE[y-1][x] == Maze::Floor;
    bool bot =MAZE[y+1][x] ==Maze::Floor;
    bool left =MAZE[y][x-1] == Maze::Floor;
    bool right =MAZE[y][x+1] == Maze::Floor;
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
void GaneratStars()
{
    for(int i = 1; i < HEIGHT - 1; ++i)
    {
        for(int j = 1; j < WIDTH - 1; ++j)
        {
            bool corner = isCorner(j, i);
            bool deadend = isDeadend(j, i);
            if( MAZE[i][j] == Maze::Floor && ( corner || deadend ))
            {
                I[CANDIDATS]=i;
                J[CANDIDATS]=j;
                ++CANDIDATS;
            }
        }
    }

    for(int i =0; i < CANDIDATS; ++i)
    {
        cout << I[i] << " " << J[i] << endl;
    }

    int needStars = STARS;
    while(needStars > 0)
    {
        int num = rand()%CANDIDATS;

        if( MAZE[I[num]][J[num]] != Maze::Player)
        {
            MAZE[I[num]][J[num]] = Maze::Player;
            needStars--;
        }
    }
}

void Read(const char* filename)
{
    FILE* file;
    file=fopen("maze.txt","r");
    fscanf(file, "%d %d", &WIDTH, &HEIGHT);

    for(int i=0; i < WIDTH; ++i)
    {
        for(int j=0; j < HEIGHT; ++j)
        {
            fscanf(file, "%d ", &MAZE[i][j]);
            if(MAZE[i][j]==Maze::Player)
            {
                x=j;
                y=i;
            }
            if(MAZE[i][j]==Maze::Exit)
            {
                Exit_x = j;
                Exit_y = i;
            }

        }
    }
    for(int i=0; i < WIDTH; ++i)
    {
        for(int j=0; j < HEIGHT; ++j)
        {
            printf( "%d ", MAZE[i][j]);
        }
        printf( "\n");
    }
}

bool Loop(int key)
{
    new_x = x;
    new_y = y;
    switch(key)
    {
    case Maze::Key_Down:
        new_y = y + 1;
        break;
    case Maze::Key_Up:
        new_y = y - 1;
        break;
    case Maze::Key_Left:
        new_x = x - 1;
        break;
    case Maze::Key_Right:
        new_x = x + 1;
        break;
    }

    if(new_y > WIDTH - 1)  new_y = WIDTH - 1;
    if(new_y < 0)          new_y = 0;
    if(new_x > HEIGHT - 1) new_x = HEIGHT - 1;
    if(new_x < 0 )         new_x = 0;

    if(MAZE[new_y][new_x] != Maze::Wall && MAZE[new_y][new_x] != Maze::Exit)
    {
        if(MAZE[new_y][new_x] == Maze::Player && (x != new_x || y != new_y))
            --STARS;

        MAZE[y][x] = Maze::Floor;
        x = new_x;
        y = new_y;
        MAZE[y][x] = Maze::Player;
    }

    if(STARS <= 0)
    {
        MAZE[Exit_y][Exit_x] = Maze::Floor;
    }
    Maze::Update((int*)(MAZE), WIDTH, HEIGHT);

    if(Exit_x == x && Exit_y == y)
        return false;
    return true;
}
int main(int argc, char** argv)
{
    srand(time(0));
    Read("maze.txt");
    GaneratStars();
    Maze::Initialize(argc, argv);
    Maze::CreateWindow(800, 600);
    Maze::Start(Loop);
    Maze::DeInitialize();
    return 0;
}
