#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

/* Function 1: sumOfDigits - from sumdigits.c */
int sumOfDigits(int n) {
    int sum = 0;
    
    /* Here we start with the invalid inputs */
    if (n <= 0) {
        return -1;
    }
    
    /* We add each sum to the last digit */
    while (n > 0) {
        sum = sum + (n % 10);  /* Add last digit */
        n = n / 10;            /* Remove last digit */
    }
    
    return sum;
}

/* Function 2: MaxMinDiff - from MaxMinDiff.c */
int MaxMinDiff(int arr[], int size) {
    int max = arr[0];
    int min = arr[0];
    int i;
    
    for (i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    
    return max - min;
}

/* Function 3: replaceEvenWithZero - from replaceell.c */
int* replaceEvenWithZero(int arr[], int size) {
    int *newArr;
    int i;
    
    /* Start with allocating new array */
    newArr = (int *)malloc(size * sizeof(int));
    
    /* copy elements or replace even ones with 0 */
    for (i = 0; i < size; i++) {
        if (arr[i] % 2 == 0) {
            newArr[i] = 0;      /* Even number */
        } else {
            newArr[i] = arr[i]; /* Odd number */
        }
    }
    
    return newArr;
}

/* Function 4: perfectSquare - from perfsquare.c */
int perfectSquare(int n) {
    int root;
    
    /* Negative numbers do not have perfect sqaures so we remove them */
    if (n < 0) {
        return 0;
    }
    
    /* we calculate the square root and round down */
    root = (int)sqrt(n);
    
    /* Check if root * root equals n */
    if (root * root == n) {
        return 1;  /* True */
    } else {
        return 0;  /* False */
    }
}

/* Function 5: countVowels - from vowelscount.c */
int countVowels(char s[]) {
    int count = 0;
    int i = 0;
    char c;
    
    while (s[i] != '\0') {
        c = tolower(s[i]);  /* converts the words to all lowercase */
        
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
        
        i++;
    }
    
    return count;
}

/* Main function with menu */
int main() {
    int choice;
    int n, size, result, i;
    int *arr, *newArr;
    char s[100];
    
    do {
        printf("\n========== MENU ==========\n");
        printf("1. Sum of Digits\n");
        printf("2. Max Min Difference\n");
        printf("3. Replace Even With Zero\n");
        printf("4. Perfect Square Check\n");
        printf("5. Count Vowels\n");
        printf("0. Exit\n");
        printf("==========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                /* User gives us the number */
                printf("\n--- Sum of Digits ---\n");
                printf("Enter a positive integer: ");
                scanf("%d", &n);
                
                /* The function is called */
                result = sumOfDigits(n);
                
                /* Result displayed */
                printf("sumOfDigits(%d) = %d\n", n, result);
                break;
                
            case 2:
                /* Collect the size of array */
                printf("\n--- Max Min Difference ---\n");
                printf("Enter array size: ");
                scanf("%d", &size);
                
                /* Allocate memory */
                arr = (int *)malloc(size * sizeof(int));
                
                /* Collect the elements in array */
                for (i = 0; i < size; i++) {
                    printf("Enter element %d: ", i + 1);
                    scanf("%d", &arr[i]);
                }
                
                /* result is calculated and displayed */
                result = MaxMinDiff(arr, size);
                printf("Difference (max - min) = %d\n", result);
                
                /* Free memory */
                free(arr);
                break;
                
            case 3:
                /* Get array size */
                printf("\n--- Replace Even With Zero ---\n");
                printf("Enter array size: ");
                scanf("%d", &size);
                
                /* Allocate and fill original array */
                arr = (int *)malloc(size * sizeof(int));
                
                for (i = 0; i < size; i++) {
                    printf("Enter element %d: ", i + 1);
                    scanf("%d", &arr[i]);
                }
                
                /* Call function */
                newArr = replaceEvenWithZero(arr, size);
                
                /* result is displayed */
                printf("Result: [");
                for (i = 0; i < size - 1; i++) {
                    printf("%d, ", newArr[i]);
                }
                printf("%d]\n", newArr[size - 1]);
                
                /* Free memory */
                free(arr);
                free(newArr);
                break;
                
            case 4:
                /* Get input from user */
                printf("\n--- Perfect Square Check ---\n");
                printf("Enter a number: ");
                scanf("%d", &n);
                
                /* this checks if it is a perfect sqaure */
                result = perfectSquare(n);
                
                /* Show the result */
                if (result == 1) {
                    printf("True\n");
                } else {
                    printf("False\n");
                }
                break;
                
            case 5:
                /* User inputs words */
                printf("\n--- Count Vowels ---\n");
                printf("Enter a string: ");
                getchar();  /* Clear buffer */
                fgets(s, sizeof(s), stdin);  /* read the string given also with spaces */
                
                /* Counts vowels */
                result = countVowels(s);
                
                /* The result is given */
                printf("Number of vowels: %d\n", result);
                break;
                
            case 0:
                printf("Goodbye!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
        
    } while (choice != 0);
    
    return 0;
}
