/*
 * Stack Stabilization (Chapter 1)
 *
 * There's a stack of N inflatable discs, with the i-th disc from the top
 * having an initial radius of R[i] inches.
 *
 * The stack is considered unstable if it includes at least one disc whose
 * radius is larger than or equal to that of the disc directly under it.  In
 * other words, for the stack to be stable, each disc must have a strictly
 * smaller radius than that of the disc directly under it.
 *
 * As long as the stack is unstable, you can repeatedly choose any disc of
 * your choice and deflate it down to have a radius of your choice which is
 * strictly smaller than the disc's prior radius.  The new radius must be a
 * positive integer number of inches.
 *
 * Determine the minimum number of discs which need to be deflated in order to
 * make the stack stable, if this is possible at all.  If it is impossible to
 * stabilize the stack, return -1 instead.
 *
 * Constraints:
 *
 * 1 <= N <= 50
 * 1 <= R[i] <= 1,000,000,000
 *
 * Sample test cases:
 *
 * N = 5, R = [2, 5, 3, 6, 5]   ->  result = 3
 * N = 3, R = [100, 100, 100]   ->  result = 2
 * N = 4, R = [6, 5, 4, 3]      ->  result = -1
 */

#include <stdio.h>


int getMinimumDeflatedDiscCount(int N, int *R) {
    int result = 0;
    int r = R[N-1] + 1;

    while (N--) {
        if (R[N] <= N)
            return -1;

        if (R[N] >= r) {
            R[N] = r - 1;
            result++;
        };

        r = R[N];
    };

    return result;
}


int main(int argc, char **argv) {
    int R1[] = { 2, 5, 3, 6, 5 };
    int R2[] = { 100, 100, 100 };
    int R3[] = { 6, 5, 4, 3 };

    printf("result = %d, expected = %d\n",
            getMinimumDeflatedDiscCount(5, R1), 3);
    printf("result = %d, expected = %d\n",
            getMinimumDeflatedDiscCount(3, R2), 2);
    printf("result = %d, expected = %d\n",
            getMinimumDeflatedDiscCount(4, R3), -1);
}

