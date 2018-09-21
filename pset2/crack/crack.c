//pset2/crack

#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>

// TESTED HASHES:
// 25g4misjGGyG6 > 25MG
// 25LslphwWXHuc > 25zA
// anushree:50xcIMJ0y.RXo > YES
// brian:50mjprEcqC/ts > CA
// bjbrown:50GApilQSG3E2 > UPenn
// lloyd:50n0AAUD.pL8g > lloyd
// malan:50CcfIk1QrPr6 > maybe
// maria:509nVI8B9VfuA > TF
// natmelo:50JIIyhDORqMU > nope
// rob:50JGnXUgaafgc > ROFL
// stelios:51u8F0dkeDSbY > NO
// zamyla:50cI2vYkF0YU2 > LOL

int main(int argc, char **argv)
{
    // assuming user supplies a 56-bit DES hash
    if (argc == 2)
    {
        char password[6];
        char salt[3];
        strncpy(salt, argv[1], 2); // copy first two characters (bytes) of the hash into the salt
        // this is definitely a brute force / O(N5) solution
        // so this letter frequency might help add speed:
        // http://www.oxfordmathcenter.com/drupal7/node/353
        char *alpha = "\0eEtTaAoOiInNsShHrRdDlLcCuUmMwWfFgGyYpPbBvVkKjJsSqQzZ"; // set up a pointer
        // note: a previous attempt had an array of ASCII character numbers but this seemed to be impossibly slow - I wonder why?
        int len = 53; // there are 53 characters in alpha, including \0 :)
        // iterate through the entire max 5-digit array
        for (int i = 0; i < len; i++)
        {
            password[4] = alpha[i];
            for (int j = 0; j < len; j++)
            {
                password[3] = alpha[j];
                for (int k = 0; k < len; k++)
                {
                    password[2] = alpha[k];
                    for (int l = 0; l < len; l++)
                    {
                        password[1] = alpha[l];
                        for (int m = 0; m < len; m++)
                        {
                            // this is working backwards - so the most nested for loop deals with the [0] char of the password
                            // this means we test out one digit passwords before two digit, three digit etc.
                            password[0] = alpha[m];
                            // if the hashed password matches the user's inputted hash - bingo!
                            if (strcmp(argv[1], crypt(password, salt)) == 0)
                            {
                                printf("%s\n", password);
                                return 0;
                            }
                        }
                    }
                }
            }
        }
        // if the code reaches this stage it means a password hasn't been found
        printf("Uh oh! Password not found!\n");
        return 0;
    }
    else
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
}