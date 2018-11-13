def get_positive_int():
    """Ask user for a positive integer between 0 and 23"""
    while True:
        try:
            height = int(input("Height: "), 10)
        except ValueError:
            continue
        if height >= 0 and height < 24:
            return height
        else:
            continue


def mario():
    """Print a double half pyramid of a specified height"""
    height = get_positive_int()
    for row in range(1, height + 1):
        for _space in range(height - row, 0, -1):
            print(" ", end="")
        for _brick in range(row):
            print("#", end="")
        print("  ", end="")
        for _brick in range(row):
            print("#", end="")
        print()


if __name__ == "__main__":
    mario()
