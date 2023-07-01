/*
 * Tunnel Time
 *
 * There's a circular train track with a circumference of C metres.  Positions
 * along the track are measured in meters, clockwise from its topmost point.
 * For example, the topmost point is position 0, 1 meter clockwise along the
 * track is position 1, and 1 meter counterclockwise along the track is
 * position C-1.
 *
 * A train with negligible length runs clockwise along this track at a speed
 * of 1 meter per second, starting from position 0.  After C seconds go by,
 * the train will have driven around the entire track and returned to position
 * 0, at which point it will continue going around again, with this process
 * repeated indefinitely.
 *
 * There are N tunnels covering sections of the track, the i-th of which
 * begins at position A[i] and ends at position B[i] (and therefore has a
 * length of B[i] - A[i] meters).  No tunnel covers or touches position 0
 * (including at its endpoints), and no two tunnels intersect or touch one
 * another (including at their endpoints).  For example, if there's a tunnel
 * spanning the interval of positions [1,4], there cannot be another tunnel
 * spanning interval [2,3] nor [4,5].
 *
 * The train's tunnel time is the total number of seconds it has spent going
 * through tunnels so far.  Determine the total number of seconds which will
 * go by before the train's tunnel time becomes K.
 *
 * Constraints
 *
 * 3 <= C <= 10**12
 * 1 <= N <= 500,000
 * 1 <= A[i] < B[i] <= C-1
 * 1 <= K <= 10**12
 *
 * Sample test cases
 *
 * C = 10, N = 2, A = [1,6], B = [3,7], K = 7               -> result = 22
 * C = 50, N = 3, A = [39,19,28], B = [49,27,35], K = 15    -> result = 35
 */

#include <stdio.h>
#include <stdlib.h>


int ll_cmp(const void *x, const void *y) {
    return *(const long long *)x > *(const long long *)y;
}


long long getSecondsElapsed(long long C, int N, long long *A, long long *B, long long K) {
    long long result;

    qsort(A, N, sizeof *A, ll_cmp);
    qsort(B, N, sizeof *B, ll_cmp);

    long long tunnel_time_per_round = 0;
    for (int i = 0; i < N; i++)
        tunnel_time_per_round += B[i]-A[i];

    long long num_rounds = (K / tunnel_time_per_round);
    K -= num_rounds * tunnel_time_per_round;

    result = num_rounds * C;

    if (K == 0) {
        // finish at the end of the last tunnel
        result -= C - B[N-1];
        return result;
    };

    for (int i = 0; i < N; i++) {
        if (K <= B[i]-A[i]) {
            result += A[i] + K;
            break;
        };
        K -= B[i]-A[i];
    };

    return result;
}


int main(int argc, char **argv) {
    printf("result = %lld, expected = %d\n",
        getSecondsElapsed(10, 2, (long long []){1,6}, (long long []){3,7}, 7), 22);
    printf("result = %lld, expected = %d\n",
        getSecondsElapsed(50, 3, (long long []){39,19,28}, (long long []){49,27,35}, 15), 35);
}

