// Helper functions for music

#include <cs50.h>
#include "math.h"
#include "helpers.h"
#include "string.h"
#include "stdio.h"

// Converts a fraction formatted as X/Y to eighths by multiplying the numerator against denominator / 8
int duration(string fraction)
{
    string numerator = strtok(fraction, "/");
    string denominator = strtok(NULL, "/");
    int eighths = atoi(numerator) * (8 / atoi(denominator));
    return eighths;
}

// Calculates frequency (in Hz) of a note
// first identifies the right key then adjusts in steps of + or - 12 to match the octave to find the semitone
int frequency(string note)
{
    // define variables
    char *scale[13] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    char *sharp[11] = {"Db", "Eb", "Gb", "Ab", "Bb", "C#", "D#", "F#", "G#", "A#"};
    char key[3] = "";
    int octave;
    int n;

    // check for # or b note and create key and octave accordingly
    if (note[1] == '#' || note[1] == 'b')
    {
        strncpy(key, note, 2);
        octave = note[2] - '0';
        // if b, convert b to # note (as they are the same)
        if (note[1] == 'b')
        {
            for (int s = 0; s < 5; s++)
            {
                if (strcmp(key, sharp[s]) == 0)
                {
                    strncpy(key, sharp[s + 5], 2);
                }
            }
        }
    }
    else
    {
        strncpy(key, note, 1);
        octave = note[1] - '0';
    }

    for (int i = 0; i < 12; i++)
    {
        if (strcmp(key, scale[i]) == 0)
        {
            n = i - 9;
        }
    }
    // if you divide with two integers in C you get an integer (i.e truncated, 0) response - at least one number must be a float
    float power = (n + (octave - 4) * 12) / 12.0;
    int frequency = round(pow(2, power) * 440);
    return frequency;
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    if (strcmp(s, "") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
