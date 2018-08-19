#include <stdio.h>
#include <cs50.h>

// ask the user for an input between 1 and 23, print a double half pyramid of specified height
int main(void)
{
    int height;
    // ensure input is between 1 and 23
    do
    {
        height = get_int("Height: ");
    } while (height < 0 || height > 23);
    // print out each row
    for (int i = 0; i < height; i++)
    {
        // print the left gap
        for (int j = height - i - 1; j > 0; j--)
        {
            printf(" ");
        }
        // print the left pyramid
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }
        // print the space between pyramids
        printf("  ");
        // print the right pyramid
        for (int l = 0; l <= i; l++)
        {
            printf("#");
        }
        printf("\n");
    }
}
