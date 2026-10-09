#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Sorter {
public:
    void mergeSort(int arr[], int l, int r) {
        if (l < r) {
            int m = l + (r - l) / 2;
            mergeSort(arr, l, m);
            mergeSort(arr, m + 1, r);
            merge(arr, l, m, r);
        }
    }



    // Standard Quick Sort (pivot = last element)
    void quickSort(int arr[], int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

    // Quick Sort with Median-of-Three pivot
    void quickSortM3(int arr[], int low, int high) {
        if (low < high) {
            int pi = partitionM3(arr, low, high);
            quickSortM3(arr, low, pi - 1);
            quickSortM3(arr, pi + 1, high);
        }
    }


    // Instrumenting versions for empirical comparison analysis
    void quickSortCounted(int arr[], int low, int high, long long& counter) {
        if (low < high) {
            int pi = partitionCounted(arr, low, high, counter);
            quickSortCounted(arr, low, pi - 1, counter);
            quickSortCounted(arr, pi + 1, high, counter);
        }
    }

    void quickSortM3Counted(int arr[], int low, int high, long long& counter) {
        if (low < high) {
            int pi = partitionM3Counted(arr, low, high, counter);
            quickSortM3Counted(arr, low, pi - 1, counter);
            quickSortM3Counted(arr, pi + 1, high, counter);
        }
    }

private:
    void merge(int arr[], int l, int m, int r) {
        int n1 = m - l + 1;
        int n2 = r - m;
        vector<int> L(n1), R(n2);

        for (int i = 0; i < n1; i++) L[i] = arr[l + i];
        for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
    }

    int partition(int arr[], int low, int high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    int partitionCounted(int arr[], int low, int high, long long& counter) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            counter++;
            if (arr[j] <= pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    // Median of arr[low], arr[mid], arr[high] placed at arr[high]
    int medianOfThree(int arr[], int low, int high) {
        int mid = low + (high - low) / 2;
        if (arr[low] > arr[mid]) swap(arr[low], arr[mid]);
        if (arr[mid] > arr[high]) swap(arr[mid], arr[high]);
        if (arr[low] > arr[mid]) swap(arr[low], arr[mid]);
        // arr[mid] is now the median; swap it to high to serve as pivot
        swap(arr[mid], arr[high]);
        return high;
    }

    int medianOfThreeCounted(int arr[], int low, int high, long long& counter) {
        int mid = low + (high - low) / 2;
        counter++;
        if (arr[low] > arr[mid]) swap(arr[low], arr[mid]);
        counter++;
        if (arr[mid] > arr[high]) swap(arr[mid], arr[high]);
        counter++;
        if (arr[low] > arr[mid]) swap(arr[low], arr[mid]);
        swap(arr[mid], arr[high]);
        return high;
    }

    int partitionM3(int arr[], int low, int high) {
        medianOfThree(arr, low, high);
        return partition(arr, low, high);
    }

    int partitionM3Counted(int arr[], int low, int high, long long& counter) {
        medianOfThreeCounted(arr, low, high, counter);
        return partitionCounted(arr, low, high, counter);
    }
};

int main() {
    Sorter s;
    int tmp[8];
    int a[] = {64, 25, 12, 22, 11, 90, 45, 34};
    int n = 8;

    // Driver tests
    copy(a, a + n, tmp); s.mergeSort(tmp, 0, n - 1);
    cout << "MS: "; for (int x : tmp) cout << x << " "; cout << endl;

    copy(a, a + n, tmp); s.quickSort(tmp, 0, n - 1);
    cout << "QS: "; for (int x : tmp) cout << x << " "; cout << endl;

    int sorted[] = {1, 2, 3, 4, 5, 6, 7, 8};
    copy(sorted, sorted + n, tmp); s.quickSort(tmp, 0, n - 1);
    cout << "QS sorted: "; for (int x : tmp) cout << x << " "; cout << endl;

    copy(sorted, sorted + n, tmp); s.quickSortM3(tmp, 0, n - 1);
    cout << "QSM3 sorted: "; for (int x : tmp) cout << x << " "; cout << endl;

    // Section 3: Empirical analysis on n = 16
    int n16 = 16;
    int rand16[16] = {9, 3, 14, 1, 8, 12, 2, 15, 7, 10, 6, 16, 5, 11, 4, 13};
    int asc16[16]  = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int desc16[16] = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    long long qsCount = 0, qsm3Count = 0;
    int testArr[16];

    cout << "\n--- Section 3 Empirical Verification (n = 16) ---\n";

    // Random
    qsCount = 0; copy(rand16, rand16 + n16, testArr); s.quickSortCounted(testArr, 0, n16 - 1, qsCount);
    qsm3Count = 0; copy(rand16, rand16 + n16, testArr); s.quickSortM3Counted(testArr, 0, n16 - 1, qsm3Count);
    cout << "Random: QS = " << qsCount << ", QSM3 = " << qsm3Count << endl;

    // Sorted
    qsCount = 0; copy(asc16, asc16 + n16, testArr); s.quickSortCounted(testArr, 0, n16 - 1, qsCount);
    qsm3Count = 0; copy(asc16, asc16 + n16, testArr); s.quickSortM3Counted(testArr, 0, n16 - 1, qsm3Count);
    cout << "Sorted: QS = " << qsCount << ", QSM3 = " << qsm3Count << endl;

    // Reverse
    qsCount = 0; copy(desc16, desc16 + n16, testArr); s.quickSortCounted(testArr, 0, n16 - 1, qsCount);
    qsm3Count = 0; copy(desc16, desc16 + n16, testArr); s.quickSortM3Counted(testArr, 0, n16 - 1, qsm3Count);
    cout << "Reverse: QS = " << qsCount << ", QSM3 = " << qsm3Count << endl;

    return 0;
}