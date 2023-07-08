/*
 * Boss Fight
 *
 * There are N warriors, the i-th of which has a health of H[i] units and can
 * deal D[i] damage per second.  They are confronting a boss who has unlimited
 * health and can deal B units of damage per second.  Both the warriors and
 * the boss deal damage continuously -- for example, in half a second, the
 * boss deals B/2 units of damage.
 *
 * The warrior feel it would be unfair for many of them to fight the boss at
 * once, so they'll select just two representatives to go into battle.  One
 * warrior i will be the front line, and a different warrior j will back them
 * up.  During the battle, the boss will attack warrior i until that warrior
 * is defeated (that is, until the boss has dealt H[i] units of damage to
 * them), and will then attack warrior j until that warrior is also defeated,
 * at which point the battle will end.  Along the way, each of the two
 * warriors will do damage to the boss as long as they are undefeated.
 *
 * Of course, the warriors will never prevail, but they'd like to determine
 * the maximum amount of damage they could deal to the boss for any choice of
 * warriors i and j before the battle ends.
 *
 * Constraints
 *
 * 2 <= N <= 500,000
 * 1 <= H[i] <= 1,000,000,000
 * 1 <= D[i] <= 1,000,000,000
 * 1 <= B <= 1,000,000,000
 *
 * Sample test cases
 *
 * N = 3, H = {2,1,4}, D = {3,1,2}, B = 4           ->  result = 6.5
 * N = 4, H = {1,1,2,100}, D = {1,2,1,3}, B = 8     ->  result = 62.75
 * N = 4, H = {1,1,2,3}, D = {1,2,1,100}, B = 8     ->  result = 62.75
 */

#include <stdio.h>


#define MAX(x,y)    ( (x) > (y) ? (x) : (y) )


double getMaxDamageDealt(int N, int *H, int *D, int B) {
    double max_damage = 0.0;
    double damage;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (j == i)
                continue;
            damage = H[i] * (D[i] + D[j]) + H[j] * D[j];
            max_damage = MAX(max_damage, damage);
        };
    };

    return max_damage / B;
}


int main (int argc, char **argv) {
    printf("result = %f, expected = %f\n",
        getMaxDamageDealt(3, (int []){2,1,4}, (int []){3,1,2}, 4), 6.5);
    printf("result = %f, expected = %f\n",
        getMaxDamageDealt(4, (int []){1,1,2,100}, (int []){1,2,1,3}, 8), 62.75);
    printf("result = %f, expected = %f\n",
        getMaxDamageDealt(4, (int []){1,1,2,3}, (int []){1,2,1,100}, 8), 62.75);
}

