#include <iostream>
#define DIM 1000

using namespace std;

int main(void) 
{
    char map[DIM][DIM];

    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            if (i % 2 == 0 && i != 0 && j != DIM/2)
                map[i][j] = '1';
            else
                map[i][j] = '0';
        }
    }
    
    map[0][DIM/2] = 'R';
    map[0][DIM/2-1] = map[0][DIM/2+1] = '1';

    cout << DIM << ' ' << DIM << ' ' << 3*DIM-2 << '\n';

    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            cout << map[i][j];
            cout << (j < DIM-1 ? ' ' : '\n');
        }
    }

    return 0;
}
