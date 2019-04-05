#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <deque>

#define vcnode vector<Node>
#define pii pair<int,int>
//#define DEBUG 39

using namespace std;

struct Node
{
    int dist;
    bool visit;
    string type;
};

vector <pii> dir = {{1,0},{0,-1},{-1,0},{0,1}};
vector <vcnode> cell;
vector <pii> poslog;
stack <pii> toclean;
int m, n, charge;
pii start;

void WriteFile(string folder)
{
    ofstream fout("./" + folder + "/final.path"); 

    if (!fout.is_open()) {
        cout << "unable to write file\n";
        exit(1);
    }

    fout << poslog.size() << '\n';

    for (auto p : poslog)
        fout << p.first << ' ' << p.second << '\n';

    fout.close();
}

void ReadMap(string folder)
{
    ifstream fin("./" + folder + "/floor.data");
    
    if (!fin.is_open()) {
        cout << "unable to read file\n";
        exit(1);
    }

    fin >> m >> n >> charge;

    cell.resize(m, vcnode(n));

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            fin >> cell[i][j].type;

            cell[i][j].visit = false;
            
            if (cell[i][j].type == "R")
                start = make_pair(i, j);
        }
    }

    fin.close();
}

bool isBound(int i, int j)
{
    return (i >= 0 && i < m && j >= 0 && j < n);
}

bool isValid(int i, int j)
{
    if (isBound(i, j))
        return (cell[i][j].type == "0" && !cell[i][j].visit);
    else
        return false;
}

void GenPath(void)
{
    deque <pii> tmplog;
    bool rev = false;
    int cnt = 0;
    pii p;

    for (; !toclean.empty(); toclean.pop()) {
        p = toclean.top();

        if (cell[p.first][p.second].visit) continue;

        cnt ++;

        (rev ? tmplog.emplace_back(p) : tmplog.emplace_front(p));

        pii np = p;
        int x = np.first;
        int y = np.second;

        while (cell[x][y].type != "R") {
            bool found = false;

            for (auto d : dir) {
                int i = x + d.first;
                int j = y + d.second;

                if (isValid(i, j)) {
                    if (cell[x][y].dist - cell[i][j].dist == 1) {
                        cell[i][j].visit = true;
                        np = make_pair(i, j);
                        x = np.first;
                        y = np.second;
                        found = true;
                        break;
                    }
                }
            }

            if (!found) {
                for (auto d : dir) {
                    int i = x + d.first;
                    int j = y + d.second;

                    if (isBound(i, j)) {
                        if (cell[x][y].dist - cell[i][j].dist == 1) {
                            cell[i][j].visit = true;
                            np = make_pair(i, j);
                            x = np.first;
                            y = np.second;
                            break;
                        }
                    }
                }
            }

            (rev ? tmplog.emplace_back(np) : tmplog.emplace_front(np));
        }

        rev = !rev;
#ifdef DEBUG
        for (pii p = tmplog.front(); !tmplog.empty(); tmplog.pop_front()) {
            p = tmplog.front();
            cout << p.first << ' ' << p.second << '\n';
        }
#endif
        if (cnt % 2 == 0) {
            poslog.reserve(poslog.size() + tmplog.size());
            move(begin(tmplog), end(tmplog), back_inserter(poslog));
            tmplog.clear();
        }
    }

    if (!tmplog.empty()) {
        poslog.reserve(poslog.size() + 2 * tmplog.size());
        move(begin(tmplog), end(tmplog), back_inserter(poslog));
        move(rbegin(tmplog), rend(tmplog), back_inserter(poslog));
        tmplog.clear();
    }
}

void SetDistance(void)
{
    queue <pii> q;

    cell[start.first][start.second].dist = 0;
    cell[start.first][start.second].visit = true;

    for (q.emplace(start); !q.empty(); q.pop()) {
        pii p = q.front();
        int x = p.first;
        int y = p.second;

        toclean.emplace(p);

        for (auto d : dir) {
            int i = x + d.first;
            int j = y + d.second;

            if (isValid(i, j)) {
                cell[i][j].dist = cell[x][y].dist + 1;
                cell[i][j].visit = true;
                q.emplace(i, j);
            }
        }
    }

    for (auto& v : cell)
        for (auto& n : v)
            n.visit = false;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "invalid number of arguments\n";
        exit(1);
    }

    ReadMap(string(argv[1]));

    SetDistance();

    GenPath();

    WriteFile(string(argv[1]));

#ifdef DEBUG
    for (auto p : poslog)
        cout << p.first << ' ' << p.second << '\n';

    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            cout << cell[i][j].dist << "\n "[j < n-1];
#endif

    return 0;
}
