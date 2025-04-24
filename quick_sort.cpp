//#include <bits/stdc++.h>
//using namespace std;
#include <vector>

int partition(std::vector<std::pair<int, double>>& arr, int low, int high, bool (*comp)(std::pair<int, double>, std::pair<int, double>)) {

    // Choose the pivot
    std::pair<int, double> pivot = arr[high];

    // Index of smaller element and indicates 
    // the right position of pivot found so far
    int i = low - 1;

    // Traverse arr[low..high] and move all smaller
    // elements on left side. Elements from low to 
    // i are smaller after every iteration
    for (int j = low; j <= high - 1; j++) {
        if (comp(arr[j], pivot)) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    // Move pivot after smaller elements and
    // return its position
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// The QuickSort function implementation
void quickSort(std::vector<std::pair<int, double>>& arr, int low, int high, bool (*comp)(std::pair<int, double>, std::pair<int, double>)) {

    if (low < high) {

        // pi is the partition return index of pivot
        int pi = partition(arr, low, high, comp);

        // Recursion calls for smaller elements
        // and greater or equals elements
        quickSort(arr, low, pi - 1, comp);
        quickSort(arr, pi + 1, high, comp);
    }
}
