#include "stdio.h"

#include "Maze.h"

using namespace std;

int MAZE[100][100];
int WIDTH = 10, HEIGHT = 4;
int I[441],J[441];
int CANDIDATES = 0;
int STARS = 5;
int X;
int Y;
int new_X;
int new_Y;
int Exit_X;
int Exit_Y;

int Neighbours(int x, int y)
{
    int neighbours = 0;
    if(MAZE[y-1][x] ==Maze::Floor)++neighbours;
    if(MAZE[y][x-1] ==Maze::Floor)++neighbours;
    if(MAZE[y+1][x] ==Maze::Floor)++neighbours;
    if(MAZE[y][x+1] ==Maze::Floor)++neighbours;
    return neighbours;
}
bool isCorner(int x,int y)
{
    bool top = MAZE[y-1][x] == Maze::Floor;
    bool bot = MAZE[y+1][x] == Maze::Floor;
    bool left = MAZE[y][x-1] == Maze::Floor;
    bool right = MAZE[y][x+1] == Maze::Floor;
    if(Neighbours(x,y) == 2)
    {
        return  (top && left)  ||
                (top && right) ||
                (bot && left)  ||
                (bot && right) ;
    }
    return false;
}
bool isDeadend(int x,int y)
{
    return Neighbours(x,y) == 1;
}
void Generate()
{
    for(int i = 1; i < HEIGHT-1; ++i)
        for(int j = 1; j < WIDTH-1; ++j)
        {
            if(MAZE[i][j] == Maze::Floor && (isCorner(j, i) || isDeadend(j, i)))
            {
                I[CANDIDATES] = i;
                J[CANDIDATES] = j;
                ++CANDIDATES;
            }
        }

    for(int i = 0; i < CANDIDATES; ++i)
        printf("%d %d\n", I[i], J[i]);
}
void Read()
{
    FILE* f = fopen("maze.txt", "r");
    if(!f)
        return ;

    fscanf(f, "%d %d", &WIDTH, &HEIGHT);
    for(int i = 0; i < HEIGHT; ++i)
    {
        for(int j = 0; j < WIDTH; ++j)
        {
            fscanf(f, "%d", &(MAZE[i][j]));
            if(MAZE[i][j] == Maze::Player)
            {
                X=j;
                Y=i;
            }
            else if(MAZE[i][j] == Maze::Exit)
            {
                Exit_Y = i;
                Exit_X = j;
            }

        }
    }

    fclose(f);
}

//Функция в которую приходит код клавиши key
//Результ работы - булево значение, указывающее продолжать работу или нет
bool Loop(int key)
{
    new_X = X;
    new_Y = Y;
    switch(key)
    {
    case Maze::Key_W:
        new_Y--;
        break;
    case Maze::Key_S:
        new_Y++;
        break;
    case Maze::Key_A:
        new_X--;
        break;
    case Maze::Key_D:
        new_X++;
        break;
    }
    if(MAZE[new_Y][new_X] == Maze::Exit)
    {
        return false;
    }
    if(MAZE[new_Y][new_X] != Maze::Wall)
        if(new_Y < HEIGHT)
            if(new_Y > -1)
                if(new_X < WIDTH)
                    if(new_X > -1)
                    {
                        MAZE[Y][X] = Maze::Floor;
                        Y = new_Y;
                        X = new_X;
                        MAZE[Y][X] = Maze::Player;
                    }


    Maze::Update((int*)(MAZE), WIDTH, HEIGHT);
    return true;
}

int main(int argc, char** argv)
{
    Read();
    Generate();
    //Предварительно нужно считать лабиринт в массив MAZE
    Maze::Initialize(argc, argv);
    Maze::CreateWindow(800, 600);
    Maze::Start(Loop);
    Maze::DeInitialize();
    return 0;
}
