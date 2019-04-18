/*
Implement a program called recover that recovers JPEGs from a forensic image.
    * Implement your program in a file called recover.c in a directory called recover.
    * Your program should accept exactly one command-line argument, the name of a forensic image from which to recover JPEGs.
    * If your program is not executed with exactly one command-line argument, it should remind the user of correct usage, as with fprintf (to stderr), and main should return 1.
    * If the forensic image cannot be opened for reading, your program should inform the user as much, as with fprintf (to stderr), and main should return 2.
    - Your program, if it uses malloc, must not leak any memory.
*/

#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <stdint.h>

typedef uint8_t  BYTE;

// Function to determine the correct file type
bool isRaw(char *f);
bool isJPEG(BYTE *g);

int main(int argc, char *argv[])
{
    // Variables
    char *image = argv[1];
    char filename[8];
    BYTE check[512];
    FILE *img;
    int found = 0;
    bool inJPEG = false;

    // Validate proper usage
    if (argc != 2 || isRaw(image) != true)
    {
        // Open error log
        FILE *stderr = fopen("Error_Log.txt", "w");

        // Send the error
        fprintf(stderr, "Usage: ./recover image\n");

        // Close the file
        fclose(stderr);

        // Quit
        return 1;
    }

    // Validate that the forensic file can be read
    FILE *forensic = fopen(image, "r");

    // Verify that the file has been opened
    if (forensic == NULL)
    {
        // Open error log
        FILE *stderr = fopen("Error_Log.txt", "w");

        // Send the error
        fprintf(stderr, "File could not be read\n");

        // Close the file
        fclose(stderr);

        // Quit
        return 2;
    }

    // Read through the forensic file
    while (fread(check, 512, 1, forensic) == 1)
    {
        // Check if we are at the beginning of a jpeg
        if (isJPEG(check) == true)
        {
            // Update filename
            sprintf(filename, "%03i.jpg", found);

            // Increase found and set inJPEG to true
            found++;
            inJPEG = true;
        }

        if (inJPEG == true)
        {
            // Open file
            img = fopen(filename, "a");

            // Write to file
            fwrite(check, 1, 512, img);

            // Close the last img
            fclose(img);
        }

    }

    // Close the forensic file
    fclose(forensic);
}

// Function to determine the correct file type is used
bool isRaw(char *f)
{
    // Prescribed filetype
    char *fileType = ".raw";

    // Loop through the last 4 characters to make sure that the input file is the correct type
    for (int i = (strlen(f) - strlen(fileType)), j = 0; i < strlen(f); i++, j++)
    {
        // If the type does not match, return false
        if (f[i] != fileType[j])
        {
            return false;
        }
    }
    return true;
}

bool isJPEG(BYTE *g)
{
    if (g[0] == 0xff && g[1] == 0xd8 && g[2] == 0xff && (g[3] & 0xf0) == 0xe0)
    {
        return true;
    }

    return false;
}
