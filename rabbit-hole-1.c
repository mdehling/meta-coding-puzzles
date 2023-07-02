/*
 * Rabbit Hole (Chapter 1)
 *
 * You're having a grand old time clicking through the rabbit hole that is
 * your favorite online encyclopedia.
 *
 * The encyclopedia consists of N different web pages, numbered from 1 to N.
 * Each page i contains nothing but a single link to a different page L[i].
 *
 * A session spent on this website involves beginning on one of the N pages,
 * and then navigating around using the links until you decide to stop.  That
 * is, while on page i, you may either move to page L[i], or stop your
 * browsing session.
 *
 * Assuming you can choose which page you begin the session on, what's the
 * maximum number of different pages you can visit in a single session?  Note
 * that a page only counts once even if visited multiple times during the
 * session.
 *
 * Constraints
 *
 * 2 <= N <= 500,000
 * 1 <= L[i] <= N
 * L[i] != i
 *
 * Sample test cases
 *
 * N = 4, L = {4,1,2,1}     -> result = 4
 * N = 5, L = {4,3,5,1,2}   -> result = 3
 * N = 5, L = {2,4,2,2,3}   -> result = 4
 */

#include <stdio.h>
#include <stdlib.h>


void deep_dive(int N, int *L, int i0, int *d) {
    int s[N];
    int si = 0;

    int *visited = calloc(N, sizeof *visited);
    for (int i = i0; visited[i] == 0; i = L[i]-1) {
        visited[i] = si+1;
        s[si++] = i;
    };

    int loop_length = visited[s[si-1]] - visited[L[s[si-1]]-1] + 1;

    for (int i = 0; i < si-loop_length; i++)
        d[s[i]] = si - i;

    for (int i = si-loop_length; i < si; i++)
        d[s[i]] = loop_length;

    free(visited);
}


int getMaxVisitableWebpages(int N, int *L) {
    int *d = calloc(N, sizeof *d);

    for (int i = 0; i < N; i++) {
        if (d[i] == 0)
            deep_dive(N, L, i, d);
    };

    int max_d = 0;
    for (int i = 0; i < N; i++)
        if (d[i] > max_d)
            max_d = d[i];

    free(d);

    return max_d;
}


int main(int argc, char **argv) {
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(4, (int []){4,1,2,1}), 4);
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(5, (int []){4,3,5,1,2}), 3);
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(5, (int []){2,4,2,2,3}), 4);
}

