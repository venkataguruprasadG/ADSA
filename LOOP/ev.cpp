#include <iostream>
using namespace std;

int main()
{
    int result = 0;

    for (int i = 2; i <= 20; i += 2)
    {
        result += i;
    }
    cout << result << endl;
    return 0;
}