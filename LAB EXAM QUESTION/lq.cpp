#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <cmath>

using namespace std;

struct Student
{
    int studentID;
    string name;
    double courseScore;
    int completionTime;
    int modulesCompleted;
};

long long quickSortComparisons = 0;
long long binarySearchComparisons = 0;

// Returns true if a should come before b
// Course Score: descending
// Equal Course Score: Completion Time ascending
bool comesBefore(const Student &a, const Student &b)
{
    quickSortComparisons++;

    if (a.courseScore > b.courseScore)
        return true;

    if (a.courseScore < b.courseScore)
        return false;

    return a.completionTime < b.completionTime;
}

void swapStudents(Student &a, Student &b)
{
    Student temp = a;
    a = b;
    b = temp;
}

// Quick Sort using the middle element as pivot
void quickSort(Student arr[], int low, int high)
{
    if (low >= high)
        return;

    int i = low;
    int j = high;

    int mid = low + (high - low) / 2;
    Student pivot = arr[mid];

    while (i <= j)
    {
        while (i <= high && comesBefore(arr[i], pivot))
            i++;

        while (j >= low && comesBefore(pivot, arr[j]))
            j--;

        if (i <= j)
        {
            swapStudents(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    if (low < j)
        quickSort(arr, low, j);

    if (i < high)
        quickSort(arr, i, high);
}

// Recursive Binary Search
// Array is sorted in descending order
int binarySearch(Student arr[], int low, int high, double target)
{
    if (low > high)
        return -1;

    int mid = low + (high - low) / 2;

    binarySearchComparisons++;

    if (arr[mid].courseScore == target)
        return mid;

    if (target > arr[mid].courseScore)
    {
        // Target is on the left side
        return binarySearch(arr, low, mid - 1, target);
    }
    else
    {
        // Target is on the right side
        return binarySearch(arr, mid + 1, high, target);
    }
}

// Find first and last occurrence starting from the index
// returned by Binary Search.
void findFirstLast(
    Student arr[],
    int n,
    double target,
    int foundIndex,
    int &firstOccurrence,
    int &lastOccurrence)
{
    firstOccurrence = foundIndex;
    lastOccurrence = foundIndex;

    // Move left until a different score is found
    while (firstOccurrence > 0 &&
           arr[firstOccurrence - 1].courseScore == target)
    {
        firstOccurrence--;
    }

    // Move right until a different score is found
    while (lastOccurrence < n - 1 &&
           arr[lastOccurrence + 1].courseScore == target)
    {
        lastOccurrence++;
    }
}

// Display all students having the target score
void displayMatchingStudents(
    Student arr[],
    int firstOccurrence,
    int lastOccurrence)
{
    for (int i = firstOccurrence; i <= lastOccurrence; i++)
    {
        cout << left
             << setw(12) << arr[i].studentID
             << setw(20) << arr[i].name
             << setw(15) << fixed << setprecision(2)
             << arr[i].courseScore
             << setw(20) << arr[i].completionTime
             << setw(18) << arr[i].modulesCompleted
             << endl;
    }
}

// Find closest higher and lower scores
// by traversing the sequence and comparing differences
void findClosestScores(
    Student arr[],
    int n,
    double target,
    double &closestHigher,
    double &closestLower,
    bool &higherFound,
    bool &lowerFound)
{
    higherFound = false;
    lowerFound = false;

    double higherDifference = 0;
    double lowerDifference = 0;

    for (int i = 0; i < n; i++)
    {
        double difference = fabs(arr[i].courseScore - target);

        if (arr[i].courseScore > target)
        {
            if (!higherFound || difference < higherDifference)
            {
                closestHigher = arr[i].courseScore;
                higherDifference = difference;
                higherFound = true;
            }
        }
        else if (arr[i].courseScore < target)
        {
            if (!lowerFound || difference < lowerDifference)
            {
                closestLower = arr[i].courseScore;
                lowerDifference = difference;
                lowerFound = true;
            }
        }
    }
}

void displayStudents(Student arr[], int n)
{
    cout << left
         << setw(12) << "Student ID"
         << setw(20) << "Name"
         << setw(15) << "Score"
         << setw(20) << "Completion Time"
         << setw(18) << "Modules"
         << endl;

    cout << string(85, '-') << endl;

    for (int i = 0; i < n; i++)
    {
        cout << left
             << setw(12) << arr[i].studentID
             << setw(20) << arr[i].name
             << setw(15) << fixed << setprecision(2)
             << arr[i].courseScore
             << setw(20) << arr[i].completionTime
             << setw(18) << arr[i].modulesCompleted
             << endl;
    }
}

int main()
{
    int n;

    cout << "Enter number of students: ";
    cin >> n;

    if (n < 1 || n > 10000)
    {
        cout << "Invalid number of students. Enter N between 1 and 10000." << endl;
        return 0;
    }

    Student *students = new Student[n];

    cout << "\nEnter student details:\n";
    cout << "Student ID Name CourseScore CompletionTime ModulesCompleted\n";

    for (int i = 0; i < n; i++)
    {
        cin >> students[i].studentID >> students[i].name >> students[i].courseScore >> students[i].completionTime >> students[i].modulesCompleted;
    }

    // Quick Sort
    quickSortComparisons = 0;

    auto sortStart = chrono::high_resolution_clock::now();

    quickSort(students, 0, n - 1);

    auto sortEnd = chrono::high_resolution_clock::now();

    auto sortTime = chrono::duration_cast<chrono::microseconds>(
                        sortEnd - sortStart)
                        .count();

    cout << "\nSorted Records:\n";
    displayStudents(students, n);

    cout << "\nQuick Sort Comparisons: "
         << quickSortComparisons << endl;

    cout << "Quick Sort Execution Time: "
         << sortTime << " microseconds" << endl;

    // Target score
    double target;

    cout << "\nEnter target Course Score: ";
    cin >> target;

    // Binary Search
    binarySearchComparisons = 0;

    auto searchStart = chrono::high_resolution_clock::now();

    int foundIndex = binarySearch(students, 0, n - 1, target);

    auto searchEnd = chrono::high_resolution_clock::now();

    auto searchTime = chrono::duration_cast<chrono::microseconds>(
                          searchEnd - searchStart)
                          .count();

    if (foundIndex != -1)
    {
        cout << "\nTarget score found." << endl;

        int firstOccurrence;
        int lastOccurrence;

        // Start from the index found by Binary Search and
        // move left and right to find the boundaries.
        findFirstLast(
            students,
            n,
            target,
            foundIndex,
            firstOccurrence,
            lastOccurrence);

        cout << "First occurrence: "
             << firstOccurrence << endl;

        cout << "Last occurrence: "
             << lastOccurrence << endl;

        cout << "\nStudents having target score:\n";

        cout << left
             << setw(12) << "Student ID"
             << setw(20) << "Name"
             << setw(15) << "Score"
             << setw(20) << "Completion Time"
             << setw(18) << "Modules"
             << endl;

        cout << string(85, '-') << endl;

        displayMatchingStudents(
            students,
            firstOccurrence,
            lastOccurrence);

        // Rank = first occurrence index + 1
        int rank = firstOccurrence + 1;

        cout << "\nRank: "
             << rank << endl;
    }
    else
    {
        cout << "\nTarget score not found." << endl;

        double closestHigher = 0;
        double closestLower = 0;

        bool higherFound;
        bool lowerFound;

        findClosestScores(
            students,
            n,
            target,
            closestHigher,
            closestLower,
            higherFound,
            lowerFound);

        cout << "\nClosest scores:\n";

        if (higherFound)
        {
            cout << "Closest higher score: "
                 << fixed << setprecision(2)
                 << closestHigher << endl;
        }
        else
        {
            cout << "No higher score exists." << endl;
        }

        if (lowerFound)
        {
            cout << "Closest lower score: "
                 << fixed << setprecision(2)
                 << closestLower << endl;
        }
        else
        {
            cout << "No lower score exists." << endl;
        }
    }

    cout << "\nBinary Search Comparisons: "
         << binarySearchComparisons << endl;

    cout << "Binary Search Execution Time: "
         << searchTime << " microseconds" << endl;

    delete[] students;

    return 0;
}
