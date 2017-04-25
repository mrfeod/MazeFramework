#include "Maze.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace Maze;

int new_y, new_x, exit_y, exit_x;
int x, y;
int IY[500];
int JX[500];
int MAZE[100][100];
int WIDTH = 10, HEIGHT = 4;
int CANDIDAT = 0;
int Stars = 5;
int sosedi(int y, int x)
{
         int sosed = 0;
         if(y > 0 && MAZE[y - 1][x] == Floor)
         {
             sosed = sosed + 1;
         }
         if(y < HEIGHT && MAZE[y + 1][x] == Floor)
         {
             sosed  = sosed + 1;
         }
         if(x > 0 && MAZE[y][x - 1] == Floor)
         {
             sosed  = sosed + 1;
         }
         if(x < WIDTH && MAZE[y][x + 1] == Floor)
         {
             sosed  = sosed + 1;
         }
    return sosed;
}
bool Deadendes(int y, int x)
{
    return sosedi(y, x) == 1;
}
bool Cornerers(int y, int x)
{
    bool top = MAZE[y - 1][x]== Floor;
    bool bot = MAZE[y + 1][x] == Floor;
    bool left = MAZE[y][x - 1] == Floor;
    bool right = MAZE[y][x + 1] == Floor;
    if(sosedi(y, x) == 2)
    {
        return (top && left) || (top && right) || (bot && left) || (bot && right);
    }
    return false;
}
void Candidats()
{
    for(int i = 1; i < HEIGHT - 1; i++)
    {
        for(int j = 1; j < WIDTH - 1; j++)
        {
            bool corner = Cornerers(i, j);
            bool deadend = Deadendes(i, j);
              if(MAZE[i][j] == Floor && (corner || deadend))
            {
                IY[CANDIDAT] = i;
                JX[CANDIDAT] = j;
                ++CANDIDAT;
            }
        }
    }

    for( int i = 0; i < CANDIDAT; ++i)
    {
        std::cout << IY[i] << " " << JX[i] << std::endl;
    }
    int Star = Stars;
    while(Star > 0)
    {
        int num = rand()% CANDIDAT;
        while(MAZE[IY[num]][JX[num]] == Player && num < CANDIDAT)
            ++num;
        if(num == CANDIDAT) --num;

        if(MAZE[IY[num]][JX[num]] == Player)
        {
            num = 0;
            while(MAZE[IY[num]][JX[num]] == Player && num < CANDIDAT)
                ++num;
            if(num == CANDIDAT) --num;
        }
        if( MAZE[IY[num]][JX[num]] != Player)
        {
            MAZE[IY[num]][JX[num]] = Player;
            Star--;
        }
        else
            Star = 0;
    }
}

void Read(const char* filename)
{
    FILE * f;
    f = fopen("maze.txt", "r");
    fscanf(f, "%d %d", &WIDTH, &HEIGHT );

    for(int i = 0; i < HEIGHT; ++i)
    {
        for(int j = 0; j < WIDTH; ++j)
        {
            fscanf(f, "%d ", &MAZE[i][j]);
            if(MAZE[i][j] == Player)
            {
                x = j;
                y = i;
            }
            else if(MAZE[i][j] == Exit)
            {
                exit_x = j;
                exit_y = i;
            }
        }
    }
    fclose(f);
    for(int i = 0; i < HEIGHT; ++i)
    {
        for(int j = 0; j < WIDTH ; ++j)
        {
            printf( "%d ", MAZE[i][j]);
        }

        std::cout << "\n" ;
    }

}


//Функция в которую приходит код клавиши key
//Результ работы - булево значение, указывающее продолжать работу или нет
bool Loop(int key)
{
   new_y = y;
   new_x = x;
    switch(key)
    {
    case Key_W :
        new_y = y - 1;
        break;
    case Key_S :
        new_y = y + 1;
        break;
    case Key_A :
        new_x = x - 1;
        break;
    case Key_D :
        new_x = x + 1;
        break;
    }
    if(new_y > HEIGHT) new_y = HEIGHT - 1;
    if(new_y < 0) new_y = 0;
    if(new_x > WIDTH) new_x = WIDTH - 1;
    if(new_x < 0) new_x = 0;

    if(MAZE[new_y][new_x] != Wall && MAZE[new_y][new_x] != Exit)
    {
       if(MAZE[new_y][new_x] == Player && (new_y != y || new_x != x))
       {
           --Stars;
       }

       MAZE[y][x] = Floor;
       x = new_x;
       y = new_y;
       MAZE[y][x] = Player;
    }

    if(Stars <= 0)
         MAZE[exit_y][exit_x] = Player;

    Maze::Update((int*)(MAZE), WIDTH, HEIGHT);
    if(exit_x == x && exit_y == y)
        return false;
    return true;
}

int main(int argc, char** argv)
{
    srand (time(0));
    Read("maze.txt");
    Candidats();
    Maze::Initialize(argc, argv);
    Maze::CreateWindow(800, 600);
    Maze::Start(Loop);
    Maze::DeInitialize();
    return 0;
}
