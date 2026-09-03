#include <vector>
#include <iostream>
using namespace std;
bool searchMatrix(const vector<vector<int>> &grid, int target)
{
    int m = grid.size();
    int n = grid[0].size();

    int row = 0;
    int col = n - 1;

    while (row < m && col >= 0)
    {
        if (grid[row][col] == target)
        {
            return true;
        }
        else if (grid[row][col] > target)
        {
            col--;
        }
        else
        {
            row++;
        }
    }
    return false;
}

int main()
{
    vector<vector<int>> grid = {
        {1, 4, 7, 11, 15},
        {2, 5, 8, 12, 19},
        {3, 6, 9, 16, 22},
        {10, 13, 14, 17, 24},
        {18, 21, 23, 26, 30}};

    int target = 5;

    cout << searchMatrix(grid, target) << endl;
}