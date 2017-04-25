#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <fstream>

#include "Maze.h"

using namespace std;

int MAZE[100][100];
int WIDTH = 10, HEIGHT = 4;

int bombs = 2;
int chance = 20;
int points = 0;
int points_picked = 0;

int player_x = 0;
int player_y = 0;

bool DebugMode = false;

const string UP = "up";
const string DOWN = "down";
const string LEFT = "left";
const string RIGHT = "right";

void SpawnBombs()
{
    int rnd = rand() % 1000;

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (MAZE[i][j] == Maze::Floor && rnd < chance)
            {
                MAZE[i][j] = Maze::Player;
                rnd = rand() % 1000;
            }
            else
            {
                rnd = rand() % 1000;
            }
        }
    }
}

bool isEnd (int x, int y){
    if (MAZE[x+1][y] == Maze::Wall && MAZE[x-1][y] == Maze::Wall && MAZE[x][y+1] == Maze::Wall && MAZE[x][y - 1] != Maze::Wall)
        return true;
    else if (MAZE[x+1][y] == Maze::Wall && MAZE[x-1][y] == Maze::Wall && MAZE[x][y+1] != Maze::Wall && MAZE[x][y - 1] == Maze::Wall)
        return true;
    else if (MAZE[x+1][y] == Maze::Wall && MAZE[x-1][y] != Maze::Wall && MAZE[x][y+1] == Maze::Wall && MAZE[x][y - 1] == Maze::Wall)
        return true;
    else if (MAZE[x+1][y] != Maze::Wall && MAZE[x-1][y] == Maze::Wall && MAZE[x][y+1] == Maze::Wall && MAZE[x][y - 1] == Maze::Wall)
        return true;
    else
        return false;
}

bool inTheField (int _x, int _y)
{
    if (_x > -1 && _x < 22 && _y > -1 && _y < 22)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool inTheSuperInnerField (int _x, int _y)
{
    if (_x > 2 && _x < 20 && _y > 2 && _y < 20)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SpawnPoints (){
    int rnd = rand() % 1000;

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (MAZE[i][j] == Maze::Floor && isEnd(i, j) && inTheField(j, i))
            {
                MAZE[i][j] = Maze::Exit;
                rnd = rand() % 1000;
                points++;
            }
            else
            {
                rnd = rand() % 1000;
            }
        }
    }
}

void Read()
{
    system("cls");

    FILE* _FILE;

    _FILE = fopen("maze.txt", "r");

    fscanf (_FILE, "%i %i", &WIDTH, &HEIGHT);

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            fscanf (_FILE, "%i", &MAZE[i][j]);
        }
    }

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            cout << MAZE[i][j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (MAZE[j][i] == Maze::Player)
            {
                player_x = j;
                player_y = i;
                break;
            }
        }
    }

    fclose(_FILE);
}

