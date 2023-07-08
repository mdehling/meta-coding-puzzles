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
 * 1) Reduce graph to strongly connected components.
 *
 * The link graph has pages as nodes and links as (directed) edges. Because we
 * are allowed to revisit nodes, but the page view counts only once, it
 * doesn't matter where we enter and exit a strongly connected component (a
 * subgraph in which every node is reachable from every other).  This means
 * that we can consider the graph of strongly connected components to which we
 * assign as a weight the number of vertices in each component.
 *
 * 2) Find longest path in directed acyclic graph.
 *
 * This is a simple depth-first search.
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>


#define MIN(x,y)    ( (x) < (y) ? (x) : (y) )
#define MAX(x,y)    ( (x) > (y) ? (x) : (y) )


typedef struct graph {
    int num_vertices, num_edges;
    int *weight;                    // [num_vertices]
    int *num_edges_;                // [num_vertices]
    int **edge;                     // [num_vertices][num_edges_[]]
} graph_t;


graph_t *graph_new(int num_vertices, int num_edges, int *source, int *target) {
    graph_t *g = malloc(sizeof *g);

    g->num_vertices = num_vertices;
    g->num_edges = num_edges;

    g->weight = calloc(num_vertices, sizeof *g->weight);
    g->num_edges_ = calloc(num_vertices, sizeof *g->num_edges_);

    // count number of edges for each vertex
    for (int j = 0; j < num_edges; j++)
        g->num_edges_[source[j]-1]++;

    // allocate space for edges
    g->edge = malloc(num_vertices * sizeof *g->edge);
    g->edge[0] = malloc(num_edges * sizeof *g->edge[0]);
    for (int i = 1; i < num_vertices; i++)
        g->edge[i] = g->edge[i-1] + g->num_edges_[i-1];

    // fill in the edges
    int *I = calloc(num_vertices, sizeof *I);
    for (int j = 0; j < num_edges; j++) {
        int src = source[j]-1, tgt = target[j]-1;
        g->edge[src][I[src]++] = tgt;
    };
    free(I);

    return g;
}


void graph_delete(graph_t *g) {
    if (!g)
        return;

    free(g->edge[0]);
    free(g->edge);
    free(g->num_edges_);
    free(g->weight);
    free(g);
}


void graph_print(graph_t *g) {
    printf("digraph {\n");
    for (int i = 0; i < g->num_vertices; i++) {
        printf("  %d (w = %d)", i+1, g->weight[i]);
        if (g->num_edges_[i] > 0) {
            printf(" -> {", i+1);
            for (int j = 0; j < g->num_edges_[i]; j++)
                printf(" %d", g->edge[i][j]+1);
            printf(" }");
        };
        printf("\n");
    };
    printf("}\n");
}


void scc_find(graph_t *g, int v, int *np,
        int *index, int *low_link, int *on_stack,
        int *s, int *sip,
        int *scc, int *num_sccp)
{
    index[v] = *np, low_link[v] = *np, (*np)++;
    s[(*sip)++] = v, on_stack[v] = 1;

    for (int j = 0; j < g->num_edges_[v]; j++) {
        int w = g->edge[v][j];
        if (!index[w]) {
            scc_find(g, w, np, index, low_link, on_stack, s, sip, scc, num_sccp);
            low_link[v] = MIN(low_link[v], low_link[w]);
        } else if (on_stack[w]) {
            low_link[v] = MIN(low_link[v], index[w]);
        };
    };

    if (low_link[v] == index[v]) {
        int w;
        do {
            w = s[--(*sip)], on_stack[w] = 0;
            scc[w] = *num_sccp;
        } while (w != v);
        (*num_sccp)++;
    };
}


/*
 * Reduce graph to its strongly connected components using Tarjan's algorithm.
 */
