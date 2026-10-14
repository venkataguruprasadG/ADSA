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

    int zero = 0;
    int pos = 0;
    int neg = 0;

    for (int i = 0; i < n; i++)
    {
        if (arr[i] == 0)

        {
            zero++;
        }
        else if (arr[i] > 0)
        {
            pos++;
        }
        else if (arr[i] < 0)
        {
            neg++;
        }
    }
    cout << "Positive numbers count: " << pos << endl;
    cout << "Negative numbers count: " << neg << endl;
    cout << "Zeroes count: " << zero << endl;
}