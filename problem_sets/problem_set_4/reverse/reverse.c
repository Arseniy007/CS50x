#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "wav.h"

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    char *input_file = argv[1];
    FILE *input = fopen(input_file, "r");

    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Read header
    WAVHEADER header;
    fread(&header, HEADER_SIZE, 1, input);

    // Use check_format to ensure WAV format
    if (check_format(header) != 0)
    {
        printf("Input is not a WAV file.\n");
        return 1;
    }

    // Open output file for writing
    char *output_file = argv[2];
    FILE *output = fopen(output_file, "w");

    if (output == NULL)
    {
        printf("Could not open output file.\n");
        return 1;
    }

    // Write header to file
    fwrite(&header, HEADER_SIZE, 1, output);

    // Use get_block_size to calculate size of block
    int block_size = get_block_size(header);

    BYTE buffer[block_size];

    fseek(input, block_size, SEEK_END);

    // Read backwards through input file and write to our output file
    while (ftell(input) - block_size > HEADER_SIZE)
    {
        fseek(input, -2 * block_size, SEEK_CUR);

        fread(buffer, block_size, 1, input);
        fwrite(buffer, block_size, 1, output);
    }

    // Close both files
    fclose(input);
    fclose(output);

    return 0;
}

int check_format(WAVHEADER header)
{
    char *right_format = "WAVE";
    int length_of_word = 4;

    for (int i = 0; i < length_of_word; i++)
    {
        if (right_format[i] != header.format[i])
        {
            return 1;
        }
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    int block_size = header.numChannels * (header.bitsPerSample / 8);
    return block_size;
}