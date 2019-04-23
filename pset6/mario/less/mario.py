from cs50 import get_int


def main():
    # Get an integer between 1 - 8
    height = get_positive_int("Height: ")
    # Print bricks
    print_bricks(height)


def get_positive_int(prompt):
    # Use get_int to get an integer from the user
    n = get_int(prompt)
    # While not in the proper range
    while n < 1 or n > 8:
        n = get_int(prompt)
    return n


def print_bricks(height):
    # Initialize spaces to height
    spaces = height
    for i in range(height):
        # Set spaces to the current row
        spaces -= 1
        for j in range(spaces):
            # Print the spaces while omitting the new line
            print(" ", end="")
        # Set the bricks
        bricks = height - spaces
        for k in range(bricks):
            # Print the bricks
            print("#", end="")
        # Create a new line
        print("")


if __name__ == "__main__":
    main()
