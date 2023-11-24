#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
int main(int argc, char *argv[])
{


    FILE *in = fopen(argv[1], "r");
    if (in == NULL)
    {
        printf("Could not open %s.\n", argv[2]);
        return 1;
    }

    //fread(<buffer>, <size>, <qty>, <file pointer>);
    BYTE r[1];
    int n = 0;
    char x[8];
    sprintf(x, "%03i.jpg", n);
    FILE *out = fopen(x, "w");

    while (1)
    {
        fread(r, 1, 1, in);
        if (r[0] == 0xff)
        {
            fread(r, 1, 1, in);
            if (r[0] == 0xd8)
            {
                fread(r, 1, 1, in);
                if (r[0] == 0xff)
                {
                    fread(r, 1, 1, in);
                    if (r[0] <= 0xef && r[0] >= 0xe0)
                    {
                        fputc(0xff, out);
                        fputc(0xd8, out);
                        fputc(0xff, out);
                        fwrite(r, 1, 1, out);
                        //fwrite(<buffer>, <size>, <qty>, <file pointer>);

                        break;
                    }
                }
            }
        }
    }
    //first jpg here


    BYTE buffer[512];
    fread(buffer, 1, 508, in);
    fwrite(buffer, 1, 508, out);
    //fwrite(<buffer>, <size>, <qty>, <file pointer>)
    while (fread(buffer, 1, 512, in) == 512)
    {
        //if new jpg start new file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] <= 0xef && buffer[3] >= 0xe0)
        {
            fclose(out);
            n++;
            sprintf(x, "%03i.jpg", n);
            out = fopen(x, "w");
            printf("%i", n);
        }

        fwrite(buffer, 1, 512, out);
    }

    fclose(out);


    fclose(in);
}
