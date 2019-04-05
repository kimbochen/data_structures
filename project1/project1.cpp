#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <chrono>

using namespace std;

int main(int argc, char *argv[])
{
    auto t1 = chrono::high_resolution_clock::now();
    string foldername(argv[1]);
    int *up, *md, *dw;
    int m, n, k, num = 0;
    vector<pair<int, int>> ans;
    ios_base::sync_with_stdio(false);
    ifstream infile("./" + foldername + "/matrix.data"); 
    ofstream outfile("./" + foldername + "/final.peak", ios::out);
 
    infile >> m >> n;
    up = new int[n];
    for (int i = 0; i < n; ++ i) infile >> up[i];

    if (m > 1) {
        dw = new int[n];
        for (int i = 0; i < n; ++ i) infile >> dw[i];
        
        if (up[0] >= up[1] && up[0] >= dw[0]) {
            pair<int, int> p(0, 0);
            ans.emplace_back(p);
            num ++;
        }
           
        for (int i = 1; i < n-1; ++ i) { 
            if (up[i] >= up[i-1] && up[i] >= up[i+1] && up[i] >= dw[i]) {
                pair<int, int> p(0, i);
                ans.emplace_back(p);
                num ++;
            }    
        }
        if (up[n-1] >= up[n-2] && up[n-1] >= dw[n-1]) {
            pair<int, int> p(0, n-1);
            ans.emplace_back(p);
            num ++;
        } 
       
        md = dw;

        for (k = 1, dw = new int[n]; k < m-1; ++ k) {
            for (int i = 0; i < n; ++ i) infile >> dw[i];

            if (md[0] >= md[1] && md[0] >= up[0] && md[0] >= dw[0]) {
                pair<int, int> p(k, 0);
                ans.emplace_back(p);
                num ++;
            }

            for (int i = 1; i < n-1; ++ i) {
                if (md[i] >= md[i-1] && md[i] >= md[i+1] &&
                    md[i] >= up[i] && md[i] >= dw[i]) {
                    pair<int, int> p(k, i);
                    ans.emplace_back(p);
                    num ++;
                }
            }
            if (md[n-1] >= md[n-2] && md[n-1] >= up[n-1] && md[n-1] >= dw[n-1]) {
                pair<int, int> p(k, n-1);
                ans.emplace_back(p);
                num ++;
            }

            int *tmp = up;
            up = md;
            md = dw;
            dw = tmp;
        }

        if (md[0] >= md[1] && md[0] >= up[0]) {
            pair<int, int> p(m-1, 0);
            ans.emplace_back(p);
            num ++;
        }

        for (int i = 1; i < n-1; ++ i) {
            if (md[i] >= md[i-1] && md[i] >= md[i+1] && md[i] >= up[i]) {
                pair<int, int> p(m-1, i);
                ans.emplace_back(p);
                num ++;   
            }
        }
        if (md[n-1] >= md[n-2] && md[n-1] >= up[n-1]) {
            pair<int, int> p(m-1, n-1);
            ans.emplace_back(p);
            num ++; 
        }
    } else {
        if (up[0] >= up[1]) {
            pair<int, int> p(0, 0);
            ans.emplace_back(p);
            num ++;
        }
           
        for (int i = 1; i < n-1; ++ i) {
            if (up[i] >= up[i-1] && up[i] >= up[i+1]) {
                pair<int, int> p(0, i);
                ans.emplace_back(p);
                num ++;
            }    
        }
        if (up[n-1] >= up[n-2]) {
            pair<int, int> p(0, n-1);
            ans.emplace_back(p);
            num ++;
        }
    }

    outfile << num << '\n';
    for (auto p : ans) {
        outfile << p.first+1 << ' ' << p.second+1 << '\n';
    } 
    outfile.close();
    
    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double,milli> elapsed = t2 - t1;
    cout << "Algorithm runtime is: " << elapsed.count() << " miliseconds." << '\n';
    return 0;
}

