#include <iostream>
#include <vector>
using namespace std;

int linear_search(const vector<int> &arr, int target)
{
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i] == target)
        {
            return i;
        }
    }
    return -1; // target not found
}

int main()
{
    vector<int> arr = {1, 2, 3, 4, 5};
    int target = 3;

    int result = linear_search(arr, target);
    if (result != -1)
    {
        cout << "Index of the number " << result << endl;
    }
    else
    {
        cout << "Number not found in the array." << endl;
    }
}