#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open forensic image
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];
    FILE *img = NULL;
    char filename[8];
    int file_count = 0;

    // Read 512-byte blocks until the end of the file
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, file))
    {
        // Check if block contains a JPEG signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // Close previous file if it's open
            if (img != NULL)
            {
                fclose(img);
            }

            // Create a new filename for the JPEG
            sprintf(filename, "%03i.jpg", file_count);
            img = fopen(filename, "w");
            file_count++;
        }

        // Write data to the JPEG file if it's open
        if (img != NULL)
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, img);
        }
    }

    // Close last JPEG file if open
    if (img != NULL)
    {
        fclose(img);
    }

    // Close the forensic image file
    fclose(file);

    return 0;
}
