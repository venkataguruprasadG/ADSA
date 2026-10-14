#include <iostream>
using namespace std;

int main()
{
    int arr2[5]; // creates an array of size 5.

    int arr[5] = {10, 20, 30, 40, 50}; // declaration of array and initialization of array and its values.

    int arr1[] = {10, 20}; // compiler automatically calculates its size based on the number of elements.

    cout << arr1[1] << endl; // accessing the element

    int n;
    cin >> n;

    int arr3[n];

    for (int i = 0; i < n; i++)
    {
        cin >> arr3[i]; // taking input from user and storing it in the array.
    }

    for (int i = 0; i < n; i++)
    {
        cout << arr3[i] << " "; // printintg the elements of the array.
    }
}