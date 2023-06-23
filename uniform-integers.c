/*
 * Uniform Integers
 *
 * A positive integer is considered uniform if all of its digits are equal.
 * For example, 222 is uniform, while 223 is not.
 *
 * Given two positive integer A and B, determine the number of uniform
 * integers between A and B, inclusive.
 *
 * Constraints:
 *
 * 1 <= A <= B <= 10**12
 *
 * Sample test cases:
 *
 * A = 75, B = 300  ->  result = 5
 * A = 1, B = 9     ->  result = 9
 * A = 999,999,999,999, B = 999,999,999,999     ->  result = 1
 */

#include <stdio.h>


int getUniformIntegerCountInInterval(long long A, long long B) {
    int An = 1, Ad;
    int Bn = 1, Bd;
    long long u;

    for (u = 1; (A/u) / 10; ++An, u = 10*u + 1);
    Ad = A/u;
    if (A%u) Ad++;

    for (u = 1; (B/u) / 10; ++Bn, u = 10*u + 1);
    Bd = B/u;

    return (Bd-Ad+1) + (Bn-An)*9;
}


int main(int argc, char **argv) {
    printf("A = %lld, B = %lld, result = %d, expected = %d\n",
            75, 300, getUniformIntegerCountInInterval(75, 300), 5);
    printf("A = %lld, B = %lld, result = %d, expected = %d\n",
            1, 9, getUniformIntegerCountInInterval(1, 9), 9);
    printf("A = %lld, B = %lld, result = %d, expected = %d\n",
            999999999999, 999999999999,
            getUniformIntegerCountInInterval(999999999999, 999999999999), 1);
}

