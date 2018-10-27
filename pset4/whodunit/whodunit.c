#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: whodunit infile outfile\n");
        return 1;
    }

    // OPEN THE FILES

    // assign infile and outfile to arguments supplied
    char *infile = argv[1];
    char *outfile = argv[2];

    // open the input file
    // the FILE struct pointer is an opaque data type we use to handle file manipulation
    // fopen takes the char *filename and a char *mode and returns a pointer or NULL
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open/create the output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // READ BITMAP INFORMATION

    BITMAPFILEHEADER bf; // declare bf as a BITMAPFILEHEADER data struct
    // fread reads 1 item of size BITMAPFILEHEADER data from inptr and stores it at &bf
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // check the file is a 24-bit uncompressed BMP 4.0 - error if not
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER and BITMAPINFOHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // i = bi.biWidth * RGBTRIPLE mod 4
    // j = 4 - i
    // k = j mod 4
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over scanline (rows of pixels)
    // abs gets the absolute value of an integer - is there a reason we have to check here? and not for biWidth?
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline (columns)
        for (int j = 0; j < bi.biWidth; j++)
        {
            // declare triple to be a RGBTRIPLE struct
            RGBTRIPLE triple;

            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            if (triple.rgbtRed == 0xff && triple.rgbtGreen == 0x00 && triple.rgbtBlue == 0x00)
            {
                triple.rgbtRed = 0xff;
                triple.rgbtGreen = 0xff;
                triple.rgbtBlue = 0xff;
            }

            if (triple.rgbtRed == 0xe0 && triple.rgbtGreen == 0xff && triple.rgbtBlue == 0xff)
            {
                triple.rgbtRed = 0x00;
                triple.rgbtGreen = 0x00;
                triple.rgbtBlue = 0x00;
            }

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
        }
    }

    // close files & success
    fclose(inptr);
    fclose(outptr);
    return 0;
}