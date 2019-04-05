#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <string>

using namespace std;

#define MAX_MN 1000
#define FLOOR '0'
#define BLOCK '1'
#define DEBUG 0

struct Point
{
    Point (int x_, int y_)
    {
        x = x_;
        y = y_;
    }

    friend ostream& operator << (ostream& out, const Point& obj)
    {
        if (DEBUG) cout << obj.x << ' ' << obj.y;
        return out;
    }

    friend bool operator == (const Point& a, const Point& b)
    {
        return (a.x == b.x && a.y == b.y);
    }

    friend bool operator != (const Point& a, const Point& b)
    {
        return (a.x != b.x || a.y != b.y);
    }

    int x, y;
};

int   m, n, charge;
int   dist[MAX_MN][MAX_MN];
char  type[MAX_MN][MAX_MN];
bool  clean[MAX_MN][MAX_MN];
stack <Point> cleanList;
vector<deque<Point>> logger;
const Point dir[4] = {{0,1},{-1,0},{0,-1},{1,0}};

inline bool inRange(int i, int j)
{
    return (i >= 0 && i < m && j >= 0 && j < n);
}

void ReadMap(const char* arg)
{
    ifstream fin("./" + string(arg) + "/floor.data");

    if (!fin.is_open())
    {
        cout << "Unable to read file\n";
        exit(1);
    }

    fin >> m >> n >> charge;

    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            fin >> type[i][j];
            clean[i][j] = false;
            dist[i][j] = -1;
        }
    }

    fin.close();

    if (DEBUG)
    {
        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (DEBUG) cout << type[i][j] << "\n "[j < n-1];
            }
        }
        if (DEBUG) cout << '\n';
    }
}

void SetMinDist(Point& start, bool isFirst)
{
    bool distRec[MAX_MN][MAX_MN] = {{false}};
    queue <Point> q;

    dist[start.x][start.y] = 0;

    for (q.emplace(start); !q.empty(); q.pop())
    {
        Point now = q.front();

        for (int k = 0; k < 4; ++k)
        {
            int i = now.x + dir[k].x;
            int j = now.y + dir[k].y;

            if (inRange(i, j))
            {
                if (!distRec[i][j] && type[i][j] == FLOOR)
                {
                    dist[i][j] = dist[now.x][now.y] + 1;
                    distRec[i][j] = true;
                    q.emplace(i, j);
                    if (isFirst) cleanList.emplace(i, j);
                }
            }
        }
    }

    if (DEBUG)
    {
        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (DEBUG) cout << dist[i][j] << "\n "[j < n-1];
            }
        }
        if (DEBUG) cout << '\n';
    }
}

void FindWalk(const Point& start, const Point& dest, deque<Point>& poslog, bool isGo)
{
    Point now = dest;

    if (!isGo) poslog.emplace_back(dest);

    while (now != start)
    {
        Point next = now;

        clean[now.x][now.y] = true;

        //if (DEBUG && !isGo) if (DEBUG) cout << "current point: " << now 
            //<< " type: " << type[now.x][now.y] << '\n';

        // Traverse through 4 adjacent cells
        for (int k = 0; k < 4; ++k)
        {
            Point p(now.x+dir[k].x, now.y+dir[k].y);
            //if (DEBUG && !isGo) if (DEBUG) cout << "adjacent point: " << p;

            if (inRange(p.x, p.y))
            {
                //if (DEBUG && !isGo) if (DEBUG) cout << " is in range";
                if (dist[p.x][p.y] == dist[now.x][now.y]-1)
                {
                    //if (DEBUG && !isGo) if (DEBUG) cout << " is valid";
                    next = p;
                    if (!clean[next.x][next.y])
                    {
                        //if (DEBUG && !isGo) if (DEBUG) cout << " is ideal\n";
                        break;
                    }
                }
            }
            //else if (DEBUG && !isGo) if (DEBUG) cout << " is not in range";
            //if (DEBUG && !isGo) if (DEBUG) cout << '\n';
        }

        if (isGo) poslog.emplace_front(next);
        else poslog.emplace_back(next);

        now = next;
    }

    if (DEBUG && !isGo) 
    {
        for (auto p : poslog) if (DEBUG) cout << p << '\n';
        if (DEBUG) cout << '\n';
    }
}

void FindPath(void)
{
    Point root(-1,-1);

    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (type[i][j] == 'R')
            {
                root = Point(i,j);
                break;
            }
        }
    }

    SetMinDist(root, true);

    Point start(-1,-1);
    while (!cleanList.empty())
    {
        Point now = cleanList.top();
        deque<Point> poslog;

        if (DEBUG) cout << "Clean map:\n";
        for (int j = 0; j < m; ++j)
        {
            for (int k = 0; k < n; ++k)
            {
                if (DEBUG) cout << clean[j][k] << "\n "[k < n-1];
            }
        }

        if (DEBUG) cout << "number of points to clean: " << cleanList.size() << '\n';

        if (clean[now.x][now.y])
        {
            if (DEBUG) cout << "Cleaning position " << now << " cleaned\n\n";
            cleanList.pop();
            continue;
        }

        if (DEBUG) cout << "Cleaning position " << now << '\n';

        FindWalk(root, now, poslog, true);
        FindWalk(root, now, poslog, false);

        if (start.x == -1) start = poslog[poslog.size()-2];
        else
        {
            if (start != poslog[poslog.size()-2])
            {
                if (DEBUG) cout << "Find path from last enter position: " << start 
                    << " to start position: " << poslog[poslog.size()-2] << '\n';
                deque<Point> tmplog;

                SetMinDist(start, false);
                FindWalk(start, poslog[poslog.size()-2], tmplog, true);
                logger.emplace_back(tmplog);

                SetMinDist(root, false);
                start = poslog[poslog.size()-2];
            }
        }

        poslog.pop_front();
        logger.emplace_back(poslog);
        cleanList.pop();
    }
}

void WriteLog(const char *arg)
{
    ofstream fout("./" + string(arg) + "/final.path");

    if (!fout.is_open())
    {
        cout << "Unable to write file\n";
        exit(1);
    }

    int num = 0;

    for (auto d : logger) num += d.size();

    fout << num << '\n';

    for (auto d : logger)
    {
        for (auto p : d) 
        {
            fout << p.x << ' ' << p.y << '\n';
        }
    }

    fout.close();
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Invalid number of arguments\n";
        exit(1);
    }

    ReadMap(argv[1]);

    FindPath();
    
    WriteLog(argv[1]);

    return 0;
}
