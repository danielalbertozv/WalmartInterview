// -------------------------------------------------------------- -*- C++ -*-
// File: Sorting.cpp
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 09/18/2021
// Version 1.0.0
//
// C++ Version: Sorting Examples
// --------------------------------------------------------------------------

// Libraries
#include <cstdlib>		// General purpose function, abs(), ...
#include <ilcplex/ilocplex.h>	// CPLEX library
#include <string>		// String manipulation
#include <list>
#include <vector>
#include <stack>

ILOSTLBEGIN // IBM macro to allow run the application in c++ with STL (Standard Template Library)

// Creates AuthorDetials method, (void: no value returned)
void AuthorDetails() {
	cout << "--------------------------------------------------------" << endl;
	cout << "File: Sorting.cpp" << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << "Developed by Daniel Zuniga" << endl;
	cout << "Date: 09/18/2021" << endl;
	cout << "Version 1.0.0\n" << endl;
	cout << "C++ Version: Sorting Examples" << endl;
	cout << "--------------------------------------------------------\n" << endl;

} // End AuthorDetails

#pragma region Helper Functions

// Number of numbers to sort
#define N 9

/// <summary>
///  Prints an integer array
/// </summary>
/// <param name="unsArray"> Unsorted array </param>
/// <param name="n"> Size of array </param>
void printArray(int unsArray[], int n)
{
    for (int i = 0; i < n; i++) {
        cout << unsArray[i] << " ";
    }cout << endl;
}

#pragma endregion

#pragma region Quick Sort Algorithm

/// <summary>
///  Lomuto partition scheme: Last number of array is pivot and partitions (left and right) according to it
/// </summary>
/// <param name="unsArray"> Unsorted array </param>
/// <param name="first"> First numer of array size (e.g., 0)</param>
/// <param name="last"> Last number of array size (e.g., n)</param>
int partitionLomuto(int unsArray[], int first, int last)
{
    // Define pivot
    int pivotNumber = unsArray[last];

    // Define partition index
    int partitionIndex = first;

    // Loop first,...,last-1
    for (int i = first; i < last; i++)
    {
        // If number unsArray[i] is less than pivot number, it gets swapped and partition index++ 
        if (unsArray[i] <= pivotNumber)
        {
            swap(unsArray[i], unsArray[partitionIndex]);
            partitionIndex++;
        }
    }

    // Swaps partitionIndex with the pivotNumber (Last)
    swap(unsArray[partitionIndex], unsArray[last]);

    return partitionIndex;
}

/// <summary>
///  Quick sort algorithm
/// </summary>
/// <param name="unsArray"> Unsorted array </param>
/// <param name="first"> First numer of array size (e.g., 0)</param>
/// <param name="last"> Last number of array size (e.g., n)</param>
void quickSortAlgorithm(int unsArray[], int first, int last)
{
    // Stopping criteria
    if (first >= last) {
        return;
    }

    // Applies Lomuto partition
    int pivotIndex = partitionLomuto(unsArray, first, last);

    // Recurrent call for left side partition 
    quickSortAlgorithm(unsArray, first, pivotIndex - 1);

    // Recurrent call for right side partition 
    quickSortAlgorithm(unsArray, pivotIndex + 1, last);
}

#pragma endregion

#pragma region Merge Sort Algorithm (Recursive)

/// <summary>
///  Merges two arrays
/// </summary>
/// <param name="unsArray"> Unsorted array </param>
/// <param name="auxsArray"> Auxiliary array </param>
/// <param name="first"> First cell position of splitted array based on original   </param>
/// <param name="middle"> Middle cell position of previous array splitting based on original  </param>
/// <param name="last"> Last cell position of splitted array based on original </param>
void mergeSort(int unsArray[], int auxArray[], int first, int middle, int last)
{
    //Auxiliary
    int k = first, i = first, j = middle + 1;

    // Compares value of each cell position in both halfs and sorts them in the auxArray
    // Due to recursion, sorting will start with the smallest size arrays, each with one cell
    while (i <= middle && j <= last)
    {
        if (unsArray[i] <= unsArray[j]) {
            auxArray[k++] = unsArray[i++];
        }
        else {
            auxArray[k++] = unsArray[j++];
        }
    }

    // Copy remaining unsorted elements
    while (i <= middle) {
        auxArray[k++] = unsArray[i++];
    }

    // Copy sorted array in original
    for (int i = first; i <= last; i++) {
        unsArray[i] = auxArray[i];
    }
}

/// <summary>
///  Splits and merges the array recursively
/// </summary>
/// <param name="unsArray"> Unsorted array </param>
/// /// <param name="auxsArray"> Auxiliary array </param>
/// /// /// <param name="first"> First cell position of splitted array based on original   </param>
/// /// /// <param name="last"> Last cell position of splitted array based on original </param>
void splitMergeSort(int unsArray[], int auxArray[], int first, int last)
{
    // End recursion for array size 1
    if (last == first) {
        return;
    }

    // Identify array middle size
    int middle = (first + ((last - first) >> 1));

    splitMergeSort(unsArray, auxArray, first, middle);        // Splits and merges first half
    splitMergeSort(unsArray, auxArray, middle + 1, last);     // Splits and merges last half

    mergeSort(unsArray, auxArray, first, middle, last);        // Merges the two half
}


