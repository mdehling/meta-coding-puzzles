/*
 * Rabbit Hole (Chapter 2)
 *
 * You're having a grand old time clicking through the rabbit hole that is
 * your favorite online encyclopedia.
 *
 * The encyclopedia consists of N different web pages, numbered from 1 to N.
 * There are M links present across the pages, the i-th of which is present on
 * page A[i] and links to a different page B[i].  A page may include multiple
 * links, including multiple leading to the same other page.
 *
 * A session spent on this website involves beginning on one of the N pages,
 * and then navigating around using the links until you decide to stop.  That
 * is, while on page i, you may either move to page any of the pages linked to
 * from it, or stop your browsing session.
 *
 * Assuming you can choose which page you begin the session on, what's the
 * maximum number of different pages you can visit in a single session?  Note
 * that a page only counts once even if visited multiple times during the
 * session.
 *
 * Constraints
 *
 * 2 <= N <= 500,000
 * 1 <= M <= 500,000
 * 1 <= A[i], B[i] <= N
 * A[i] != B[i]
 *
 * Sample test cases
 *
 * N = 4, M = 4, A = {1,2,3,4}, B = {4,1,2,1}           ->  result = 4
 * N = 5, M = 6, A = {3,5,3,1,3,2}, B = {2,1,2,4,5,4}   ->  result = 4
 * N = 10, M = 9, A = {3,2,5,9,10,3,3,9,4}, B = {9,5,7,8,6,4,5,3,9}
 *                                                      ->  result = 5
 */

/*
 * Solution
 *
 * 1) Contract cycles.
 *
 * The link graph has the pages as nodes and the links as (directed) edges.
 * Because we are allowed to revisit nodes, but the page view counts only
 * once, it doesn't matter where we enter and exit a link cycle.  This means
 * that we can contract any cycle to a node to which we assign as a weight the
 * number of pages in the cycle.
 *
 * 2) Find longest path in directed acyclic graph.
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>


#define MAX(x,y)    ( (x) > (y) ? (x) : (y) )

typedef struct page {
    int weight;
    int num_links;
    int *link;
} page_t;


typedef struct index {
    int num_pages;
    page_t *page;
    int *links;
} index_t;


void create_index(index_t *ip, int num_pages, int *weight,
        int num_links, int *link_src, int *link_dst)
{
    page_t *page = malloc(num_pages * sizeof *page);
    int *links = malloc(num_links * sizeof *links);

    // initialize
    for (int j = 0; j < num_pages; j++) {
        page[j].weight = weight[j];
        page[j].num_links = 0;
    };

    // count the number of links on each page
    for (int i = 0; i < num_links; i++) {
        int src = link_src[i]-1, dst = link_dst[i]-1;
        if (dst != src)
            page[src].num_links++;
    };

    // set up the page links pointers
    page[0].link = links;
    for (int j = 1; j < num_pages; j++)
        page[j].link = page[j-1].link + page[j-1].num_links;

    // fill page links
    int *I = calloc(num_pages, sizeof *I);
    for (int i = 0; i < num_links; i++) {
        int src = link_src[i]-1, dst = link_dst[i]-1;
        if (dst != src)
            page[src].link[I[src]++] = dst;
    };
    free(I);

    // write index
    *ip = (index_t){.num_pages = num_pages, .page = page, .links = links};
}


void print_index(index_t *ip) {
    for (int j = 0; j < ip->num_pages; j++) {
        // print the (possibly empty) list of links for each page
        printf("page[%d] = { .weight = %d, .link = [", j, ip->page[j].weight);
        for (int i = 0; i < ip->page[j].num_links; i++)
            printf(" %d", ip->page[j].link[i]);
        printf(" ] }\n");
    };
}


void find_circuits(index_t *ip, int *nc, int **cp) {
    int *c = calloc(ip->num_pages, sizeof *c), n = 0;
    int *I = malloc(ip->num_pages * sizeof *I);
    int s[ip->num_pages], si = 0;

    /*
     * Create page -> circuit mapping
     */
    for (int j = 0; j < ip->num_pages; j++) {
        if (c[j])
            continue;

        c[j] = ++n;
        s[si++] = j;

        bzero(I, ip->num_pages * sizeof *I);
        while (si) {
            int cpi = s[si-1];
            page_t *cp = ip->page + cpi;

            int *cpI = I + cpi;
            if (*cpI < cp->num_links) {
                int tpi = cp->link[(*cpI)++];
                if (c[tpi]) {
                    // target part of circuit
                    if (c[tpi] == n)
                        c[cpi] = n;
                } else {
                    s[si++] = tpi;
                };
            } else {
                // done with current page
                if (--si && c[cpi])
                    c[s[si-1]] = c[cpi];
            };
        };
    };
    free(I);

    /*
     * Print circuits for debugging
     */
    /*
    for (int k = 0; k < n; k++) {
        printf("circuit %d:", k+1);
        for (int j = 0; j < ip->num_pages; j++)
            if (c[j] == k+1)
                printf(" %d", j);
        printf("\n");
    };
    */

    *nc = n, *cp = c;
}


int maximize_weight(index_t *ip) {
    int *w = calloc(ip->num_pages, sizeof *w), W = 0;
    int *I = malloc(ip->num_pages * sizeof *I);
    int s[ip->num_pages], si = 0;

    // total weight of a page is the max of weight of targets + page weight
    for (int j = 0; j < ip->num_pages; j++) {
        if (w[j])
            continue;

        s[si++] = j;

        bzero(I, ip->num_pages * sizeof *I);
        while (si) {
            int cpi = s[si-1];
            page_t *cp = ip->page + cpi;

            int *cpI = I + cpi;
            if (*cpI < cp->num_links) {
                int tpi = cp->link[(*cpI)++];
                // skip links to self
                if (!w[tpi])
                    s[si++] = tpi;
            } else {
                // done with current page
                for (int i = 0; i < cp->num_links; i++)
                    w[cpi] = MAX(w[cpi], w[cp->link[i]]);
                w[cpi] += cp->weight;
                si--;
            };
        };

        W = MAX(W, w[j]);
    };
    free(I);
    free(w);

    return W;
}


int getMaxVisitableWebpages(int N, int M, int *A, int *B) {
    index_t page_index, circuit_index;
    int *page_weight = malloc(N * sizeof *page_weight);
    int num_circuits, *circuit;

    printf("\n");

    for (int j = 0; j < N; j++)
        page_weight[j] = 1;

    create_index(&page_index, N, page_weight, M, A, B);

    find_circuits(&page_index, &num_circuits, &circuit);

    int *circuit_weight = calloc(num_circuits, sizeof *circuit_weight);
    for (int j = 0; j < N; j++)
        circuit_weight[circuit[j]-1] += page_weight[j];

    for (int i = 0; i < M; i++)
        A[i] = circuit[A[i]-1], B[i] = circuit[B[i]-1];

    create_index(&circuit_index, num_circuits, circuit_weight, M, A, B);
    print_index(&circuit_index);

    return maximize_weight(&circuit_index);
}


int main(int argc, char **argv) {
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(4, 4, (int []){1,2,3,4}, (int []){4,1,2,1}), 4);
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(5, 6, (int []){3,5,3,1,3,2}, (int []){2,1,2,4,5,4}), 4);
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(10, 9, (int []){3,2,5,9,10,3,3,9,4}, (int []){9,5,7,8,6,4,5,3,9}), 5);
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(5, 6, (int []){1,2,3,3,4,5}, (int []){2,3,1,4,5,1}), 5);
}

