//#define WRITE 0

#ifdef WRITE
    #include <iostream>
    #include <vector>
    #include <queue>
    #include <cstring>

    using namespace std;

    enum Color {White, Blue, Red, Black};
#endif

struct Offset
{
    int x, y;
};

struct Node
{
    Node (void)
    {
        x = y = -1;
        inpC = oppC = Black;
        key = 3985;
    }
    Node (const int m[5][6], const int r[5][6], const Color c[5][6], int _x, int _y, Color _inpC)
    {
        x = _x;
        y = _y;
        inpC = _inpC;
        oppC = (inpC == Blue)? Red : Blue;
        simulate(m, r, c);
        calcKey(m, r, c);
    }

    inline bool isValid(int i, int j)
    {
        return (i >= 0 && i < 5 && j >= 0 && j < 6);
    }

    void printRecord(void)
    {
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                std::cout << simR[i][j] << "\n "[j < 5];
            }
        }
    }

    void printColor(void)
    {
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                std::cout << simC[i][j] << "\n "[j < 5];
            }
        }
    }

    void simExplode(const int Max[5][6], int cx, int cy)
    {
        Offset dir[4] = { {0,1}, {-1,0}, {0,-1}, {1, 0} };
    
        simC[cx][cy] = Black;
    
        for (Offset d : dir)
        {
            int i = cx + d.x, j = cy + d.y;
    
            if (isValid(i, j) && simC[i][j] != Black)
            {
                simR[i][j] ++;
                simC[i][j] = inpC;
    
                if (simR[i][j] >= Max[i][j])
                {
                    simExplode(Max, i, j);
                }
            }
        }
    }
    
    void simulate(const int Max[5][6], const int Record[5][6], const Color color[5][6])
    {
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                simR[i][j] = Record[i][j];
                simC[i][j] = color[i][j];
            }
        }
    
        simR[x][y] ++;
        simC[x][y] = inpC;
    
        if (simR[x][y] >= Max[x][y])
        {
            simExplode(Max, x, y);
        }
    }

    int calcOrbDiff(const Color color[5][6])
    {
        int oNumInp = 0, oNumOpp = 0;
        int sNumInp = 0, sNumOpp = 0;

        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                if (color[i][j] == inpC) oNumInp ++;
                else if (color[i][j] == oppC) oNumOpp ++;

                if (simC[i][j] == inpC) sNumInp ++;
                else if (simC[i][j] == oppC) sNumOpp ++;
            }
        }

        int oDiff = oNumInp - oNumOpp;
        
        int sDiff = sNumInp - sNumOpp;

        //std::cout << "Original: Input " << oNumInp << ", Opponent " << oNumOpp << '\n';

        //std::cout << "Simulate: Input " << sNumInp << ", Opponent " << sNumOpp << '\n';

        return sDiff - oDiff;
    }

    int calcBlankOrb(void)
    {
        int blankOrb = 0;

        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                if (simC[i][j] == White) blankOrb ++;
            }
        }

        return blankOrb;
    }

    int calcCritOrb(const int Max[5][6])
    {
        int inpCritOrb = 0, oppCritOrb = 0;

        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                if (Max[i][j] - simR[i][j] == 1)
                {
                    if (simC[i][j] == inpC) inpCritOrb ++;
                    else if (simC[i][j] == oppC) oppCritOrb ++;
                }
            }
        }

        //std::cout << "input critical orbs: " << inpCritOrb
        //     << ", opponent critical orbs: " << oppCritOrb << '\n';

        return inpCritOrb - oppCritOrb;
    }

    void calcKey(const int Max[5][6], const int Record[5][6], const Color color[5][6])
    {
        int orbDiff = calcOrbDiff(color); // the bigger the better

        int blankOrb = calcBlankOrb(); // the smaller the better

        int critOrbDiff = calcCritOrb(Max); // the bigger the better

        key = 1 * critOrbDiff + 1 * blankOrb + 1 * orbDiff;
    }

    int simR[5][6];
    int x, y;
    int key;
    Color simC[5][6];
    Color inpC, oppC;
};

void merge(Node *start, int len)
{
    int mid = len / 2, i = 0, j = 0, k = 0;
    int lenL = mid + 1, lenR = len - mid;

    Node lf[lenL], rt[lenR];

    for (int x = 0; x < lenL; ++x) lf[x] = start[x];
    for (int x = 0; x < lenR; ++x) rt[x] = start[mid+1+x];

    while (i < lenL && j < lenR)
    {
        if (lf[i].key > rt[j].key) start[k++] = lf[i++];
        else start[k++] = rt[j++];
    }

    while (i < lenL) start[k++] = lf[i++];
    while (j < lenR) start[k++] = rt[j++];
}
void mergeSort(Node* start, Node* end)
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
void makeMove(int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor)
{
    int num = 0;

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            if (color[i][j] == inputColor || color[i][j] == White) num ++;
        }
    }

    Node choiceList[num];
    int k = 0;

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            if (color[i][j] == inputColor || color[i][j] == White)
            {
                Node node(Max, Record, color, i, j, inputColor);
                choiceList[k ++] = node;
            }
        }
    }

    mergeSort(&choiceList[0], &choiceList[num-1]);

    for (int i = 0; i < num; ++i)
    {
        std::cout << choiceList[i].x << ' ' << choiceList[i].y 
            << ": " << choiceList[i].key << '\n';
    }
}
