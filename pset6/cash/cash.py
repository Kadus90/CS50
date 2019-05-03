# Import get_float
from cs50 import get_float


def main():
    # Ask user how much change is owed.
    change = get_change("Change owed: ")

    # Determine the amount of coins needed
    coins = break_change(change)

    # Print out the minimum number of coins
    print(coins)


def get_change(prompt):
    while True:
        c = get_float(prompt)
        if c > 0:
            break
    return c


def break_change(c):
    # Set values for coins
    q = .25
    d = .10
    n = .05
    p = .01

    # Initialize the counter
    count = 0

    # Check change versus coins and increase the count
    while c >= q:
        c = c - q
        c = round(c,2)
        count = count + 1

    while c >= d:
        c = c - d
        c = round(c,2)
        count = count + 1

    while c >= n:
        c = c - n
        c = round(c,2)
        count = count + 1

    while c > 0:
        c = c - p
        c = round(c,2)
        count = count + 1

    return count


if __name__ == "__main__":
    main()
