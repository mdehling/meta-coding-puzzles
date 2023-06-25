/*
 * Rotary Lock (Chapter 2)
 *
 * You're trying to open a lock.  The lock comes with two wheels, each of
 * which has the integers from 1 to N arranged in a circle in order around it
 * (with integers 1 and N adjacent to one another).  Each wheel is initially
 * pointing at 1.
 *
 * It takes 1 second to rotate a wheel by 1 unit to an adjacent integer in
 * either direction, and it takes no time to select an integer once the wheel
 * is pointing at it.
 *
 * The lock will open if you enter a certain code.  The code consists of a
 * sequence of M integers, the i-th of which is c[i].  For each integer in the
 * sequence, you may select it with either of the two wheels.  Determine the
 * minimum number of seconds required to select all M of the code's integers
 * in order.
 *
 * Constraints
 *
 * 3 <= N <= 1,000,000,000
 * 1 <= M <= 3,000
 * 1 <= c[i] <= N
 *
 * Samples test cases:
 *
 * N = 3, M = 3, c = [1,2,3]    -> result = 2
 * N = 10, M = 4, c = [9,4,4,8] -> result = 6
 */

/*
 * Solution
 *
 * 1) This can be phrased as a quadratic integer programming problem.
 *
 * find         [x; y; p; q; r; s; u; v] in Z^8M
 * minimize     |p + q + r + s|
 * subject to   x + y = 1
 *              y*L*(p-q) + x*L*(r-s) + N*(u-v) = c
 *
 * where L is the NxN lower triangular matrix given by L[i,j] = 1 if j <= i.
 *
 * 2) The quadratic constraint can be written as w^T*Q*w + q*w = c, where
 *
 * Q = [ [ 0, 0, 0, 0, L,-L, 0, 0];
 *       [ 0, 0, L,-L, 0, 0, 0, 0];
 *       [ 0, L, 0, 0, 0, 0, 0, 0];
 *       [ 0,-L, 0, 0, 0, 0, 0, 0];
 *       [ L, 0, 0, 0, 0, 0, 0, 0];
 *       [-L, 0, 0, 0, 0, 0, 0, 0];
 *       [ 0, 0, 0, 0, 0, 0, 0, 0];
 *       [ 0, 0, 0, 0, 0, 0, 0, 0] ]
 *
 * q = [0; 0; 0; 0; 0; 0; N;-N]
 *
 * Problem: Q is indefinite and hence the optimization problem in non-convex.
 *
 */

#include <stdio.h>


#define MIN(x,y)    ( (x) < (y) ? (x) : (y) )
#define ABS(x)      ( (x) >= 0 ? (x) : -(x) )

#define COST(c,d,N) ( MIN( ABS((c)-(d)), (N)-ABS((c)-(d)) ) )


long long getMinTime(int N, int M, int *C, int D1, int D2) {
    long long cost1, cost2;

    if (M == 0)
        return 0;

    cost1 = COST(*C,D1,N) + getMinTime(N, M-1, C+1, *C, D2);
    cost2 = COST(*C,D2,N) + getMinTime(N, M-1, C+1, D1, *C);

    return MIN(cost1, cost2);
}


long long getMinCodeEntryTime(int N, int M, int *C) {
    return getMinTime(N, M, C, 1, 1);
}


int main(int argc, char **argv) {
    printf("result = %lld, expected = %d\n", 
        getMinCodeEntryTime(3, 3, (int []){1,2,3}), 2);
    printf("result = %lld, expected = %d\n", 
        getMinCodeEntryTime(10, 4, (int []){9,4,4,8}), 6);
    printf("result = %lld, expected = %d\n", 
        getMinCodeEntryTime(5, 4, (int []){3,4,3,4}), 4);
}
