#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

// encrypt messages using Caesar's cipher
// usage: caesar (k), where k is the number of characters to encrypt by
int main(int argc, char **argv)
{
    // only run if argument is provided
    if (argc == 2)
    {
        // turn key into integer
        int k = atoi(argv[1]);
        string p = get_string("plaintext: "); // prompt user for text to encrypt
        int l = strlen(p);
        int a;
        // iterate through the provided string and produce cipher text
        printf("ciphertext: ");
        for (int i = 0; i < l; i++)
        {
            if (isalpha(p[i])) // only convert alphanumeric characters
            {
                if (islower(p[i])) // convert lowercase into alphabet index (e.g. a = 0, b = 1)
                {
                    a = ((p[i] - 97 + k) % 26) + 97;
                }
                else // convert uppercase
                {
                    a = ((p[i] - 65 + k) % 26) + 65;
                }
                printf("%c", a); // print cipher output
            }
            else
            {
                printf("%c", p[i]); // don't cipher if not alphanumerical
            }
        }
        printf("\n");
        return 0;
    }
    else
    {
        printf("Must supply a non-negative integer\n");
        return 1;
    }
}
