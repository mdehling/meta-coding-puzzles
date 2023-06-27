/*
 * Scoreboard Inference (Chapter 2)
 *
 * You are spectating a programming contest with N competitors, each trying to
 * independently solve the same set of programming problems.  Each problem has
 * a point value, which is either 1, 2, or 3.
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
 * N = 5, S = [1,2,3,4,5]   ->  result = 3
 * N = 4, S = [4,3,3,4]     ->  result = 2
 * N = 4, S = [2,4,6,8]     ->  result = 4
 * N = 1, S = [8]           ->  result = 3
 */

#include <stdio.h>


int getMinProblemCount(int N, int *S) {
    int max = 0, max_ = 0;
    int has_one = 0;
    int has_rem[3] = {0, 0, 0};

    for (; N--; S++) {
        if (*S == 1)
            has_one = 1;
        if (*S > max) {
            max_ = max;
            max = *S;
        };
        if (!has_rem[*S % 3])
            has_rem[*S % 3] = 1;
    };

    return max/3 + has_rem[1] + has_rem[2] \
        - (!(max%3) && has_rem[1] && has_rem[2] ? 1 : 0) \
        - (max%3 == 1 && max_ != max-1 && !has_one && has_rem[2] ? 1 : 0);
}


int main(int argc, char **argv) {
    printf("result = %d, expected = %d\n",
        getMinProblemCount(5, (int []){1,2,3,4,5}), 3);
    printf("result = %d, expected = %d\n",
        getMinProblemCount(4, (int []){4,3,3,4}), 2);
    printf("result = %d, expected = %d\n",
        getMinProblemCount(4, (int []){2,4,6,8}), 4);
    printf("result = %d, expected = %d\n",
        getMinProblemCount(1, (int []){8}), 3);
    printf("result = %d, expected = %d\n",
        getMinProblemCount(2, (int []){2,9}), 4);
    printf("result = %d, expected = %d\n",
        getMinProblemCount(3, (int []){1,2,3}), 2);
    printf("result = %d, expected = %d\n",
        getMinProblemCount(3, (int []){2,4,8}), 4);
    printf("result = %d, expected = %d\n",
        getMinProblemCount(2, (int []){2,4}), 2);
    printf("result = %d, expected = %d\n",
        getMinProblemCount(4, (int []){2,4,5,7}), 3);
}

