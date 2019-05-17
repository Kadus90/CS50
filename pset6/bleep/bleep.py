from cs50 import get_string
from sys import argv


def main():
    # Does the command line match usage
    if len(argv) != 2:
        exit("Usage: python bleep.py dictionary")

    # Store the name of the file
    file = argv[1]

    # Store the dictionary file type
    file_ext = '.txt'

    # Does the file use the correct extension
    if file.endswith(file_ext) != True:
        # If you're here it's too late
        exit("Usage: python bleep.py dictionary")

    # Create the dictionary as a set
    dictionary = set()

    # Load the data from the file
    with open(f'{file}', 'r') as f:
        # For each line in the file
        for line in f:
            # Load the word into a variable
            word = line.strip()

            # Add the word to the dictionary
            dictionary.add(word)

    # Get the message from the user
    user_message = get_string("What message would you like to censor?\n")

    # Create the censored string
    censored_message = str()

    # For each item in the message
    for i, item in enumerate(user_message.split()):
        # If the lower case of the item is in the dictionary
        if (item.lower()) in dictionary:
            # Censor item
            item = '*' * len(item)

        # If not at the beginning of the string
        if i:
            # Print a space
            censored_message = censored_message + ' '

        # Add item to the censored message
        censored_message = censored_message + item

    # Print the message
    print(censored_message)


if __name__ == "__main__":
    main()
