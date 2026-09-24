#include <stdio.h>
#include <mraa/gpio.h>
#include <unistd.h>

int main()
{
    mraa_gpio_context led1;
    mraa_gpio_context led2;
    mraa_gpio_context led3;
    mraa_gpio_context button;

    int count = 0;
    int state;
    int previous_state = 1;

    /*
     * IMPORTANT:
     * Replace these numbers with the MRAA pin numbers
     * corresponding to PC13, PC17, PC19 and PC8.
     */
    led1 = mraa_gpio_init(61);
    led2 = mraa_gpio_init(62);
    led3 = mraa_gpio_init(63);
    button = mraa_gpio_init(35);

    if (led1 == NULL || led2 == NULL ||
        led3 == NULL || button == NULL)
    {
        printf("GPIO initialization failed\n");
        return 1;
    }

    mraa_gpio_dir(led1, MRAA_GPIO_OUT);
    mraa_gpio_dir(led2, MRAA_GPIO_OUT);
    mraa_gpio_dir(led3, MRAA_GPIO_OUT);
    mraa_gpio_dir(button, MRAA_GPIO_IN);

    mraa_gpio_write(led1, 0);
    mraa_gpio_write(led2, 0);
    mraa_gpio_write(led3, 0);

    printf("3 LED + 1 SWITCH Program Started\n");
    printf("PC13 = LED1\n");
    printf("PC17 = LED2\n");
    printf("PC19 = LED3\n");
    printf("PC8  = SWITCH\n");

    while (1)
    {
        state = mraa_gpio_read(button);

        /* Switch pressed */
        if (state == 0 && previous_state == 1)
        {
            count++;

            printf("Switch pressed: %d time(s)\n", count);

            /* Initially turn all LEDs OFF */
            mraa_gpio_write(led1, 1);
            mraa_gpio_write(led2, 1);
            mraa_gpio_write(led3, 1);

            if (count == 1)
            {
                mraa_gpio_write(led1, 0);
                printf("LED1 ON\n");
            }
            else if (count == 2)
            {
                mraa_gpio_write(led2, 0);
                printf("LED2 ON\n");
            }
            else if (count == 3)
            {
                mraa_gpio_write(led3, 0);
                printf("LED3 ON\n");
            }
            else
            {
		mraa_gpio_write(led1, 0);
            	mraa_gpio_write(led2, 0);
            	mraa_gpio_write(led3, 0);
                printf("3 presses completed - resetting count\n");
                count = 0;
            }

            usleep(200000);
        }

        /* Switch released -> all LEDs OFF */
        if (state == 1)
        {
            mraa_gpio_write(led1, 1);
            mraa_gpio_write(led2, 1);
            mraa_gpio_write(led3, 1);
        }

        previous_state = state;

        usleep(10000);
    }

    return 0;
}

