#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

enum Color
{
    White, // Initital 0
    Blue, // Player One
    Red, // Player Two
    Black //Explosion
};

#include "simulator.cpp"

void scanInput(int Max[5][6], int Record[5][6], Color color[5][6])
{
    int n;

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            std::cin >> Max[i][j];
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            std::cin >> Record[i][j];
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            std::cin >> n;
            color[i][j] = Color(n);
        }
    }

}

int main(void)
{
    int Record[5][6];
    int Max[5][6];
    Color color[5][6];

    scanInput(Max, Record, color);

    makeMove(Record, Max, color, Red);

    return 0;
}
