#include <iostream>
#include <fstream>
#define MAX 2147483646
#define DIM 1000
#define MIN -2147483646

using namespace std;

int main(void)
{
    long int matrix[DIM][DIM];
    
    for (int i = 0; i < DIM; ++ i) {
        for (int j = 0; j < DIM; ++ j) {
            if ((i+j) % 2 == 0) matrix[i][j] = MAX;
            else matrix[i][j] = MIN;
        }
    }

    ofstream outfile;
    
    outfile.open("./test/matrix.data");
    
    outfile << DIM << ' ' << DIM << '\n';
    for (int i = 0; i < DIM; ++ i) {
        for (int j = 0; j < DIM - 1; ++ j) {
            outfile << matrix[i][j] << ' ';
        }
        outfile << matrix[i][DIM - 1] << '\n';
    }
    
    outfile.close();
    
    return 0;
}

