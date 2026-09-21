#include <iostream>
using namespace std;

int linearSearch(int arr[], int n, int key)
{
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == key)
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    int arr[] = {1, 2, 3, 4, 5};
    int size = 5;
    int key = 3;

    int result = linearSearch(arr, size, key);

    if (result == -1)
    {
        cout << "Element not found in the array." << endl;
    }
    else
    {
        cout << "Element found at index: " << result << endl;
    }

    return 0;
}