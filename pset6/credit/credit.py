import math


def get_positive_int(prompt):
    """Ask user for a positive integer between 1 and 23"""
    while True:
        try:
            num = int(input("{}: ".format(prompt)), 10)
        except ValueError:
            continue
        if num > 0:
            return num
        else:
            continue


def luhn():
    """Calculate luhn's algorithm as per cs50 pset1 credit spec :) """
    card_number = get_positive_int("Number")
    digit_count = math.ceil(math.log10(card_number))
    card_str = [int(i) for i in str(card_number)]
    first_two_digits = card_str[0] * 10 + card_str[1]
    digit_product_odd = [x for i in range(
        digit_count - 1, 0, -2) for x in list(str(card_str[i-1] * 2))]
    digit_product_even = [str(card_str[i-1])
                          for i in range(digit_count, 0, -2)]
    if sum(int(i) for i in digit_product_odd + digit_product_even):
        if digit_count == 15 and first_two_digits == 37 or first_two_digits == 34:
            print("AMEX")
        elif digit_count == 13 or 15 and card_str[0] == 4:
            print("VISA")
        elif digit_count == 16 and card_str[0] == 5 and first_two_digits >= 51 and first_two_digits <= 55:
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")


if __name__ == "__main__":
    luhn()
