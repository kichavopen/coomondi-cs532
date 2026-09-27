#include <stdio.h>
#include <stdlib.h>

int main() {
    int N;
    int *arr;
    int i;
    int temp, currLoc;
    
    /* We get the number of elements at this point */
    printf("Please enter number of elements in array: ");
    scanf("%d", &N);
    
    /* We allocate the memory for the array */
    arr = (int *)malloc(N * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    
    /* Array elements read */
    for (i = 0; i < N; i++) {
        printf("Please enter element %d of array: ", i + 1);
        scanf("%d", &arr[i]);
    }
    
    /* Display original array */
    printf("Given array is: ");
    printf("[");
    for (i = 0; i < N - 1; i++) {
        printf("%d, ", arr[i]);
    }
    if (N > 0) {
        printf("%d]\n", arr[N - 1]);
    } else {
        printf("]\n");
    }
    
    /* Naive InsertionSort with swaps */
    for (i = 1; i < N; i++) {
        currLoc = i;
        while (currLoc > 0 && arr[currLoc - 1] > arr[currLoc]) {
            temp = arr[currLoc];
            arr[currLoc] = arr[currLoc - 1];
            arr[currLoc - 1] = temp;
            currLoc--;
        }
    }
    
    /* Display sorted array */
    printf("Sorted array is: ");
    printf("[");
    for (i = 0; i < N - 1; i++) {
        printf("%d, ", arr[i]);
    }
    if (N > 0) {
        printf("%d]\n", arr[N - 1]);
    } else {
        printf("]\n");
    }
    
    /* We have to free the alocated memory */
    free(arr);
    
    return 0;
}
