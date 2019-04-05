#include <iostream>

#define MAX 4

int main(void)
{
    char arr[MAX][MAX];

    for (int i = 0; i < MAX; ++i)
        for (int j = 0; j < MAX; ++j)
            arr[i][j] = '0';

    arr[0][0] = 'R';

    std::cout << MAX << ' ' << MAX 
        << ' ' << 2*MAX-2 << '\n';

    for (int i = 0; i < MAX; ++i)
        for (int j = 0; j < MAX; ++j)
            std::cout << arr[i][j] << "\n "[j < MAX-1];

    return 0;
}
