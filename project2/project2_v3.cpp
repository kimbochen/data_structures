#include <algorithm>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iterator>
#include <iostream>
#include <queue>
#include <string>
#include <stack>
#include <vector>

using namespace std;

struct Point
{
    Point(int x_, int y_)
    {
        x = x_;
        y = y_;
    }

    int x, y;
};

Point root;
int m, n, steps;
int battery, charge;
vector <deque<Point>> poslog;
vector <vector<char>> type;
vector <vector<bool>> clean;
vector <vector<int>> dist;
vector <vector<int>> cost;
vector <Point> dir = {{1,0},{0,-1},{-1,0},{0,1}};
deque  <Point> clist;

void ReadMap(const char& *arg)
{
    ifstream fin("./" + string(arg) + "/floor.data");

    if (!fin.is_open())
    {
        cout << "Unable to read file\n";
        exit(1);
    }

    fin >> m >> n >> charge;

    type.resize(m, vector<char>(n));
    clean.resize(m, vector<bool>(n, false));
    dist.resize(m, vector<int>(n, -1));

    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            fin >> type[i][j];

            if (type[i][j] == 'R')
                root = Point(i, j);
        }
    }
}

void setMinDist(void)
{
    queue <Point> q;

    dist[root.x][root.y] = 0;

    for (q.emplace(root); !q.empty(); q.pop())
    {
        Point now = q.front();

        clist.emplace_front(now);

        for (auto d : dir)
        {
            int i = now.x + d.x;
            int j = now.y + d.y;

            if (dist[i][j] == -1 && type[i][j] != '1')
            {
                dist[i][j] = dist[now.x][now.y] + 1;
                q.emplace(i, j);
            }
        }
    }
}

void WriteLog(const char& *arg)
{
    ofstream fout("./" + string(arg) + "/final.path");

    if (!fout.is_open())
    {
        cout << "Unable to write file\n";
        exit(1);
    }

    steps = 0;
    for (auto d : poslog)
    {
        steps += d.size();
    }

    cout << steps << '\n';

    for (auto p : posLog)
    {
        cout << p.x << ' ' << p.y << '\n';
    }
}

void setMaxCost(int i, int j)
{
    
    queue <Point> q;

    for (q.emplace(i,j); !q.empty(); q.pop())
    {
        Point p = q.front();


    }
}

void FindPath(void)
{
    Point start = root;

    for (auto d : dir)
    {
        int i = root.x + d.x;
        int j = root.y + d.y;

        if (i >= 0 && i < m && j >= 0 && j < n)
        {
            start = Point(i, j);
            break;
        }
    }

    for (auto c : clist)
    {
        deque <Point> cycle;

        for (Point cur = c;cur != start; )
        {
            cycle.emplace_back(cur);

            Point next = cur;

            for (auto d : dir)
            {
                int i = cur.x + d.x;
                int j = cur.y + d.y;

                if (i >= 0 && i < m && j >= 0 && j < n)
                {
                    next = Point(i, j);
                    break;
                }
            }

            for (auto d : dir)
            {
                int i = cur.x + d.x;
                int j = cur.y + d.y;

                if (i >= 0 && i < m && j >= 0 && j < n)
                {
                    if (!clean[i][j])
                    {
                        next = Point(i, j);
                        break;
                    }
                }
            }

            cur = next;
        }

        setMaxCost(c.x, c.y);

        for (Point cur = c; cur != root; )
        {
            Point next = cur;

            if (battery > dist[cur.x][cur.y])
            {
                int max_cost = 0;

                for (auto d : dir)
                {
                    int i = cur.x + d.x;
                    int j = cur.y + d.y;

                    if (i >= 0 && i < m && j >= 0 && j < n)
                    {
                        if (cost[i][j] > max_cost)
                        {
                            max_cost = cost[i][j];
                            next = Point(i, j);
                        }
                    }
                }
            }
            else
            {
                for (auto d : dir)
                {
                    int i = cur.x + d.x;
                    int j = cur.y + d.y;

                    if (i >= 0 && i < m && j >= 0 && j < n)
                    {
                        if (dist[i][j] == dist[cur.x][cur.y] - 1)
                        {
                            next = Point(i, j);
                        }
                    }
                }
            }

            cycle.emplace_back(next);
            cur = next;
            battery --;
        }
    }

    start = cycle[cycle.size()-2];
    poslog.emplace_back(cycle);
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Invalid number of arguments\n";
        exit(1);
    }

    ReadMap(argv[1]);

    setMinDist();

    FindPath();

    WriteLog();

    return 0;
}
