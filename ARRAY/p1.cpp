#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;

    int arr[n];

    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    int total = 0;
    for (int i = 0; i < n; i++)
    {
        total += arr[i];
    }

    cout << total << endl;
}