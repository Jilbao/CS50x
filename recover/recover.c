#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Can't read the file\n");
        return 1;
    }
    typedef uint8_t BYTE;
    BYTE blocks[512];
    char filename[15];
    FILE *img = NULL;
    int fileno = 0;

    while (fread(blocks, sizeof(BYTE), 512, file))
    {
        //.jpg header
        if (blocks[0] == 0xff && blocks[1] == 0xd8 && blocks[2] == 0xff && (blocks[3] & 0xf0) == 0xe0)
        {
            //If writing file already exist
            if (img != NULL)
            {
                fclose(img);
            }
            //Creating file and count +1 to "fileno" variable
            sprintf(filename, "%03i.jpg", fileno);
            img = fopen(filename, "w");
            fileno++;
        }
        //if file created, write to it
        if (img != NULL)
        {
            fwrite(blocks, sizeof(BYTE), 512, img);
        }

    }
    //if last file still exist
    if (img != NULL)
    {
        fclose(img);
    }
    fclose(file);
}