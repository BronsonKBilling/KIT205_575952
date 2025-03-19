#define _CRT_SECURE_NO_WARNINGS
#include "time.h"
#include <stdio.h>
#include <stdlib.h>

long f1(long n) {   // O(n^2)
    long k = 0;                         // O(1)
    for (long i = 0; i < n; i++) {      // O(n) * (O(1) + O(n)) = O(n^2)
        for (long j = 0; j < n; j++) {  // O(n) * (O(1) + O(1)) = O(n)
            k++;                        // O(1)
        }
    }
    return k;                           // O(1)
}

void f2(long n) {   // O(n^2)
    long k = 0;                             // O(1)
    for (long i = 0; i < n; i++) {          // O(n) * (O(1) + O(n)) = O(n^2)
        for (long j = 0; j < i; j++) {      // O(n) * (O(1) + O(1)) = O(n)
            k++;                            // O(1)
        }
        for (long t = 0; t < 10000; t++) {  // O(1) * (O(1) + O(1)) = O(1)
            k++;                            // O(1)
        }
    }
}

void f3(long n) { // O(n^1 ln n)
    // T(n) = aT(n/b) + f(n)
    // T(n) = 2T(n/2) + O(1)
    // log2^2 = n^1, O(1) = n^1. So it is the second of the 3 possible cases
    if (n > 0) {   // O(1)
        f3(n / 2); // O(1)
        f3(n / 2); // O(1)
    }
}

void f4(long n) { //O(n^2)
    // T(n) = aT(n/b) + f(n)
    // T(n) = 2T(n/2) + O(n^2)
    // log2^2 = n^1, O(n^2) grows faster than log2^2. So it is the third of the 3 possible cases
    if (n > 0) {  // O(1) + O(n^2) = O(n^2)
        f4(n / 2);  // O(1)
        f4(n / 2);  // O(1)
        f2(n);      // O(n^2)
    }
}

void f5(long n) { // O(log n * n^2)
    long k = 0;
    for (long i = 0; i < 10; i++) { // O(1) * (O(1) + O(O(log n * n^2)) = O(log n * n^2)
        long j = n;                 // O(1)
        while (j > 0) {             // O(log n) * (O(1) + O(n^2)) = O(log n * n^2)
            f1(1000);               // O(n^2)
            k++;                    // O(1)
            j = j / 2;              // O(1)
        }
    }
}

void f6(long n) {   // O(n^2)
    f2(n);          // O(n^2)
    f3(n);          // O(n^1 ln n)
    f5(n);          // O(log n * n^2)
}

void f7(long n) {   // O(n^2)
    long k = 0;                         //O(1)
    for (long i = 0; i < f1(n); i++) {  //O(n^2)
        k++;                            //O(1)
    }
    for (long j = 0; j < n; j++) {      // O(n)
        k++;                            // O(1)
    }
}

int main() {
    int quit = 0; // Program will quit if this is set to 1
    long n = 1;   // Given starting value so loop will run
    clock_t start;

    while (n != 0) {
        printf("Enter a value for n: ");
        scanf("%d", &n);

        start = clock();
        f7(n);
        clock_t diff = clock() - start;

        long msec = diff * 1000 / CLOCKS_PER_SEC;
        printf("Operation took %d milliseconds\n\n", msec);
    }

	return 0;
}