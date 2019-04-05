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

struct Offset
{
    int x, y;
};

Offset dir[4] = { {0,1}, {-1,0}, {0,-1}, {1,0} };

inline bool isValid(int i, int j)
{
    return (i >= 0 && i < 5 && j >= 0 && j < 6);
}

struct DangerIndex
{
    DangerIndex(void)
    {
        cntDwn = orbNum = -1;
    }

    DangerIndex(int critNum, int record)
    {
        cntDwn = critNum - record;
        orbNum = 0;
    }

    void calcOrbNum(int Max[5][6], int Record[5][6], bool Counted[5][6], int x, int y)
    {
        if (Record[x][y] >= Max[x][y]) return;

        Counted[x][y] = true;

        for (Offset d : dir)
        {
            int i = x + d.x, j = y + d.y;

            if (isValid(i, j))
            {
                if (!Counted[i][j] && Record[i][j] < Max[i][j])
                {
                    if (Record[i][j] == Max[i][j]-1)
                    {
                        calcOrbNum(Max, Record, Counted, i, j);
                    }
                    else orbNum ++;
                }
            }
        }
    }

    friend std::ostream& operator<< (std::ostream& os, const DangerIndex& obj)
    {
        os << "orbNum: " << obj.orbNum << ", "
            << "cntDwn: " << obj.cntDwn << '\n';
        return os;
    }

    int cntDwn, orbNum;
};

struct Cell
{
    DangerIndex idx;
    Color clr;
    int i, j;
};

bool cellCmp(const Cell& a, const Cell& b)
{
    if (a.idx.cntDwn == b.idx.cntDwn)
    {
        if (a.idx.orbNum == b.idx.orbNum)
        {

        }
        else return (a.idx.orbNum > b.idx.orbNum);
    }
    else return (a.idx.cntDwn < b.idx.cntDwn);
}

void merge(Cell *start, int len)
{
    int mid = len / 2, i = 0, j = 0, k = 0;
    int lenL = mid + 1, lenR = len - mid;

    Cell lf[lenL], rt[lenR];

    for (int x = 0; x < lenL; ++x) lf[x] = start[x];
    for (int x = 0; x < lenR; ++x) rt[x] = start[mid+1+x];

    while (i < lenL && j < lenR)
    {
        if (cellCmp(lf[i], rt[j])) start[k++] = lf[i++];
        else start[k++] = rt[j++];
    }

    while (i < lenL) start[k++] = lf[i++];
    while (j < lenR) start[k++] = rt[j++];
}

void mergeSort(Cell* start, Cell* end)
{
    int len = end - start;

    if (len > 0)
    {
        int mid = len / 2;

        mergeSort(start, start+mid);

        mergeSort(start+(mid+1), end);

        merge(start, len);
    }
}

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

    Cell choiceList[30];

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            bool Counted[5][6] = { {false} };
            int k = 6 * i + j;

            choiceList[k].i = i;  choiceList[k].j = j;

            choiceList[k].clr = color[i][j];

            choiceList[k].idx = DangerIndex(Max[i][j], Record[i][j]);

            choiceList[k].idx.calcOrbNum(Max, Record, Counted, i, j);
        }
    }

    mergeSort(&choiceList[0], &choiceList[29]);

    for (int i = 0; i < 30; ++i)
    {
        std::cout << choiceList[i].i << ' ' << choiceList[i].j << '\n';
    }

    return 0;
}