graph_t *graph_scc_reduce(graph_t *g) {
    int *scc = calloc(g->num_vertices, sizeof *scc), num_scc = 0;

    //
    // Use Tarjan's algorithm to find strongly connected components (SCC)
    //
    int *index = calloc(g->num_vertices, sizeof *index);
    int *low_link = calloc(g->num_vertices, sizeof *low_link);
    int *on_stack = calloc(g->num_vertices, sizeof *on_stack);

    int n = 1;
    int s[g->num_vertices], si = 0;

    for (int i = 0; i < g->num_vertices; i++) {
        if (index[i])
            continue;

        scc_find(g, i, &n, index, low_link, on_stack, s, &si, scc, &num_scc);
    };

    free(index);
    free(low_link);
    free(on_stack);


    //
    // Generate the SCC graph from g and the results of Tarjan's algorithm
    //
    graph_t *sccg = malloc(sizeof *sccg);

    sccg->num_vertices = num_scc;
    sccg->num_edges = 0;

    sccg->weight = calloc(num_scc, sizeof *sccg->weight);
    sccg->num_edges_ = calloc(num_scc, sizeof *sccg->num_edges_);

    // go through the original graph's edges and count those that remain valid
    for (int i = 0; i < g->num_vertices; i++) {
        for (int j = 0; j < g->num_edges_[i]; j++) {
            int src = scc[i], tgt = scc[g->edge[i][j]];
            if (src != tgt) {
                sccg->num_edges++;
                sccg->num_edges_[src]++;
            };
        };
    };

    // reserve space for edges
    sccg->edge = malloc(num_scc * sizeof *sccg->edge);
    sccg->edge[0] = malloc(sccg->num_edges * sizeof *sccg->edge[0]);
    for (int i = 1; i < num_scc; i++)
        sccg->edge[i] = sccg->edge[i-1] + sccg->num_edges_[i-1];

    // fill in edges & update weights
    int *I = calloc(num_scc, sizeof *I);
    for (int i = 0; i < g->num_vertices; i++) {
        sccg->weight[scc[i]]++;
        for (int j = 0; j < g->num_edges_[i]; j++) {
            int src = scc[i], tgt = scc[g->edge[i][j]];
            if (src != tgt) {
                sccg->edge[src][I[src]++] = tgt;
            };
        };
    };
    free(I);

    free(scc);
    return sccg;
}


/*
 * Find longest path in a directed acyclic graph by DFS
 */
int graph_longest_path(graph_t *g) {
    int W = 0;
    int *w = calloc(g->num_vertices, sizeof *w);

    int r;
    int i[g->num_vertices];
    int j[g->num_vertices];

    for (int i0 = 0; i0 < g->num_vertices; i0++) {
        if (w[i0])
            continue;

        r = 0, i[r] = i0, j[r] = 0;
        while (r >= 0) {
            if (j[r] < g->num_edges_[i[r]]) {
                int tgt = g->edge[i[r]][j[r]++];
                if (!w[tgt]) {
                    r++, i[r] = tgt, j[r] = 0;
                };
            } else {
                // done with this vertex
                for (j[r] = 0; j[r] < g->num_edges_[i[r]]; j[r]++) {
                    int tgt = g->edge[i[r]][j[r]];
                    w[i[r]] = MAX(w[i[r]], w[tgt]);
                };
                w[i[r]] += g->weight[i[r]];
                r--;
            };
        };
        W = MAX(W, w[i0]);
    };

    free(w);
    return W;
}


int getMaxVisitableWebpages(int N, int M, int *A, int *B) {
    graph_t *g = graph_new(N, M, A, B);
    // graph_print(g);

    graph_t *sccg = graph_scc_reduce(g);
    // graph_print(sccg);

    return graph_longest_path(sccg);
}


int main(int argc, char **argv) {
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(4, 4, (int []){1,2,3,4}, (int []){4,1,2,1}), 4);
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(5, 6, (int []){3,5,3,1,3,2}, (int []){2,1,2,4,5,4}), 4);
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(10, 9, (int []){3,2,5,9,10,3,3,9,4}, (int []){9,5,7,8,6,4,5,3,9}), 5);
    printf("result = %d, expected = %d\n",
        getMaxVisitableWebpages(5, 6, (int []){1,2,3,3,4,5}, (int []){2,3,1,4,5,2}), 5);
}

