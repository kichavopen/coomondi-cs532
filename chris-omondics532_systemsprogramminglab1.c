#include <stdio.h>
#include <math.h>

int main() {
    int given_number;
    int is_prime = 1;  // The prime number is initialied and given first value
    int i;
    
    // User inputs the number
    printf("Enter a number: ");
    scanf("%d", &given_number);
    
    // Check if number is less than 2 (not prime)
    if (given_number < 2) {
        is_prime = 0;
    } else {

        // check for the numbers factors from 2 all the way to the square root of the number
        for (i = 2; i <= sqrt(given_number); i++) {
            if (given_number % i == 0) {
                is_prime = 0;  // Found a factor, not prime
                break;
            }
        }
    }
    
    // Print the result
    if (is_prime == 1) {
        printf("The number is prime\n");
    } else {
        printf("The number is not prime\n");
    }
    
    return 0;
}
// the c file is compiled by doing gcc -prime.c -o prime -lm

// the -lm is standard math library
