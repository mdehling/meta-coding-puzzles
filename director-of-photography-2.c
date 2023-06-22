/*
 * Director of Photography (Chapter 2).
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
 * 1 <= N <= 300,000
 * 1 <= X <= Y <= N
 *
 * Sample test cases:
 *
 * N = 5, C = "APABA", X = 1, Y = 2     ->  result = 1
 * N = 5, C = "APABA", X = 2, Y = 3     ->  result = 0
 * N = 8, C = ".PBAAP.B", X = 1, Y = 3  ->  result = 3
 */

#include <stdio.h>


long long getArtisticPhotographCount(int N, char *C, int X, int Y) {
    int P[N], A[N], B[N];
    int NP = 0, NA = 0, NB = 0;
    int ill = 0, ilr = 0, irl = 0, irr = 0;
    int kll = 0, klr = 0, krl = 0, krr = 0;
    long long result = 0;

    // build index arrays
    for (int n = 0; n < N; n++) {
        switch (C[n]) {
            case 'P': P[NP++] = n; break;
            case 'A': A[NA++] = n; break;
            case 'B': B[NB++] = n; break;
        };
    };

    for (int j = 0; j < NA; j++) {
        while (irr < NP && P[irr] <= A[j] + Y) irr++;
        while (irl < irr && P[irl] < A[j] + X) irl++;
        while (ilr < irl && P[ilr] <= A[j] - X) ilr++;
        while (ill < ilr && P[ill] < A[j] - Y) ill++;

        while (krr < NB && B[krr] <= A[j] + Y) krr++;
        while (krl < krr && B[krl] < A[j] + X) krl++;
        while (klr < krl && B[klr] <= A[j] - X) klr++;
        while (kll < klr && B[kll] < A[j] - Y) kll++;

        result += (ilr-ill) * (krr-krl);
        result += (irr-irl) * (klr-kll);
    };

    return result;
}


int main(int argc, char **argv) {
    printf("result = %lld, expected = %d\n",
        getArtisticPhotographCount(5, "APABA", 1, 2), 1);
    printf("result = %lld, expected = %d\n",
        getArtisticPhotographCount(5, "APABA", 2, 3), 0);
    printf("result = %lld, expected = %d\n",
        getArtisticPhotographCount(8, ".PBAAP.B", 1, 3), 3);
}

