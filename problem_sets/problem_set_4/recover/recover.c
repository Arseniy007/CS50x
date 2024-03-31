#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Check for right command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open a file
    char *filename = argv[1];

    FILE *file = fopen(filename, "r");

    // Check the file
    if (file == NULL)
    {
        printf("No such file found\n");
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];

    char *name_of_file = malloc(8);

    FILE *jpeg = NULL;

    int number_of_photo = 0;

    // Reading every block in the file
    while (fread(buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {

            // Close the file if it is not the first image
            if (number_of_photo != 0)
            {
                fclose(jpeg);
            }

            // Creat the new file
            sprintf(name_of_file, "%03i.jpg", number_of_photo);
            jpeg = fopen(name_of_file, "w");
            number_of_photo++;
        }

        // Write to the image-file
        if (number_of_photo > 0)
        {
            fwrite(buffer, 1, BLOCK_SIZE, jpeg);
        }
    }

    // Close the image-file
    if (jpeg != NULL)
    {
        fclose(jpeg);
    }

    // Close file and free the sting
    fclose(file);
    free(name_of_file);

    return 0;
}