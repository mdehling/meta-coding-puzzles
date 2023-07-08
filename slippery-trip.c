/*
 * Slippery Trip
 *
 * There's a grid of cells with R rows (numbered from 1 to R from top to
 * bottom) and C columns (numbered from 1 to C from left to right).  The grid
 * wraps around horizonatally, meaning that column 1 is just to the right of
 * column C (and column C is just to the left of column 1).
 *
 * The cell in row i and column j initially contains one of the following
 * (indicated by the character G[i][j]):
 *
 * - If G[i][j] = '.', the cell is empty.
 * - If G[i][j] = '*', the cell contains a coin.
 * - If G[i][j] = '>', the cell contains an arrow pointing right.
 * - If G[i][j] = 'v', the cell contains an arrow pointing down.
 *
 * You may cyclically shift each row to the right as many times as you'd like
 * (including not at all).  Each such shift causes each of the row's cells to
 * move 1 column to the right, with its rightmost cell (in column C) wrapping
 * around and moving to column 1.
 *
 * After you've finished rotating the rows to your liking, you'll take a trip
 * through the grid, starting by entering the cell at the top-left corner (in
 * row 1 and column 1) downward from above the grid.  Upon entering a cell, if
 * it contains a coin that you haven't yet collected, you'll collect it.  If
 * it contains an arrow, your direction of travel will change to that of the
 * arrow (either right or down).  Either way, you'll then proceed to the next
 * adjacent cell in your direction of travel.  If you move rightward from
 * column C, you'll wrap around to column 1 in the same row, and if you move
 * downward from row R, you'll end your trip.  Note that you may only collect
 * each cell's coin at most once, that your trip might last forever, and that
 * once you begin your trip, you cannot shift the grid's rows further.
 *
 * Determine the maximum number of col_coins you can collect on your trip.
 *
 * Constraints
 *
 * 2 <= R,C <= 400,000
 * R*C <= 800,000
 * G[i][j] in {'.','*','>','v'}
 *
 * Sample test cases
 *
 * G = {".***",
 *      "**v>",
 *      ".*.."}     ->  result = 4
 *
 * G = {">**",
 *      "*>*",
 *      "**>"}      ->  result = 4
 *
 * G = {">>",
 *      "**"}       ->  result = 0
 *
 * G = {">*v*>*",
 *      "*v*v>*",
 *      ".*>..*",
 *      ".*..*v"}   ->  result = 6
 */

#include <stdio.h>
#include <stdlib.h>


#define MAX(x,y)    ( (x) > (y) ? (x) : (y) )


int getMaxCollectableCoins(int R, int C, char **G) {
    int *col_coins[R], *end[R];
    int *max_col_coins = calloc(R, sizeof *max_col_coins);

    col_coins[0] = calloc(R*C, sizeof *col_coins[0]);
    end[0] = calloc(R*C, sizeof *end[0]);
    for (int row = 1; row < R; row++) {
        col_coins[row] = col_coins[row-1] + C;
        end[row] = end[row-1] + C;
    };

    // mark last row as end
    for (int col = 0; col < C; col++) {
        end[R-1][col] = 1;
    };

    // go through rows backwards and fill col_coins[] and end[]
    for (int row = R-1; row >= 0; row--) {
        int row_coins[C], row_end, row_coins_total = -1;

        for (int col = 0; col < C; col++) {
            switch (G[row][col]) {
            case '*':
                col_coins[row][col] = 1;
                break;
            case '>':
                // only search once per row
                if (row_coins_total == -1) {
                    // step 1: find 'v' & count coins
                    int vcol = -1;

                    row_coins_total = 0;
                    for (int j = 0; j < C; j++) {
                        switch (G[row][j]) {
                        case '*':
                            row_coins_total++;
                            break;
                        case 'v':
                            vcol = j;
                            break;
                        };
                    };

                    if (vcol == -1) {
                        // step 2a: no 'v' found, so '>' hits every coin but
                        // ends in an infinite loop
                        row_end = 1;
                        for (int j = 0; j < C; j++) {
                            if (G[row][j] == '>')
                                row_coins[j] = row_coins_total;
                        };
                    } else {
                        // step 2b: found a 'v', so loop backwards through the
                        // row and record coins between a 'v' and any '>'
                        row_end = 0;
                        int coin_count = 0;
                        for (int j = C-1; j >= 0; j--) {
                            switch (G[row][(vcol+j)%C]) {
                            case '*':
                                coin_count++;
                                break;
                            case '>':
                                row_coins[(vcol+j)%C] = coin_count;
                                break;
                            case 'v':
                                coin_count = 0;
                                break;
                            };
                        };
                    };
                };

                col_coins[row][col] = row_coins[col];
                end[row][col] = row_end;
                break;
            };

            if (!end[row][col])
                col_coins[row][col] += max_col_coins[row+1];

            max_col_coins[row] = MAX(max_col_coins[row], col_coins[row][col]);
        };
    };

    return max_col_coins[0];
}


int main(int argc, char **argv) {
    printf("result = %d, expected = %d\n",
        getMaxCollectableCoins(3, 4, (char *[]){".***", "**v>", ".*.."}), 4);
    printf("result = %d, expected = %d\n",
        getMaxCollectableCoins(3, 3, (char *[]){">**", "*>*", "**>"}), 4);
    printf("result = %d, expected = %d\n",
        getMaxCollectableCoins(2, 2, (char *[]){">>", "**"}), 0);
    printf("result = %d, expected = %d\n",
        getMaxCollectableCoins(4, 6, (char *[]){">*v*>*", "*v*v>*", ".*>..*", ".*..*v"}), 6);
}

