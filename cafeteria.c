#include <stdio.h>
#include <stdlib.h>


int compare(const void *i, const void *j) {
    return *(const long long *)i > *(const long long *)j;
}


long long getMaxAdditionalDinersCount(long long N, long long K, int M, long long *S) {
    long long result;

    qsort(S, M, sizeof *S, compare);

    result = (S[0]-1) / (K+1);
    for (int i = 0; i < M-1; i++)
        result += ((S[i+1]-1) - (S[i]+K)) / (K+1);
    result += (N-S[M-1]) / (K+1);

    return result;
}


int main(int argc, char **argv) {
    long long S1[] = { 2, 6 };
    long long S2[] = { 11, 6, 14 };

    printf("%lld\n", getMaxAdditionalDinersCount(10, 1, 2, S1));
    printf("%lld\n", getMaxAdditionalDinersCount(15, 2, 3, S2));

    return 0;
}