/// <summary>
///  Merge sort algorithm (Recursive)
/// </summary>
/// <param name="unsArray"> Unsorted array </param>
void mergeSortAlgorithm(int unsArray[], int n)
{

    int auxArray[N];

    // Copy auxiliary array
    for (int i = 0; i < n; i++) {
        auxArray[i] = unsArray[i];
    }

    // Recursive merge function 
    splitMergeSort(unsArray, auxArray, 0, n - 1);

}
#pragma endregion

#pragma region Bubble Sort Algorithm (Recursive)

/// <summary>
///  Bubble sort algorithm (Recursive)
/// </summary>
/// <param name="unsArray"> Unsorted array </param>
/// <param name="n"> Size of array </param>
void bubbleSortAlgorithm(int unsArray[], int n)
{
    // Loop 0,...,n-1
    for (int i = 0; i < n - 1; i++)
    {
        // Swap positions if two continous values are unsorted
        if (unsArray[i] > unsArray[i + 1]) {
            swap(unsArray[i], unsArray[i + 1]);
        }
    }
    // Recursive call with n-1
    if (n - 1 > 1) {
        bubbleSortAlgorithm(unsArray, n - 1);
    }
}
#pragma endregion

#pragma region Selection Sort Algrithm

/// <summary>
///  Selection sort algorithm
/// </summary>
/// <param name="unsArray"> Unsorted array </param>
/// <param name="n"> Size of array </param>
void selectionSortAlgorithm(int unsArray[], int n)
{
    // 1st Loop 0,...,n-1
    for (int i = 0; i < n - 1; i++)
    { 
        // Identify minimum number from unsArray[i,...,n-1]
        int minNumber = i;

        // 2nd Loop i+1,...,n-1
        for (int j = i + 1; j < n; j++)
        {
            // Identify index of the minimum from 2nd Loop
            if (unsArray[j] < unsArray[minNumber]) {
                minNumber = j;
            }
        }

        // Swap two numbers of an array based on the provided indexes
        swap(unsArray[minNumber], unsArray[i]);
    }
}

#pragma endregion

#pragma region Insertion Sort Algorithm

/// <summary>
///  Insertion sort algorithm
/// </summary>
/// <param name="unsArray"> Unsorted array </param>
/// <param name="n"> Size of array </param>
void insertionSortAlgorithm(int unsArray[], int n)
{
    // Loop 1,...,n
    for (int i = 1; i < n; i++)
    {
        // Save the ith unsorted number and set the j index
        int number = unsArray[i];
        int j = i;

        // Identify the position the number belongs to in the sorted array
        while (j > 0 && unsArray[j - 1] > number)
        {
            unsArray[j] = unsArray[j - 1];
            j--;
        }

        // Update number to the identified position
        unsArray[j] = number;
    }
}
#pragma endregion 

int main() {

    AuthorDetails();	// calls AuthorDetails method/function 

    // Initialize random seed
    srand(time(NULL));

    int unsortedArray[N], sortedArray[N];
    int n = sizeof(unsortedArray) / sizeof(unsortedArray[0]);

    // generate random input of integers
    for (int i = 0; i < N; i++) {
        sortedArray[i] = unsortedArray[i] = (rand() % 100) - 50;
    }

    vector<int> sortedVector(sortedArray, sortedArray + n);

    cout << endl;
    cout << "------------------ Sorting Algorithm Examples ---------------------------" << endl;
    cout << " The following random generated integer array (-50 to 50) is used as an example" << endl;
    printArray(sortedArray, n);
    cout << endl;

    cout << " - Quick sort algorithm" << endl;
    quickSortAlgorithm(sortedArray, 0, n-1);
    printArray(sortedArray, n);
    copy(begin(unsortedArray), end(unsortedArray), begin(sortedArray));
    cout << endl;

    cout << " - Merge sort algorithm (recursive)" << endl;
    mergeSortAlgorithm(sortedArray, n);
    printArray(sortedArray, n);

    copy(begin(unsortedArray), end(unsortedArray), begin(sortedArray));
    cout << endl;

    cout << " - Bubble sort algorithm (recursive)" << endl;
    bubbleSortAlgorithm(sortedArray, n);
    printArray(sortedArray, n);
    copy(begin(unsortedArray), end(unsortedArray), begin(sortedArray));
    cout << endl;

    cout << " - Selection sort algorithm" << endl;
    selectionSortAlgorithm(sortedArray, n);
    printArray(sortedArray, n);
    copy(begin(unsortedArray), end(unsortedArray), begin(sortedArray));
    cout << endl;

    cout << " - Insertion sort algorithm" << endl;
    insertionSortAlgorithm(sortedArray, n);
    printArray(sortedArray, n);
    copy(begin(unsortedArray), end(unsortedArray), begin(sortedArray));
    cout << endl;

    cout << endl;

    return 0;
}
