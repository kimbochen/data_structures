#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

#define vcstr vector<string>
#define pii pair<int,int>

using namespace std;

struct FloorData
{
    void Read(string folder)
    {
        tcin.open("./" + folder + "/floor.data");

        if (!tcin.is_open()) {
            cout << "Unable to read \"floor.data\"" << '\n';
            exit(1);
        }
        else {
            tcin >> m >> n >> charge;

            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    tcin >> cell[i][j];
                    if (cell[i][j] == "R")
                        start = make_pair(i, j);
                }
            }
        }

        tcin.close();
    }

    void ShowFloor(void)
    {
        for (auto& v : cell) {
            for (auto& c : v) {
                cout << c << "\n "[c != v.back()];
            }
        }
    }

    vector <vcstr> cell;
    int m, n, charge;
    ifstream tcin;
    pii start;
};

struct PathData
{
    //void Read(string folder)
    //{
    //    ptin.open("./" + folder + "/final.path");

    //    if (!ptin.is_open()) {
    //        cout << "Unable to read \"final.path\"" << '\n';
    //        exit(1);
    //    }
    //    else {
    //        ptin >> num;

    //        while (ptin >> pos.x >> pos.y) {
    //            step.emplace_back(pos.x, pos.y);
    //        }
    //    }

    //    ptin.close();
    //}

    ifstream ptin;
    vector <pii> step; 
    int num;
};


int main(int argc, char* argv[])
{ 
    FloorData myFloor;
    PathData myPath;

    if (argc < 3) {
        cout << "Invalid number of arguments" << '\n';
        exit(1);
    }
    else {
        myFloor.Read(string(argv[1]));
        //myPath.Read(string(argv[2]));
    }

    return 0;
}
