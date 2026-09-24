#include <stdio.h>
#include <mraa/gpio.h>
#include <unistd.h>

int main()
{
    mraa_gpio_context led1, led2, led3;
    mraa_gpio_context sw1, sw2, sw3;

    int old1 = 1;
    int old2 = 1;
    int old3 = 1;

    led1 = mraa_gpio_init(45);
    led2 = mraa_gpio_init(46);
    led3 = mraa_gpio_init(37);

    sw1 = mraa_gpio_init(31);
    sw2 = mraa_gpio_init(32);
    sw3 = mraa_gpio_init(13);
   

    if (led1 == NULL || led2 == NULL || led3 == NULL ||
        sw1 == NULL || sw2 == NULL || sw3 == NULL)
    {
        printf("GPIO initialization failed\n");
        return 1;
    }

    mraa_gpio_dir(led1, MRAA_GPIO_OUT);
    mraa_gpio_dir(led2, MRAA_GPIO_OUT);
    mraa_gpio_dir(led3, MRAA_GPIO_OUT);

    mraa_gpio_dir(sw1, MRAA_GPIO_IN);
    mraa_gpio_dir(sw2, MRAA_GPIO_IN);
    mraa_gpio_dir(sw3, MRAA_GPIO_IN);

    mraa_gpio_write(led1, 0);
    mraa_gpio_write(led2, 0);
    mraa_gpio_write(led3, 0);

    printf("3 LED + 3 SWITCH Program Started\n");

    while (1)
    {
        /* Switch 1 -> LED 1 */
        int state1 = mraa_gpio_read(sw1);

        if (state1 == 0)
        {
            mraa_gpio_write(led1, 1);
            printf("Switch 1 pressed - LED 1 ON\n");

            usleep(500000);

            mraa_gpio_write(led1, 0);
            printf("LED 1 OFF\n");

            while (mraa_gpio_read(sw1) == 0)
                usleep(10000);
        }

        old1 = state1;


        /* Switch 2 -> LED 2 */
        int state2 = mraa_gpio_read(sw2);

        if (state2 == 0)
        {
            mraa_gpio_write(led2, 1);
            printf("Switch 2 pressed - LED 2 ON\n");

            usleep(500000);

            mraa_gpio_write(led2, 0);
            printf("LED 2 OFF\n");

            while (mraa_gpio_read(sw2) == 0)
                usleep(10000);
        }

        old2 = state2;


        /* Switch 3 -> LED 3 */
        int state3 = mraa_gpio_read(sw3);

        if (state3 == 0)
        {
            mraa_gpio_write(led3, 1);
            printf("Switch 3 pressed - LED 3 ON\n");

            usleep(500000);

            mraa_gpio_write(led3, 0);
            printf("LED 3 OFF\n");

            while (mraa_gpio_read(sw3) == 0)
                usleep(10000);
        }

        old3 = state3;

        usleep(10000);
    }

    return 0;
}
