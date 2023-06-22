/*
 * Director of Photography (Chapter 1).
 *
 * A photography set consists of N cells in a row, numbered from 1 to N in
 * order, and can be represented by a string C of length N.  Each cell i is
 * one of the following types (indicated by C[i], the i-th character of C):
 *
 * - if C[i] == 'P', it is allowed to contain a photographer
 * - if C[i] == 'A', it is allowed to contain an actor
 * - if C[i] == 'B', it is allowed to contain a backdrop
 * - if C[i] == '.', it must be left empty
 *
 * A photograph consists of a photographer, an actor, and a backdrop, such
 * that each of them is placed in a valid cell, and such that the actor is
 * between the photographer and the backdrop.  Such a photograph is considered
 * artistic if the distance between the photographer and the actor is between
 * X and Y cells (inclusive).  The distance between cells i and j is |i - j|
 * (the absolute value of the difference between their indices).
 *
 * Determine the number of different artistic photographs which could
 * potentially be taken at the set.  Two photographs are considered different
 * if they involve a different photographer cell, actor cell, and/or backdrop
 * cell.
 *
 * Constraints:
 *
 * 1 <= N <= 200
 * 1 <= X <= Y <= N
 *
 * Sample test cases:
 *
 * N = 5, C = "APABA", X = 1, Y = 2     ->  result = 1
 * N = 5, C = "APABA", X = 2, Y = 3     ->  result = 0
 * N = 8, C = ".PBAAP.B", X = 1, Y = 3  ->  result = 3
 */

#include <stdio.h>

#define MIN(x,y)    ( (x) < (y) ? (x) : (y) )
#define MAX(x,y)    ( (x) > (y) ? (x) : (y) )


int getArtisticPhotographCount(int N, char *C, int X, int Y) {
    int result = 0;

    for (int i = 0; i < N; i++) {
        if (C[i] != 'P') continue;

        for (int j = MAX(0,i-Y); j <= i-X; j++) {
            if (C[j] != 'A') continue;

            for (int k = MAX(0,j-Y); k <= j-X; k++) {
                if (C[k] != 'B') continue;

                result++;
                printf("B: %d, A: %d, P: %d\n", k, j, i);
            };
        };

        for (int j = i+X; j <= MIN(i+Y,N-1); j++) {
            if (C[j] != 'A') continue;

            for (int k = j+X; k <= MIN(j+Y,N-1); k++) {
                if (C[k] != 'B') continue;

                result++;
                printf("P: %d, A: %d, B: %d\n", i, j, k);
            };
        };
    };

    return result;
}


int main(int argc, char **argv) {
    printf("result = %d, expected = %d\n",
        getArtisticPhotographCount(5, "APABA", 1, 2), 1);
    printf("result = %d, expected = %d\n",
        getArtisticPhotographCount(5, "APABA", 2, 3), 0);
    printf("result = %d, expected = %d\n",
        getArtisticPhotographCount(8, ".PBAAP.B", 1, 3), 3);
}

