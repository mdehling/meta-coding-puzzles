/*
 * Hops
 *
 * A family of frogs in a pond are traveling towards dry land to hibernate.
 * They hope to do so by hopping across a trail of N lily pads, numbered from
 * 1 to N in order.
 *
 * There are F frogs, numbered from 1 to F.  Frog i is currently perched atop
 * lily pad P[i].  No two frogs are currently on the same lily pad.  Lily pad
 * N is right next to the shore, and none of the frogs are initially on lily
 * pad N.
 *
 * Each second, one frog may hop along the trail towards lily pad N.  When a
 * frog hops, it moves to the nearest lily pad after its current lily pad
 * which is not currently occupied by another frog (hopping over any other
 * frogs on intermediate lily pads along the way).  If this causes it to reach
 * lily pad N, it will immediatly exit onto the shore.  Multiple frogs may not
 * simultaneously hop during the same second.
 *
 * Assuming the frogs work together optimally when deciding which frog should
 * hop during each second, determine the minimum number of seconds required
 * for all F of them to reach the shore.
 *
 * Constraints
 *
 * 2 <= N <= 10**12
 * 1 <= F <= 500,000
 * 1 <= P[i] <= N-1
 *
 * Sample test cases
 *
 * N = 3, F = 1, P = [1]        -> result = 2
 * N = 6, F = 3, P = [5,2,4]    -> result = 4
 */

#include <limits.h>
#include <stdio.h>


long long getSecondsRequired(long long N, int F, long long *P) {
    long long p = LLONG_MAX;

    for (; F--; P++)
        if (*P < p) p = *P;

    return N-p;
}


int main(int argc, char **argv) {
    printf("result = %lld, expected = %d\n",
        getSecondsRequired(3, 1, (long long int []){1}), 2);
    printf("result = %lld, expected = %d\n",
        getSecondsRequired(6, 3, (long long int []){5,2,4}), 4);
    printf("result = %lld, expected = %d\n",
        getSecondsRequired(7, 3, (long long int []){5,2,3}), 5);
    printf("result = %lld, expected = %d\n",
        getSecondsRequired(6, 3, (long long int []){3,4,5}), 3);
}

