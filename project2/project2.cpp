#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <cstdlib>

#define DEBUG 0
#define vcpii vector<pair<int,int>>
#define vcint vector<int>
#define pii pair<int,int>

using namespace std;

enum {FREE, BLOCK, POWER};

vector<pii> dir = {{1,0},{0,-1},{-1,0},{0,1}};
vector <vcint> cell;
vector <vcpii> visit;
vector <pii> step;
int m, n;
int battery, charge;
int si, sj;

void ShowResult(string dest, ofstream& fout)
{
    fout.open(dest);

    if (!fout.is_open()) {
        cout << "Unable to open " << dest << '\n';
        exit(1);
    }

    fout << step.size() << '\n';

    for (auto p : step) {
        fout << p.first << ' ' 
            << p.second << '\n';
    }

    fout.close();
}

bool isValid(int i, int j)
{
    if (i >= 0 && i < m && j >= 0 && j < n)
        return (cell[i][j] == FREE && 
                visit[i][j] == make_pair(-1,-1));
    else
        return false;
}

void Log(int i, int j, int dist)
{
#ifdef DEBUG 
    cout << i << ' ' << j << ' '
        << battery << ' ' << dist << '\n';
#endif
    step.emplace_back(i, j);
}

void Recharge (pii now)
{
    vector <pii> retreat;

    pii p = visit[now.first][now.second];

    while(cell[p.first][p.second] != POWER) {
        retreat.emplace_back(p.first, p.second);
        p = visit[p.first][p.second];
    }

    for (auto p : retreat)
        Log(p.first, p.second, -1);
    
    Log(si, sj, -1);

    for (int i = retreat.size()-1; i >= 0; --i)
        Log(retreat[i].first, retreat[i].second, -1);

    Log(now.first, now.second, -1);

    battery = charge - (retreat.size() + 1);
}

void Walk(int i, int j, pii prev, int dist)
{
    visit[i][j] = prev;
    battery --;
    Log(i, j, dist);

    for (int k = 0; k < 4; ++k) {
        int ni = i + dir[k].first;
        int nj = j + dir[k].second;

        if (isValid(ni, nj)) {
            if (battery == dist) Recharge(make_pair(i,j));
            Walk(ni, nj, make_pair(i,j), dist+1);
            battery --;
            Log(i, j, dist);
        }
    }

    visit[i][j] = make_pair(-1,-1);
    cell[i][j] = FREE;
}

void Start(int i, int j)
{
    for (auto d : dir) {
        int x = i + d.first;
        int y = j + d.second;

        if (isValid(x, y)) {
            visit[x][y] = make_pair(i, j);
            Walk(x, y, visit[x][y], 1);
            break;
        }
    }

    Log(i, j, 0);
}

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);

    string infile, outfile;
    ifstream fin;
    ofstream fout;

    if (argc < 2) {
        cout << "invalid number of arguments" << '\n';
        exit(1);
    }
    
    infile = "./"  + string(argv[1]) + "/floor.data";
    outfile = "./" + string(argv[1]) + "/final.path";

    fin.open(infile);

    if (!fin.is_open()) {
        cout << "unable to open " << infile << '\n';
        exit(1);
    }

    fin >> m >> n >> charge;

    cell.resize(m, vcint(n));
    visit.resize(m, vcpii(n, make_pair(-1,-1)));
    battery = charge;

    si = sj = -1;
    string c;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            fin >> c;

            if (c == "R") {
                cell[i][j] = POWER;
                si = i;
                sj = j;
            }
            else cell[i][j] = c[0]-'0';
        }
    }

    fin.close();

    Start(si, sj);

    ShowResult(outfile, fout);

    return 0;
}
