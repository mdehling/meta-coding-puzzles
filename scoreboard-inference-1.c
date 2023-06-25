/*
 * Scoreboard Inference (Chapter 1)
 *
 * You are spectating a programming contest with N competitors, each trying to
 * independently solve the same set of programming problems.  Each problem has
 * a point value, which is either 1 or 2.
 *
 * On the scoreboard, you observe that the i-th competitor has attained a
 * score of S[i], which is a positive integer equal to the sum of the point
 * values of all the problems they have solved.
 *
 * The scoreboard does not display the number of problems in the contest, nor
 * their point values.  Using the information available, you would like to
 * determine the minimum possible number of problems in the contest.
 *
 * Constraints
 *
 * 1 <= N <= 500,000
 * 1 <= S[i] <= 1,000,000,000
 *
 * Sample test cases
 *
 * N = 6, S = [1,2,3,4,5,6]     ->  result = 4
 * N = 4, S = [4,3,3,4]         ->  result = 3
 * N = 4, S = [2,4,6,8]         ->  result = 4
 */

#include <stdio.h>


int getMinProblemCount(int N, int *S) {
    int max_score = 0;
    int have_odd_score = 0;

    for (; N--; S++) {
        if (*S > max_score)
            max_score = *S;
        if (!have_odd_score && *S % 2)
            have_odd_score = 1;
    };

    return max_score/2 + have_odd_score;
}


int main(int argc, char **argv) {
    printf("result = %d, expected = %d\n",
        getMinProblemCount(6, (int []){1,2,3,4,5,6}), 4);
    printf("result = %d, expected = %d\n",
        getMinProblemCount(4, (int []){4,3,3,4}), 3);
    printf("result = %d, expected = %d\n",
        getMinProblemCount(4, (int []){2,4,6,8}), 4);
}

