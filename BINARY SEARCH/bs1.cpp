#include <iostream>
#include <vector>
using namespace std;

int numberfind(const vector<int> &arr, int target)
{
    int low = 0;
    int high = arr.size() - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (arr[mid] == target)
        {
            return mid;
        }
        else if (arr[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return -1;
}

int main()
{
    vector<int> arr = {1, 2, 2, 3, 3, 3, 4, 5, 5, 5};
    int target = 3;

    int result = numberfind(arr, target);
    cout << "Index of the number " << result << endl;
}