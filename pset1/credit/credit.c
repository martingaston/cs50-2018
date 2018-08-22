#include <stdio.h>
#include <cs50.h>
#include <math.h>

// calculate luhn's algorithm as per cs50 pset1 credit spec :)
int main(void)
{
    // prompt user for positive long long int number
    long long int digits;
    do
    {
        digits = get_long_long("Number: ");
    } while (digits < 0);
    // set variables for total digits, and to store products of even and odd numbers
    int digit_count = log10(digits) + 1;
    int digit_product_even = 0;
    int digit_product_odd = 0;
    // iterate backwards through digit int to calculate product of digits
    for (long long int i = digit_count, j = 1; i > 0; i -= 2, j *= 100)
    {
        int dig = digits / (10 * j) % 10;
        digit_product_odd += digits / (1 * j) % 10;
        // digit * 2 on numbers over 4 will return two numbers which need to be added separately
        if (dig * 2 > 9)
        {
            digit_product_even += dig * 2 % 10;
            digit_product_even += dig * 2 / 10;
        }
        else
        {
            digit_product_even += dig * 2;
        }
    }
    // if last digit of combined products is 0
    // then check the digit amount and starting digits to confirm card type
    if ((digit_product_even + digit_product_odd) % 10 == 0)
    {
        if ((digit_count == 16 && digits / 1000000000000000 == 4) || (digit_count == 13 && digits / 1000000000000 == 4))
        {
            printf("VISA\n");
        }
        else if (digit_count == 16 && digits / 100000000000000 > 50 && digits / 100000000000000 < 56)
        {
            printf("MASTERCARD\n");
        }
        else if (digit_count == 15 && digits / 10000000000000 == 37)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}