// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cs50.h>
#include <string.h>


#include "bmp.h"

// Function to verify a valid range is used
bool intRange(char *range);

// Function to verify that BMP files are used
bool isBMP(char *file);

int main(int argc, char *argv[])
{
    // Validate proper usage case.
    if (argc != 4 || intRange(argv[1]) == false || isBMP(argv[2]) == false || isBMP(argv[3]) == false)
    {
        printf("Usage: resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // Store muiltiplier
    int mult = atoi(argv[1]);

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Save the original height, width, and padding
    int inPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int inBiWidth = bi.biWidth;
    int inBiHeight = bi.biHeight;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Make adjustments to the BITMAPFILEHEADER and BITMAPINFOHEADER data
    bi.biWidth *= mult;
    bi.biHeight *= mult;
    int outPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + outPadding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(inBiHeight); i < biHeight; i++)
    {
        // Used to keep track of array placement
        int marker = 0;

        // Used to determine the array size
        int size = (bi.biWidth);

        // Array for holding RGBTRIPLE values
        RGBTRIPLE *copy[size];

        // iterate over pixels in scanline
        for (int j = 0; j < inBiWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // Write to the outfile based on the multiplier
            for (int k = 0; k < mult; k++)
            {
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

            }

            // Set the infile to the previous value
            fseek(inptr, -(sizeof(RGBTRIPLE)), SEEK_CUR);

            // Read the value into the array
            fread(&copy[marker], sizeof(RGBTRIPLE), 1, inptr);

            // Update the marker
            marker++;
        }

        if (inPadding > 0)
        {
            // skip over padding, if any
            fseek(inptr, inPadding, SEEK_CUR);
        }

        // Add in any padding to the row
        for (int k = 0; k < outPadding; k++)
        {
            fputc(0x00, outptr);
        }

        // Print additional rows
        for (int k = 0; k < mult - 1; k++)
        {
            for (int l = 0; l < inBiWidth; l++)
            {
                marker = l;

                for (int n = 0; n < mult; n++)
                {
                    fwrite(&copy[marker], (sizeof(RGBTRIPLE)), 1, outptr);
                }
            }

            for (int l = 0; l < outPadding; l++)
            {
                fputc(0x00, outptr);
            }

        }
        // Reset the marker
        marker = 0;
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

// Used to verify the correct range.
bool intRange(char *range)
{

    // Validate the input.
    for (int i = 0; i < strlen(range); i++)
    {
        if (isalpha(range[i]) != false)
        {
            printf("Fail from part a of intRange.\n");
            return false;
        }
    }

    // Convert the value to an int.
    int num = atoi(range);

    // Check the range of the int.
    if (num < 1 || num > 100)
    {
        printf("Fail from part b of intRange.\n");
        return false;
    }

    return true;
}

// Used to verify if the user is using a bmp file.
bool isBMP(char *file)
{
    // Variables
    int length = strlen(file);
    int filename = length - 4;
    char *valid = ".bmp";

    // Iterate through to validate that the last characters are ".bmp".
    for (int i = filename, j = 0; i < length; i++, j++)
    {
        // Compare the ending of the argument.
        if (file[i] != valid[j])
        {
            printf("Fail from isBMP.\n");
            return false;
        }
    }

    return true;
}
