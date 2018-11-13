#!/usr/bin/env python3
# pset6/crack.py

import crypt
import sys
from hmac import compare_digest as compare_hash

# TESTED HASHES:
# 25g4misjGGyG6 > 25MG
# 25LslphwWXHuc > 25zA
# anushree:50xcIMJ0y.RXo > YES
# brian:50mjprEcqC/ts > CA
# bjbrown:50GApilQSG3E2 > UPenn
# lloyd:50n0AAUD.pL8g > lloyd
# malan:50CcfIk1QrPr6 > maybe
# maria:509nVI8B9VfuA > TF
# natmelo:50JIIyhDORqMU > nope
# rob:50JGnXUgaafgc > ROFL
# stelios:51u8F0dkeDSbY > NO
# zamyla:50cI2vYkF0YU2 > LOL


def crack(needle):
    """Brute force O(n5) algorithm to crack up to a five character Unix DES-based hashed password"""
    if(needle):
        alpha = " eEtTaAoOiInNsShHrRdDlLcCuUmMwWfFgGyYpPbBvVkKjJsSqQzZ"
        alpha_length = len(alpha)
        needle = sys.argv[1]
        salt = needle[0:2]
        haystack = ["" for i in range(5)]
        for i in range(alpha_length):
            haystack[4] = alpha[i]
            for j in range(alpha_length):
                haystack[3] = alpha[j]
                for k in range(alpha_length):
                    haystack[2] = alpha[k]
                    for l in range(alpha_length):
                        haystack[1] = alpha[l]
                        for m in range(alpha_length):
                            haystack[0] = alpha[m]
                            # .strip() removes whitespace - very important here
                            current = "".join(haystack).strip()
                            if (compare_hash(crypt.crypt(current, salt), needle)):
                                return print(current)
        return("Uh oh! Password not found!")
    else:
        print("Usage: crack(password)")


if __name__ == "__main__":
    crack(sys.argv[1])
