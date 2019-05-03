# Pull in resources
from sys import argv, exit
from cs50 import get_string

# Build main


def main():
    # Fetch key from the command line
    key = validate_key(argv)

    # Print plaintext
    plain = get_plaintext("plaintext: ")

    # Run cipher
    cipher = run_cipher(plain, key)

    # Print ciphertext
    print(f"ciphertext: {cipher}")


def validate_key(argv):
    # Check for errors on the command line
    if len(argv) != 2:
        if len(argv) < 2:
            exit("error: no key present")
        if len(argv) > 2:
            exit("error: too many inputs")

    value = int(argv[1])

    # Verify that the key is positive
    if value < 1:
        exit("error: not a positive integer")

    # If no errors, return the value
    return value


# Prompt for plaintext
def get_plaintext(prompt):
    text = get_string(prompt)
    return text


# Use the key on the plaintext
def run_cipher(string, key):
    # Load the new text
    encipher = string

    # For each character
    for i in range(len(string)):
        # If it is alphabetical
        if string[i].isalpha() is True:
            encipher = encipher[:i] + shift_character(string[i], key) + encipher[i+1:]

    return encipher


# Adjust a character
def shift_character(char, key):
    # Create a floor to adjust the character
    lower = ord('a')
    upper = ord('A')
    original = ord(char)

    # Check if the character is upper or lower
    if char.isupper() == True:
        shifted = chr((((original - upper) + key) % 26) + upper)
    elif char.islower() == True:
        shifted = chr((((original - lower) + key) % 26) + lower)

    return shifted


# Check for main
if __name__ == "__main__":
    main()
