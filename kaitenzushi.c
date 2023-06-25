/*
 * Kaitenzushi
 *
 * There are N dishes in a row on a kaiten belt, with the i-th dish being of
 * type D[i].  Some dishes may be of the same type as one another.
 *
 * You're very hungry, but you'd also like to keep things interesting.  The N
 * dishes will arrive in front of you, one after another in order, and for
 * each one you'll eat it as long as it isn't the same type as any of the
 * previous K dishes you've eaten.  You eat very fast, so you can consume a
 * dish before the next one gets to you.  Any dishes you choose not to eat as
 * they pass will be eaten by others.
 *
 * Determine how many dishes you'll end up eating.
 *
 * Constraints:
 *
 * 1 <= N <= 500,000
 * 1 <= K <= N
 * 1 <= D[i] <= 1,000,000
 *
 * Sample test cases:
 *
 * N = 6, D = [1,2,3,3,2,1], K = 1      -> result = 5
 * N = 6, D = [1,2,3,3,2,1], K = 2      -> result = 4
 * N = 7, D = [1,2,1,2,1,2,1], K = 2    -> result = 2
 */

#include <stdio.h>
#include <stdlib.h>


#define MAX_DISHES      1000000


int getMaximumEatenDishCount(int N, int *D, int K) {
    int *H = calloc(MAX_DISHES, sizeof *H);
    int result = 0;

    for (; N--; D++)
        if (H[*D] == 0 || result - H[*D] >= K)
            H[*D] = ++result;

    return result;
}


int main(int argc, char **argv) {
    printf("result = %d, expected = %d\n",
        getMaximumEatenDishCount(6, (int []){1,2,3,3,2,1}, 1), 5);
    printf("result = %d, expected = %d\n",
        getMaximumEatenDishCount(6, (int []){1,2,3,3,2,1}, 2), 4);
    printf("result = %d, expected = %d\n",
        getMaximumEatenDishCount(7, (int []){1,2,1,2,1,2,1}, 2), 2);
}

