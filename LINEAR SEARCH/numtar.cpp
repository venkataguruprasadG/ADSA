#include <iostream>
using namespace std;

int numberRepeats(int arr[], int n, int key)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == key)
        {
            count++;
        }
    }
    return count;
}

int main()
{
    int numbers[] = {1, 2, 4, 5, 6};
    int size = 5;

    int key = 4;

    int result = numberRepeats(numbers, size, key);

    if (result == 0)
    {
        cout << "Element is not there in the array." << endl;
    }
    else
    {
        cout << "Element is repeated " << result << " times in the array." << endl;
    }
}