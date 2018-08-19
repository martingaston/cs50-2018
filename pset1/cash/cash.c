#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>

// use a greedy algorithm to calculate the minimum amount of coins required to give correct change
int main(void)
{
    int cents;
    int coins[4] = {25, 10, 5, 1};
    int coins_used = 0;
    // get a positive float from the user
    do
    {
        cents = round(get_float("Change owed: ") * 100);
    } while (cents < 0);
    // iterate over the coins array and work out calculate:
    // 1: how many of that coin can be used on the remaining number
    // 2: set the remaining amount of change to the modulo remainder of that coin
    for (int i = 0; i < 4; i++)
    {
        coins_used += cents / coins[i];
        cents = cents % coins[i];
    }
    printf("%i\n", coins_used);
}
