/*
 * Stack Stabilization (Chapter 2)
 *
 * There's a stack of N inflatable discs, with the i-th disc from the top
 * having an initial radius of R[i] inches.
 *
 * The stack is considered unstable if it includes at least one disc whose
 * radius is larger than or equal to that of the disc directly under it.  In
 * other words, for the stack to be stable, each disc must have a strictly
 * smaller radius than that of the disc directly under it.
 *
 * As long as the stack is unstable, you can repeatedly choose a disc and
 * perform on of the following operations:
 *
 * - Inflate the disc, increasing its radius by 1 inch.  This operation takes
 *   A seconds and may be performed on discs of any radius.
 * - Deflate the disc, decreasing its radius by 1 inch.  This operation takes
 *   B seconds and may only be performed if the resulting radius is a positive
 *   integer number of inches.
 *
 * Determine the minimum number of seconds needed in order to make the stack
 * stable.
 *
 * Constraints:
 *
 * 1 <= N <= 50
 * 1 <= R[i] <= 1,000,000,000
 * 1 <= A,B <= 100
 *
 * Sample test cases:
 *
 * R = [2, 5, 3, 6, 5], A = 1, B = 1    ->  result = 5
 * R = [100, 100, 100], A = 2, B = 3    ->  result = 5
 * R = [100, 100, 100], A = 7, B = 3    ->  result = 9
 * R = [6, 5, 4, 3], A = 10, B = 1      ->  result = 19
 * R = [100, 100, 1, 1], A = 2, B = 1   ->  result = 207
 * R = [6, 5, 2, 4, 4, 7], A = 1, B = 1 ->  result = 10
 */

/*
 * Solution
 *
 * 1) This is an integer linear optimization problem.
 *
 * We are solving for S = [Si;Sd] in Z^(N+N) where Si (Sd) is how much to
 * inflate (deflate) the discs and the cost to minimize is [A,B]*[Si;Sd] =
 * A*Si + B*Sd.  Let M be the NxN matrix that is 0 except M[i,i] = 1 and
 * M[i+1,i] = -1.  Then define A = [M;-M] and b = 1 - M*R where R in Z^N is
 * the given vector of initial radii of the discs.  The ILP can now be written
 * as:
 *
 * minimize     [A,B]*[Si;Sd]
 * subject to   [M;-M]*[Si;Sd] >= 1 - M*R ,
 *              S >= 0,
 * and          S in Z^(2N).
 *
 * 2) The matrix A is totally unimodular.
 *
 * Every entry in A is 0, +1, or -1, every column contains at most two
 * non-zero entries, and (if there are two) they will be of opposite sign.
 *
 * This implies that any solution to the LP relaxation will be integral.
 *
 * 3) Solve using your favorite LP algorithm.
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ENTRY(T,i,j) ( (T)->values[i*(T)->cols+j] )


typedef struct tableau {
    int rows, cols;
    long long *values;
} tableau_t;


void init_tableau(tableau_t *T, int N, int *R, int A, int B) {
    // z-row
    for (int i = 1; i < T->rows; i++) {
        ENTRY(T, 0, i) = A;
        ENTRY(T, 0, N+i) = B;
    };

    // b-column
    ENTRY(T, 1, 0) = - 1 + R[0];
    for (int i = 2; i < T->rows; i++)
        ENTRY(T, i, 0) = - 1 + (R[i-1] - R[i-2]);

    // rest of the values
    ENTRY(T, 1, 1) = 1;
    ENTRY(T, 1, N+1) = -1;
    for (int i = 2; i < T->rows; i++) {
        ENTRY(T, i, i) = 1;
        ENTRY(T, i, N+i) = -1;
        ENTRY(T, i, i-1) = -1;
        ENTRY(T, i, N+i-1) = 1;
    };
}

void row_pivot(tableau_t *T, int row, int col) {
    if (ENTRY(T,row,col) != 1)
        errx(EXIT_FAILURE, "invalid row pivot");

    for (int j = 0; j < T->cols; j++) {
        if (j == col)
            continue;
        ENTRY(T,row,j) = - ENTRY(T,row,j);
        for (int i = 0; i < T->rows; i++) {
            if (i == row)
                continue;
            ENTRY(T,i,j) += ENTRY(T,i,col) * ENTRY(T,row,j);
        };
    };
}

void find_feasible_solution(tableau_t *T) {
    for (int i = 1; i < T->rows; i++) {
        if (ENTRY(T,i,0) < 0)
            row_pivot(T,i,i);
    };
}

void col_pivot(tableau_t *T, int row, int col) {
    if (ENTRY(T,row,col) != -1)
        errx(EXIT_FAILURE, "invalid column pivot");

    for (int i = 0; i < T->rows; i++) {
        if (i == row) continue;
        ENTRY(T,i,col) = - ENTRY(T,i,col);
        for (int j = 0; j < T->cols; j++) {
            if (j == col) continue;
            ENTRY(T,i,j) -= ENTRY(T,i,col) * ENTRY(T,row,j);
        };
    };
}

int improve_solution(tableau_t *T) {
    int pivot_row = 0;
    long long min_b_val;

    for (int j = 1; j < T->cols; j++) {
        if (ENTRY(T,0,j) >= 0)
            continue;
        for (int i = 1; i < T->rows; i++) {
            if (ENTRY(T,i,j) >= 0)
                continue;
            if (!pivot_row) {
                pivot_row = i;
                min_b_val = ENTRY(T,i,0);
            } else if (ENTRY(T,i,0) < min_b_val) {
                pivot_row = i;
                min_b_val = ENTRY(T,i,0);
            };
        };
        if (pivot_row) {
            col_pivot(T,pivot_row,j);
            return 1;
        };
    };
    return 0;
}


long long getMinimumSecondsRequired(int N, int *R, int A, int B) {
    long long result;

    tableau_t T = {
        .rows = N + 1, .cols = 2*N + 1,
        .values = calloc(T.rows * T.cols, sizeof *T.values)
    };

    init_tableau(&T, N, R, A, B);
    find_feasible_solution(&T);
    while (improve_solution(&T));

    result = ENTRY(&T,0,0);

    free(T.values);

    return result;
}


int main(int argc, char **argv) {
    printf("result = %d, expected = %d\n\n",
            getMinimumSecondsRequired(3, (int []){3, 2, 1}, 2, 1), 6);
    printf("result = %d, expected = %d\n\n",
            getMinimumSecondsRequired(5, (int []){2, 5, 3, 6, 5}, 1, 1), 5);
    printf("result = %d, expected = %d\n\n",
            getMinimumSecondsRequired(3, (int []){100, 100, 100}, 2, 3), 5);
    printf("result = %d, expected = %d\n\n",
            getMinimumSecondsRequired(3, (int []){100, 100, 100}, 7, 3), 9);
    printf("result = %d, expected = %d\n\n",
            getMinimumSecondsRequired(4, (int []){6, 5, 4, 3}, 10, 1), 19);
    printf("result = %d, expected = %d\n\n",
            getMinimumSecondsRequired(4, (int []){100, 100, 1, 1}, 2, 1), 207);
    printf("result = %d, expected = %d\n\n",
            getMinimumSecondsRequired(6, (int []){6, 5, 2, 4, 4, 7}, 1, 1), 10);
}

