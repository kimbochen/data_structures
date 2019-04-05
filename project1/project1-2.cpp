#include <iostream>
#include <fstream>
#include <algorithm>
#include <utility>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

enum STATE {GT, EQ, LS};

inline int my_cmp(int a, int b)
{
    if (a > b) return GT;
    else if (a == b) return EQ;
    else return LS;
}

int main(int argc, char *argv[])
{
    auto t1 = chrono::high_resolution_clock::now();
    string foldername(argv[1]);
    int m, n, k, num = 0;
    int *up, *md, *dw;
    int lr_st, lr_nxst;
    int *ud_st, *ud_nxst;
    vector<pair<int, int>> ans;
    ios_base::sync_with_stdio(false);
    ifstream infile("./" + foldername + "/matrix.data");
    ofstream outfile("./" + foldername + "/final2.peak", ios::out);
    
    infile >> m >> n;
    up = new int[n];
    dw = new int[n];
    ud_nxst = new int[n];
    
    for (int i = 0; i < n; ++ i) infile >> up[i];
    for (int i = 0; i < n; ++ i) infile >> dw[i];
    
    /* tackle (0,0) */
    lr_nxst = my_cmp(up[0], up[1]);
    ud_nxst[0] = my_cmp(up[0], dw[0]);
    if (lr_nxst != LS && ud_nxst[0] != LS) {
        pair<int, int> p(0, 0);
        ans.emplace_back(p);
        num ++;
    }
    lr_st = lr_nxst;
    
    /* tackle (0, i) */
    for (int i = 1; i < n-1; ++ i) {
        lr_nxst = my_cmp(up[i], up[i+1]);
        ud_nxst[i] = my_cmp(up[i], dw[i]);
        if (lr_st != GT && lr_nxst != LS && ud_nxst[i] != LS) {
            pair<int, int> p(0, i);
            ans.emplace_back(p);
            num ++;
        }
        lr_st = lr_nxst;
    }
    
    /* tackle (0, n-1) */
    ud_nxst[n-1] = my_cmp(up[n-1], dw[n-1]);
    if (lr_st != GT && ud_nxst[n-1] != LS) {
        pair<int, int> p(0, n-1);
        ans.emplace_back(p);
        num ++;
    }
    
    swap(ud_st, ud_nxst);
    swap(md, dw);
    
    /* tackle (i, j)*/
    for (k = 1, dw = new int[n], ud_nxst = new int[n]; k < m-1; ++ k) {
        for (int i = 0; i < n; ++ i) infile >> dw[i];
        
        /* tackle (i, 0) */
        lr_nxst = my_cmp(md[0], md[1]);
        ud_nxst[0] = my_cmp(md[0], dw[0]);
        if (lr_nxst != LS && ud_st[0] != GT && ud_nxst[0] != LS) {
            pair<int, int> p(k, 0);
            ans.emplace_back(p);
            num ++;
        }
        lr_st = lr_nxst;
        
        /* tackle (i, j) */
        for (int i = 1; i < n-1; ++ i) {
            lr_nxst = my_cmp(md[i], md[i+1]);
            ud_nxst[i] = my_cmp(md[i], dw[i]);
            if (lr_st != GT && lr_nxst != LS &&
                ud_st[i] != GT && ud_nxst[i] != LS) {
                pair<int, int> p(k, i);
                ans.emplace_back(p);
                num ++;
            }
            lr_st = lr_nxst;
        }
        
        /* tackle (i, n-1) */
        ud_nxst[n-1] = my_cmp(md[n-1], dw[n-1]);
        if (lr_st != GT && ud_st[n-1] != GT && ud_nxst[n-1] != LS) {
            pair<int, int> p(k, n-1);
            ans.emplace_back(p);
            num ++;
        }
        
        swap(ud_st, ud_nxst);
        swap(up, md);
        swap(md, dw);
    }
    
    /* tackle (m-1, 0) */
    lr_nxst = my_cmp(md[0], md[1]);
    if (lr_nxst != LS && ud_st[0] != GT) {
        pair<int, int> p(m-1, 0);
        ans.emplace_back(p);
        num ++;
    }
    lr_st = lr_nxst;
    
    /* tackle (m-1, j) */
    for (int i = 1; i < n-1; ++ i) {
        lr_nxst = my_cmp(md[i], md[i+1]);
        if (lr_st != GT && lr_nxst != LS && ud_st[i] != GT) {
            pair<int, int> p(m-1, i);
            ans.emplace_back(p);
            num ++;
        }
        lr_st = lr_nxst;
    }
    
    /* tackle (m-1, n-1) */
    if (lr_st != GT && ud_st[n-1] != GT) {
        pair<int, int> p(m-1, n-1);
        ans.emplace_back(p);
        num ++;
    }
    
    outfile << num << '\n';
    for (auto p : ans) {
        outfile << p.first+1<< ' ' << p.second+1 << '\n';
    }
    
    outfile.close();
    
    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double,milli> elapsed = t2 - t1;
    cout << "Algorithm runtime is: " << elapsed.count() << " miliseconds." << '\n';
    
    return 0;
}

