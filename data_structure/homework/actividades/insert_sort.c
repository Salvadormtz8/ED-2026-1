//insert sort
#include <stdio.h>

void insertionSort(int arr[], int n) {
    int i, j, key;
    
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void imprimirArray(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int arr[] = {9, 13, 4, 6, 32};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("Array original: ");
    imprimirArray(arr, n);
    
    insertionSort(arr, n);
    
    printf("Array ordenado: ");
    imprimirArray(arr, n);
    
    return 0;
}
