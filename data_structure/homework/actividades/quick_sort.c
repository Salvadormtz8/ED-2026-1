//Quick sort
#include <stdio.h>

void intercambiar(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Función para particionar el array
int particion(int arr[], int low, int high) {
    int pivot = arr[high]; // pivote
    int i = (low - 1);     // índice del elemento más pequeño
    int j;
    
    for (j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            intercambiar(&arr[i], &arr[j]);
        }
    }
    intercambiar(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Función principal de QuickSort
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = particion(arr, low, high);
        
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void imprimirArray(int arr[], int size) {
    int i; 
    for (i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int arr[] = {25, 7, 14, 48, 1, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("Array original: ");
    imprimirArray(arr, n);
    
    quickSort(arr, 0, n - 1);
    
    printf("Array ordenado: ");
    imprimirArray(arr, n);
    
    return 0;
}
