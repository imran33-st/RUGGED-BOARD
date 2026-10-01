#include <stdio.h>
#include <unistd.h>
#include "mraa.h"

#define AIO_PORT 6 

int main()
{
    mraa_aio_context mq135;
    int value;
    float voltage;

    if (mraa_init() != MRAA_SUCCESS)
    {
        printf("MRAA initialization failed\n");
        return 1;
    }

    mq135 = mraa_aio_init(AIO_PORT);

    if (mq135 == NULL)
    {
        printf("ADC initialization failed\n");
        return 1;
    }

    printf("MQ-135 ADC Reading\n");

    while (1)
    {
        value = mraa_aio_read(mq135);

        voltage = (value * 3.3) / 1023.0;

        printf("ADC Value = %d\tVoltage = %.2f V\n",
               value, voltage);

        sleep(1);
    }

    mraa_aio_close(mq135);
    mraa_deinit();

    return 0;
}
