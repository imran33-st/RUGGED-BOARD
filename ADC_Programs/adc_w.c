#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    FILE *fp;
    int value;

    while (1)
    {
        fp = fopen("/sys/bus/iio/devices/iio:device0/in_voltage6_raw", "r");

        if (fp == NULL)
        {
            perror("Error opening ADC");
            return 1;
        }

        fscanf(fp, "%d", &value);

        fclose(fp);

        printf("ADC Value : %d\n", value);

        sleep(2);
    }

    return 0;
}
