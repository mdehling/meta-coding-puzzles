/*
 * Rotary Lock (Chapter 1)
 *
 * You're trying to open a lock.  The lock comes with a wheel which has the
 * integers from 1 to N arranged in a circle in order around it (with integers
 * 1 and N adjacent to one another).  The wheel is initially pointing at 1.
 *
 * It takes 1 second to rotate the wheel by 1 unit to an adjacent integer in
 * either direction, and it takes no time to select an integer once the wheel
 * is pointing at it.
 *
 * The lock will open if you enter a certain code.  The code consists of a
 * sequence of M integers, the i-th of which is C[i].  Determine the minimum
 * number of seconds required to select all M of the code's integers in order.
 *
 * Constraints
 *
 * 3 <= N <= 50,000,000
 * 1 <= M <= 1,000
 * 1 <= C[i] <= N
 *
 * Samples test cases:
 *
 * N = 3, M = 3, C = [1,2,3]    -> result = 2
 * N = 10, M = 4, C = [9,4,4,8] -> result = 11
 */

#include <stdio.h>


#define MIN(x,y)    ( (x) < (y) ? (x) : (y) )
#define ABS(x)      ( (x) >= 0 ? (x) : -(x) )


long long getMinCodeEntryTime(int N, int M, int *C) {
    long long result = 0;

    for (int pos = 1; M--; pos = *C++)
        result += MIN(ABS(*C-pos), N - ABS(*C-pos));

    return result;
}


int main(int argc, char **argv) {
    printf("result = %lld, expected = %d\n", 
        getMinCodeEntryTime(3, 3, (int []){1,2,3}), 2);
    printf("result = %lld, expected = %d\n", 
        getMinCodeEntryTime(10, 4, (int []){9,4,4,8}), 11);
}
