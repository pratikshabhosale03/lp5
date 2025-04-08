#include <iostream> 
#include <vector> 
#include <queue> 
#include <set> 
#include <omp.h> 
#include <chrono> 
 
using namespace std; 
// Merge function 
void merge(int a[], int i1, int j1, int i2, int j2) 
{ 
    int temp[1000]; 
    int i = i1, j = i2, k = 0; 
    while (i <= j1 && j <= j2) 
    { 
        temp[k++] = (a[i] < a[j]) ? a[i++] : a[j++]; 
    } 
    while (i <= j1) 
        temp[k++] = a[i++]; 
    while (j <= j2) 
        temp[k++] = a[j++]; 
    for (i = i1, j = 0; i <= j2; i++, j++) 
    { 
        a[i] = temp[j]; 
    } 
} 
// Mergesort function 
void mergesort(int a[], int i, int j) 
{ 
    if (i < j) 
    { 
        int mid = (i + j) / 2; 
#pragma omp parallel sections 
        { 
#pragma omp section 
            { 
                mergesort(a, i, mid); 
            } 
#pragma omp section 
            { 
                mergesort(a, mid + 1, j); 
            } 
        } 
        merge(a, i, mid, mid + 1, j); 
    } 
} 
// Bubble Sort function 
void bubbleSort(int arr[], int n) 
{ 
    bool swapped; 
    for (int i = 0; i < n - 1; i++) 
    { 
        swapped = false; 
#pragma omp parallel for 
        for (int j = 0; j < n - i - 1; j++) 
        { 
            if (arr[j] > arr[j + 1]) 
            { 
                swap(arr[j], arr[j + 1]); 
                swapped = true; 
            } 
        } 
        if (!swapped) 
            break; 
    } 
} 
int main() 
{ 
    int n; 
    cout << "Enter number of elements: "; 
    cin >> n; 
    // Use dynamic allocation 
    int *arr = new int[n]; 
    cout << "Enter " << n << " elements: "; 
    for (int i = 0; i < n; i++) 
    { 
        cin >> arr[i]; 
    } 
    double start_time, end_time, seq_time, par_time; 
    while (true) 
    { 
        int choice; 
        cout << "\n1. Merge Sort\n2. Bubble Sort\n3. End\nEnter your choice: "; 
        cin >> choice; 
        if (choice == 3) 
            break; 
        else if (choice == 1) 
        { 
            start_time = omp_get_wtime(); 
            mergesort(arr, 0, n - 1); 
            end_time = omp_get_wtime(); 
            seq_time = end_time - start_time; 
            cout << "\nSequential Merge Sort Time: " << 
seq_time * 1e6 << "microseconds "<<endl;  
                start_time = omp_get_wtime(); 
#pragma omp parallel 
            { 
#pragma omp single 
                mergesort(arr, 0, n - 1); 
            } 
            end_time = omp_get_wtime(); 
            par_time = end_time - start_time; 
            cout << "Parallel Merge Sort Time: " << par_time * 
1e6 << " microseconds" << endl; 
        } 
        else if (choice == 2) 
        { 
            start_time = omp_get_wtime(); 
            bubbleSort(arr, n); 
            end_time = omp_get_wtime(); 
            seq_time = end_time - start_time; 
            cout << "\nSequential Bubble Sort Time: " << 
seq_time * 1e6 << " microseconds" 
                 << endl; 
            start_time = omp_get_wtime(); 
#pragma omp parallel 
            { 
#pragma omp single 
                bubbleSort(arr, n); 
            } 
            end_time = omp_get_wtime(); 
            par_time = end_time - start_time; 
            cout << "Parallel Bubble Sort Time: " << par_time 
* 1e6 << " microseconds" << endl; 
        } 
        else 
        { 
            cout << "Valid Option required!\n"; 
        } 
    } 
    delete[] arr; // Free memory 
    return 0; 
}


//command : g++ -fopenmp hpc2.cpp -o hpc2
//./hpc2
