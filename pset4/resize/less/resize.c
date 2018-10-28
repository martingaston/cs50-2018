#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    // OPEN THE FILES

    // assign factor, infile and outfile to arguments supplied
    int factor = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

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

    // calculate new width, height and padding

    int originalWidth = bi.biWidth;
    bi.biWidth = bi.biWidth * factor;

    int originalHeight = bi.biHeight;
    bi.biHeight = bi.biHeight * factor;

    int originalPadding = (4 - (originalWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int resizePadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // calculate new biSizeImage and bfSize

    bi.biSizeImage = bi.biWidth * bi.biHeight * sizeof(RGBTRIPLE) + resizePadding * sizeof(RGBTRIPLE);
    bf.bfSize = bi.biSizeImage + bf.bfOffBits;

    // write outfile's BITMAPFILEHEADER and BITMAPINFOHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over scanline (rows of pixels)
    // abs gets the absolute value of an integer - is there a reason we have to check here? and not for biWidth?
    int seek = (originalWidth * sizeof(RGBTRIPLE) + originalPadding) * -1;
    for (int i = 0, biHeight = abs(originalHeight); i < biHeight; i++)
    {
        for (int fH = 0; fH < factor; fH++)
        {
            // rewind our source image back to the start of a scanline if we're repeating >0 on the height
            if (fH > 0)
            {
                fseek(inptr, seek, SEEK_CUR);
            }
            // iterate over pixels in scanline (columns)
            for (int j = 0; j < originalWidth; j++)
            {
                // declare triple to be a RGBTRIPLE struct
                RGBTRIPLE triple;

                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // repeat pixels by our factor (so a 3 pixel original scanline would now be a 9 pixel scanline for a factor of 3)
                // note that we are doing no interpolation here - image editors do things like linear, cubic and lanczos3 for this
                for (int fW = 0; fW < factor; fW++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // skip over any original padding from source file
            fseek(inptr, originalPadding, SEEK_CUR);

            // add in padding needed for resized file
            for (int k = 0; k < resizePadding; k++)
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