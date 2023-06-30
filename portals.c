/*
 * Portals
 *
 * You've found yourself in a grid of cells with R rows and C columns.  The
 * cell in the i-th row from the top and j-th column from the left contains
 * one of the following characters G[i][j]:
 *
 * - If G[i][j] = '.', the cell is empty.
 * - If G[i][j] = 'S', the cell contains your starting position.  There is
 *   exactly one such cell.
 * - If G[i][j] = 'E', the cell contains an exit.  There is at least one such
 *   cell.
 * - If G[i][j] = '#', the cell contains a wall.
 * - Otherwise, if G[i][j] is a lowercase letter ('a'..'z'), the cell contains
 *   a portal marked with that letter.
 *
 * Your objective is to reach any exit from you starting position as quickly
 * as possible.  Each second, you may take either of the following actions:
 *
 * - Walk to a cell adjacent to your current one (directly above, below, to
 *   the left, or to the right), as long as you remain within the grid and
 *   that cell does not contain a wall.
 *
 * - If you current cell contains a portal, teleport to any other cell in the
 *   grid containing a portal marked with the same letter as you current
 *   cell's portal.
 *
 * Determine the minimum number of seconds required to reach an exit, if it's
 * possible to do so at all.  If it's not possible, return -1 instead.
 *
 * Constraints
 *
 * 1 <= R, C <= 50
 * G[i][j] in ".SE#[a-z]"
 *
 * Sample test cases
 *
 * G = {".E.",
 *      ".#E",
 *      ".S#"}          -> result = 4
 *
 *
 * G = {"a.Sa",
 *      "####",
 *      "Eb.b"}         -> result = -1
 *
 * G = {"aS.b",
 *      "####",
 *      "Eb.a"}         -> result = 4
 *
 * G = {"xS..x..Ex"}    -> result = 3
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>


#define MAX_ROWS        50
#define MAX_COLS        50

#define MAX_EXITS       (MAX_ROWS*MAX_COLS)
#define MAX_PORTALS     (MAX_ROWS*MAX_COLS)


#define PORTAL_BIT(x)   ( 1 << ((x)-'a') )


typedef struct {
    // the given map
    int rows, cols;
    char **map;
    // minimum cost to reach position
    unsigned **cost;
    // starting coordinates
    int start_row, start_col;
    // number and coordinates of exits
    int exits;
    int exit_row[MAX_EXITS], exit_col[MAX_EXITS];
    // number, coordinates, and char of portals
    int portals;
    int portal_row[MAX_PORTALS], portal_col[MAX_PORTALS];
    char portal_chr[MAX_PORTALS];
} map_info_t;


void map_costs(map_info_t *mi, int row, int col, unsigned cost, int portals_used) {
    if (cost >= mi->cost[row][col] || mi->map[row][col] == '#')
        return;

    mi->cost[row][col] = cost++;

    // no reason to go past an exit
    if (mi->map[row][col] == 'E')
        return;

    // no route should go through the same portal type twice
    if (mi->map[row][col] >= 'a' && mi->map[row][col] <= 'z' &&
        !(portals_used & PORTAL_BIT(mi->map[row][col])))
    {
        for (int i = 0; i < mi->portals; i++) {
            if (mi->portal_chr[i] == mi->map[row][col]) {
                map_costs(mi, mi->portal_row[i], mi->portal_col[i], cost, \
                    portals_used | PORTAL_BIT(mi->map[row][col]));
            };
        };
    };

    if (row > 0)
        map_costs(mi, row-1, col, cost, portals_used);
    if (col > 0)
        map_costs(mi, row, col-1, cost, portals_used);
    if (row+1 < mi->rows)
        map_costs(mi, row+1, col, cost, portals_used);
    if (col+1 < mi->cols)
        map_costs(mi, row, col+1, cost, portals_used);
}


int getSecondsRequired(int R, int C, char **G) {
    map_info_t mi = {
        .rows = R, .cols = C, .map = G,
        .exits = 0, .portals = 0,
    };

    // find start & make lists of exits and portals
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (G[i][j] == 'S') {
                mi.start_row = i;
                mi.start_col = j;
            } else if (G[i][j] == 'E') {
                mi.exit_row[mi.exits] = i;
                mi.exit_col[mi.exits] = j;
                mi.exits++;
            } else if (G[i][j] >= 'a' && G[i][j] <= 'z') {
                mi.portal_row[mi.portals] = i;
                mi.portal_col[mi.portals] = j;
                mi.portal_chr[mi.portals] = G[i][j];
                mi.portals++;
            };
        };
    };

    // initialize cost map
    unsigned *linear_cost = malloc(R*C * sizeof *linear_cost);
    unsigned *cost[R];

    for (int i = 0; i < R; i++) {
        cost[i] = linear_cost + i * (C * sizeof *linear_cost);
        for (int j = 0; j < C; j++) {
            cost[i][j] = UINT_MAX;
        };
    };
    mi.cost = &cost[0];

    // call recursive cost map function
    map_costs(&mi, mi.start_row, mi.start_col, 0, 0);

    // loop over exits and choose the closest one
    unsigned min_cost = UINT_MAX;
    for (int i = 0; i < mi.exits; i++)
        if (cost[mi.exit_row[i]][mi.exit_col[i]] < min_cost)
            min_cost = cost[mi.exit_row[i]][mi.exit_col[i]];

    return min_cost;
}


int main(int argc, char **argv) {
    printf("result = %d, expected = %d\n",
        getSecondsRequired(3, 3, (char *[]){".E.", ".#E", ".S#"}), 4);
    printf("result = %d, expected = %d\n",
        getSecondsRequired(3, 4, (char *[]){"a.Sa", "####", "Eb.b"}), -1);
    printf("result = %d, expected = %d\n",
        getSecondsRequired(3, 4, (char *[]){"aS.b", "####", "Eb.a"}), 4);
    printf("result = %d, expected = %d\n",
        getSecondsRequired(1, 9, (char *[]){"xS..x..Ex"}), 3);
}

