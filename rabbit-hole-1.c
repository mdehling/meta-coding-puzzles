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


int getMaxVisitableWebpages(int N, int *L) {
    int *v = calloc(N, sizeof *v);  // keep track of pages visited
    int *d = calloc(N, sizeof *d);  // record link depth from each page
    int D = 0;                      // max depth
    int s[N], si;                   // stack to keep track of link path
    int j, n;

    for (int i = 0; i < N; i++) {
        if (d[i] != 0)
            continue;

        // starting on page i, follow links and create a stack of unseen pages
        si = 0;
        for (j = i; v[j] == 0; j = L[j]-1)
            s[si] = j, si++, v[j] = si;
        // page j is the first page we had already visited

        if (d[j] != 0) {
            // been here on a previous path
            n = d[j];
        } else {
            // loop back to an earlier point of this path
            n = si - v[j] + 1;

            // every page of a loop of length n has max link depth n
            for (j = si-n; j < si; j++)
                d[s[j]] = n;
            // forget loop part
            si -= n;
        };

        // link depth decreases along the path
        for (j = 0; j < si; j++)
            d[s[j]] = si-j + n;

        // record new max depth
        if (d[i] > D)
            D = d[i];
    };

    free(d);
    free(v);

    return D;
}


int main(int argc, char **argv) {
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(4, (int []){4,1,2,1}), 4);
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(5, (int []){4,3,5,1,2}), 3);
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(5, (int []){2,4,2,2,3}), 4);
}

