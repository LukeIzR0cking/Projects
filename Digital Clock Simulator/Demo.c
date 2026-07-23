#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "Clock.h"

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    int prime = 97;
    int lim = (int)sqrt(prime);
    bool isPrime = true;
    printf("%s Input number: %d \n", getTimestamp(), prime);
    for (int i=2; i < lim; i++){  //Must start from i=2 since 2 is the first prime number
        if (prime % i == 0){         //Must check the prime
            isPrime = false;
            printf("%s\n", getTimestamp());
            break;
        } 
        else{
            continue;
        }
    }
    if (isPrime)
    {
        printf("%s It's a prime number\n", getTimestamp());
    }
    else{
        printf("%s It's not a prime number\n", getTimestamp());
    }
        
    return 0;
}