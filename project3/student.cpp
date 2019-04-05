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

class Node
{
public:
    Node (void)
    {
        x = y = -1;

        inpC = oppC = Black;
    }

    Node (const int m[5][6], const int r[5][6], const Color c[5][6], int _x, int _y, Color _inpC)
    {
        x = _x;
        y = _y;

        inpC = _inpC;
        oppC = (inpC == Blue)? Red : Blue;


        for (int i = 0; i < 4; ++i)
        {
            inpOrb[i] = oppOrb[i] = 0;
        }

        blkOrb = expOrb = 0;

        simulate(m, r, c);

        calcData(m);
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

    void printData(void)
    {
        std::cout << "choice: " << x << ' ' << y << '\n';
        std::cout << "| type | total | cd 1 | cd 2 | cd 3 | cd 4 |" << '\n';

        std::cout << "| inp  |  ";
        for (int i = 0; i < 4; ++i) std::cout << inpOrb[i] << "  |  ";
        std::cout << '\n';

        std::cout << "| opp  |  ";
        for (int i = 0; i < 4; ++i) std::cout << oppOrb[i] << "  |  ";
        std::cout << '\n';

        std::cout << "blank: " << blkOrb << ", exploded: " << expOrb << "\n\n";
    }

    friend bool operator > (const Node& lf, const Node& rt)
    {
        if (lf.oppOrb[1] == rt.oppOrb[1])
        {
            if (lf.oppOrb[0] == rt.oppOrb[0])
            {
                if (lf.inpOrb[1] == rt.inpOrb[1])
                {
                    if (lf.inpOrb[0] == rt.inpOrb[0])
                    {
                        if (lf.blkOrb == rt.blkOrb)
                        {
                            return (lf.expOrb > rt.expOrb);
                        }
                        else return (lf.blkOrb < rt.blkOrb);
                    }
                    else return (lf.inpOrb[0] > rt.inpOrb[0]);
                }
                else return (lf.inpOrb[1] > rt.inpOrb[1]);
            }
            else return (lf.oppOrb[0] < rt.oppOrb[0]);
        }
        else return (lf.oppOrb[1] < rt.oppOrb[1]);
    }

    int   x, y;
    Color inpC, oppC;

private:
    inline bool isValid(int i, int j)
    {
        return (i >= 0 && i < 5 && j >= 0 && j < 6);
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

    void calcData(const int Max[5][6])
    {
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                int cntDwn = Max[i][j] - simR[i][j];

                if (simC[i][j] == inpC)
                {
                    inpOrb[0] ++;
                    inpOrb[cntDwn] ++;
                }
                else if (simC[i][j] == oppC)
                {
                    oppOrb[0] ++;
                    oppOrb[cntDwn] ++;
                }
                else if (simC[i][j] == White)
                {
                    blkOrb ++;
                }
                else expOrb ++;
            }
        }
    }

    int   simR[5][6];
    Color simC[5][6];

    int   inpOrb[4], oppOrb[4];
    int   blkOrb, expOrb;
};

class Student{
public:

    void merge(Node *start, int len)
    {
        int mid = len / 2, i = 0, j = 0, k = 0;
        int lenL = mid + 1, lenR = len - mid;
    
        Node lf[lenL], rt[lenR];
    
        for (int x = 0; x < lenL; ++x) lf[x] = start[x];
        for (int x = 0; x < lenR; ++x) rt[x] = start[mid+1+x];
    
        while (i < lenL && j < lenR)
        {
            if (lf[i] > rt[j]) start[k++] = lf[i++];
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

        x = choiceList[0].x;
        y = choiceList[0].y;
        
        //std::cout << "choice list: \n";
        //for (int i = 0; i < 3; ++i)
        //{
        //    choiceList[i].printData();
        //}
    }

    int getX(void)
    {
        return x;
    }

    int getY(void)
    {
        return y;
    }

private:
    int x;
    int y;
};