bool inTheInnerField (int _x, int _y)
{
    if (_x > 0 && _x < 21 && _y > 0 && _y < 21)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Debug (string Inpt)
{
    system("cls");

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            cout << MAZE[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Input: " << Inpt << endl;
    cout << "Bombs: " << bombs << endl;
    cout << "Points: " << points << endl;
    cout << "Picked points: " << points_picked << endl;
}

void BigBoom()
{
    if (inTheSuperInnerField(player_x, player_y))
    {
        MAZE[player_x+1][player_y] = 1;
        MAZE[player_x-1][player_y] = 1;
        MAZE[player_x][player_y+1] = 1;
        MAZE[player_x][player_y-1] = 1;
        MAZE[player_x+1][player_y+1] = 1;
        MAZE[player_x-1][player_y-1] = 1;
        MAZE[player_x+1][player_y-1] = 1;
        MAZE[player_x-1][player_y+1] = 1;
        MAZE[player_x+2][player_y+2] = 1;
        MAZE[player_x+1][player_y+2] = 1;
        MAZE[player_x][player_y+2] = 1;
        MAZE[player_x-1][player_y-2] = 1;
        MAZE[player_x-2][player_y-2] = 1;
        MAZE[player_x+2][player_y-2] = 1;
        MAZE[player_x+1][player_y-2] = 1;
        MAZE[player_x][player_y-2] = 1;
        MAZE[player_x-1][player_y-2] = 1;
        MAZE[player_x-2][player_y-2] = 1;
        bombs -= 5;
    }
}

void Boom()
{
    if (inTheInnerField(player_x, player_y))
    {
        MAZE[player_x+1][player_y] = 1;
        MAZE[player_x-1][player_y] = 1;
        MAZE[player_x][player_y+1] = 1;
        MAZE[player_x][player_y-1] = 1;
        bombs--;
    }
}

bool isOK (int _x, int _y, string dir)
{
    if (dir == UP && MAZE[_x-1][_y] != 0 && inTheField(_x, _y))
    {
        if (MAZE[_x-1][_y] == Maze::Player)
            bombs++;
        if (MAZE[_x-1][_y] == Maze::Exit && _x != 20 && _y != 11)
            points_picked++;

        return true;
    }
    if (dir == DOWN && MAZE[_x+1][_y] != 0 && inTheField(_x, _y))
    {
        if (MAZE[_x+1][_y] == Maze::Player)
            bombs++;
        if (MAZE[_x+1][_y] == Maze::Exit && _x != 20 && _y != 11)
            points_picked++;

        return true;
    }
    if (dir == LEFT && MAZE[_x][_y-1] != 0 && inTheField(_x, _y))
    {
        if (MAZE[_x][_y-1] == Maze::Player)
            bombs++;
        if (MAZE[_x][_y-1] == Maze::Exit && _x != 20 && _y != 11)
            points_picked++;

        return true;
    }
    if (dir == RIGHT && MAZE[_x][_y+1] != 0 && inTheField(_x, _y))
    {
        if (MAZE[_x][_y+1] == Maze::Player)
            bombs++;
        if (MAZE[_x][_y+1] == Maze::Exit && _x != 20 && _y != 11)
            points_picked++;

        return true;
    }
    else
        return false;
}

void Move (string dir)
{
    if (dir == UP && isOK(player_x, player_y, dir))
    {
        MAZE[player_x][player_y] = Maze::Floor;
        MAZE[player_x-1][player_y] = Maze::Player;
        player_x -= 1;
    }
    if (dir == DOWN && isOK(player_x, player_y, dir))
    {
        MAZE[player_x][player_y] = Maze::Floor;
        MAZE[player_x+1][player_y] = Maze::Player;
        player_x += 1;
    }
    if (dir == LEFT && isOK(player_x, player_y, dir))
    {
        MAZE[player_x][player_y] = Maze::Floor;
        MAZE[player_x][player_y-1] = Maze::Player;
        player_y -= 1;
    }
    if (dir == RIGHT && isOK(player_x, player_y, dir))
    {
        MAZE[player_x][player_y] = Maze::Floor;
        MAZE[player_x][player_y+1] = Maze::Player;
        player_y += 1;
    }
    if (MAZE[20][11] != Maze::Player && points_picked >= points)
    {
        MAZE[20][11] = Maze::Floor;
    }
}

bool Loop(int key)
{
    switch (key)
    {
    case Maze::Key_S:
        Move(DOWN);
        Debug("S");
        break;
    case Maze::Key_W:
        Move(UP);
        Debug("W");
        break;
    case Maze::Key_D:
        Move(RIGHT);
        Debug("D");
        break;
    case Maze::Key_A:
        Move(LEFT);
        Debug("A");
        break;
    case Maze::Key_Space:
        if (bombs > 0)
            Boom();
        Debug("BOOM");
        break;
    case Maze::Key_B:
        if (bombs >= 5)
            BigBoom();
        Debug("BIG BOOM");
        break;
    }

    Maze::Update((int*)(MAZE), WIDTH, HEIGHT);

    if (MAZE[20][11] == Maze::Player && points_picked >= points)
    {
        return false;
    }else if (MAZE[20][11] != Maze::Player && points_picked < points)
    {
        MAZE[20][11] = Maze::Exit;
        return true;
    }else if (MAZE[20][11] != Maze::Player && points_picked >= points)
    {
        MAZE[20][11] = Maze::Floor;
        return true;
    }
    else
        return true;
}

void ReadMode (){
    string Mode = "";

    ifstream In ("config.txt");

    In >> Mode >> Mode;

    In.close();

    if (Mode == "false")
        DebugMode = false;
    if (Mode == "true")
        DebugMode = true;
}

int main(int argc, char** argv)
{
    ReadMode();

    if (DebugMode){
        cout << "Enter number of bombs: ";
        cin >> bombs;

        cout << "Enter chance of bomb spawn: ";
        cin >> chance;
    }

    srand(time(0));

    Read();

    SpawnBombs();
    SpawnPoints();

    Maze::Initialize(argc, argv);
    Maze::CreateWindow(800, 600);
    Maze::Start(Loop);
    Maze::DeInitialize();
    return 0;
}
