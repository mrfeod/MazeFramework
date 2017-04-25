#include <iostream>
#include "Maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int MAZE[100][100];
int DEBUG[100][100];
int CANDX[50];
int CANDY[50];
int STARX[5];
int STARY[5];
int candmax=5;
int WIDTH = 10, HEIGHT = 4;
int x,y;
int i,j;
int rndom;
int starcount = 0;


//Функция в которую приходит код клавиши key
//Результ работы - булево значение, указывающее продолжать работу или нет

void Read()
{
    FILE *pfile;
    pfile = fopen("maze.txt", "r");
    fscanf(pfile, "%d %d", &WIDTH, &HEIGHT);
    for (i=0; i<HEIGHT; i++)
    {
        for (j=0; j<WIDTH; j++)
        {
            fscanf(pfile, "%d", &MAZE[i][j]);
            if (MAZE[i][j]==Maze::Player)
            {
                y=i;
                x=j;

            }
        }
    }
    fclose(pfile);
    for (i=0; i<HEIGHT; i++)
    {
        for (j=0; j<WIDTH; j++)
        {
            printf("%d ", MAZE[i][j]);
            DEBUG[i][j] = MAZE[i][j];
        }
        printf("\n");
    }
}

int neigh_count (int x, int y)
{
    int neighbours = 0;
    if (MAZE[y-1][x] == Maze::Floor) neighbours++;
    if (MAZE[y+1][x] == Maze::Floor) neighbours++;
    if (MAZE[y][x-1] == Maze::Floor) neighbours++;
    if (MAZE[y][x+1] == Maze::Floor) neighbours++;
    return neighbours;
}

bool Corner(int x, int y)
{
    if (MAZE[x][y] == Maze::Floor)
    {
        bool top = MAZE[y-1][x] == Maze::Floor;
        bool bot = MAZE[y+1][x] == Maze::Floor;
        bool left = MAZE[y][x-1] == Maze::Floor;
        bool right = MAZE[y][x+1] == Maze::Floor;
        if (neigh_count(x, y) == 2)
        {
            return (top && right) || (top && left) || (bot && right) || (bot && left);
        }
    }
    return false;
}

bool Tupik(int x, int y)
{
    int wallc = 0;
    if (MAZE[y][x]==Maze::Floor)
    {
        if (neigh_count(x, y) == 1)
        {
            if (MAZE[y-1][x] == Maze::Wall) wallc++;
            if (MAZE[y+1][x] == Maze::Wall) wallc++;
            if (MAZE[y][x-1] == Maze::Wall) wallc++;
            if (MAZE[y][x+1] == Maze::Wall) wallc++;
        }
    }
    if (wallc==3) return true;
    return false;
}

void StarGen ()
{
    int starind = 0;
    int cand = 0;
    int tupikc = 0;
    int cornerc = 0;
    int candind=0;
    for (i=1; i<HEIGHT-1; i++)
    {
        for (j=1; j<WIDTH-1; j++)
        {
            if (Tupik(j,i))
            {
                tupikc++;
                CANDX[candind]=j;
                CANDY[candind]=i;
                candind++;
            }
            if (Corner(j,i))
            {
                cornerc++;
                CANDX[candind]=j;
                CANDY[candind]=i;
                candind++;
            }
        }
    }
    printf("%d ", cornerc);
    printf("%d ", tupikc);
    printf("\n");
    for (i=0; i<HEIGHT; i++)
    {
        for (j=0; j<WIDTH; j++)
        {
            MAZE[i][j] = DEBUG[i][j];
        }
    }
    printf("\n");
    printf("\n");
    printf("\n");
    for (i=0; i<HEIGHT; i++)
    {
        for (j=0; j<WIDTH; j++)
        {
            printf ("%d ", DEBUG[i][j]);
        }
        printf("\n");
    }
    while (candmax>0)
    {
        cand=rand()%candind;
        if (CANDX[cand]!=-1)
        {
            MAZE[CANDY[cand]][CANDX[cand]]=Maze::Player;
            STARX[starind] = CANDX[cand];
            STARY[starind] = CANDY[cand];
            CANDX[cand] = -1;
            candmax--;
            starind++;
        }
    }
    for (i=0;i<5;i++)
    {
        cout <<STARX[i]<<endl;
    }
}

bool check (int x, int y,  int newx, int newy)
{
    if (newx != x || newy != y)
    {
        return true;
    }
    return false;
}

void bomb(int x, int y)
{
    MAZE[x-1][y] = Maze::Floor;
    MAZE[x+1][y] = Maze::Floor;
    MAZE[x][y-1] = Maze::Floor;
    MAZE[x][y+1] = Maze::Floor;
    printf("Allahu Akbar! ");
}

bool Loop(int key)
{
    int newx=x;
    int newy=y;
    switch(key)
    {
    case Maze::Key_S:
        newy=y+1;
        break;
    case Maze::Key_W:
        newy=y-1;
        break;
    case Maze::Key_A:
        newx=x-1;
        break;
    case Maze::Key_D:
        newx=x+1;
        break;
    case Maze::Key_Space:
        bomb(newy,newx);
        break;
    }
    if (MAZE[newy][newx] == Maze::Player && check(x,y,newx,newy))
    {
        starcount++;
    }
    printf("%d\n", starcount);
    if (MAZE[newy][newx] == Maze::Exit && starcount == 5)
    {
        MAZE[y][x] = Maze::Floor;
        MAZE[newy][newx] = Maze::Player;
        return false;
    }
    if (MAZE[newy][newx] != Maze::Wall && MAZE[newy][newx] != Maze::Exit)
    {
        if (newy<HEIGHT && newy>-1 && newx<WIDTH && newx>-1)
        {
            MAZE[y][x] = Maze::Floor;
            MAZE[newy][newx] = Maze::Player;
            x=newx;
            y=newy;
        }
    }
    Maze::Update((int*)(MAZE), WIDTH, HEIGHT);
    return true;
}

int main(int argc, char** argv)
{
    srand(time(0));
    Read();
    StarGen();
    //Предварительно нужно считать лабиринт в массив MAZE
    Maze::Initialize(argc, argv);
    Maze::CreateWindow(800, 600);
    Maze::Start(Loop);
    Maze::DeInitialize();
    return 0;
}

