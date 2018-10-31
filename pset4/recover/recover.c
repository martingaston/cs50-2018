#include <stdio.h>  // printf, fprintf and sprintf
#include <stdlib.h> // malloc
#include <stdint.h> // this is super important - standard int is probably 4 bytes, and a *signed* integer (supporting negative numbers) messes everything up as well :D

int main(int argc, char *argv[])
{
    // ensure command line argument was passed
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover [file]\n");
        return 1;
    }

    // open memory card file
    char *infile = argv[1];
    FILE *input = fopen(infile, "r");
    if (input == NULL)
    {
        fprintf(stderr, "Could not open file %s. Exiting.\n", argv[1]);
        return 2;
    }

    uint8_t *buffer = malloc(512 * sizeof(uint8_t));
    // error out if memory assignment for buffer fails
    if (buffer == NULL)
    {
        fprintf(stderr, "Could not assign memory for file %s. Exiting.\n", argv[1]);
        return 3;
    }

    // define variables. outfile only seems to work in this way if called with malloc - not sure why!
    int found = 0;
    char *ext = "jpg";
    char *outfile = malloc(7 * sizeof(char));
    FILE *output;

    // scan through the file until it hits a chunk that's less than 512 bytes, indicating end of file
    while (fread(buffer, 1, 512 * sizeof(*buffer), input) == 512)
    {
        // use bit masking (making 0xe0) to ensure ffd8ff magic numbers are matched with valid fourth byte
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {

            // if this is the first file found open the first file, else close old file and start new one
            if (found == 0)
            {
                sprintf(outfile, "%03i.jpg", found);
                output = fopen(outfile, "w");
            }
            else
            {
                fclose(output);
                sprintf(outfile, "%03i.jpg", found);
                output = fopen(outfile, "w");
            }
            found += 1;
        }

        // write data to output files provided we are have found an original file (assuming jpg data is back-to-back)
        if (found > 0)
        {
            fwrite(buffer, 512 * sizeof(*buffer), 1, output);
        }
    }

    // close files and free memory
    fclose(output);
    fclose(input);
    free(buffer);
    free(outfile);

    // success!
    return 0;
}