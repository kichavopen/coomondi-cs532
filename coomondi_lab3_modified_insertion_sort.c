#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  strings function is read here*/
char** readStrings(int n) {
    char** arr = malloc(n * sizeof(char*));
    char buffer[1000];
    int i;
    
    for (i = 0; i < n; i++) {
        printf("Enter string %d: ", i + 1);
        scanf("%s", buffer);
        arr[i] = malloc(strlen(buffer) + 1);
        strcpy(arr[i], buffer);
    }
    return arr;
}

/* Sorting of string function is done here */

/* Assisted with the help of Venice.ai */

void sortStrings(char** arr, int n) {
    int i, j;
    char* key;
    
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && strcmp(arr[j], key) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/* We display the string function here */
void displayStrings(char** arr, int n) {
    int i;
    printf("[");
    for (i = 0; i < n - 1; i++) printf("%s, ", arr[i]);
    if (n > 0) printf("%s]\n", arr[n - 1]);
    else printf("]\n");
}

/* Main function */
int main() {
    int n, i;
    char** arr;
    
    printf("Enter number of strings: ");
    scanf("%d", &n);
    
    arr = readStrings(n);
    
    printf("Given array is: ");
    displayStrings(arr, n);
    
    sortStrings(arr, n);
    
    printf("Sorted array is: ");
    displayStrings(arr, n);
    
    for (i = 0; i < n; i++) free(arr[i]);
    free(arr);
    
    return 0;
}
