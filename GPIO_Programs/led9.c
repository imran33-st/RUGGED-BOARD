#include <stdio.h>
#include <unistd.h>
#include <mraa/gpio.h>

int main()
{
    mraa_gpio_context sw;
    mraa_gpio_context led1;
    mraa_gpio_context led2;
    mraa_gpio_context led3;

    int switch_count = 0;
    int switch_state;
    int previous_state = 1;

    // Initialize switch - MRAA pin 35
    sw = mraa_gpio_init(31);

    // Initialize LEDs
    led1 = mraa_gpio_init(45);
    led2 = mraa_gpio_init(46);
    led3 = mraa_gpio_init(37);

    // Check GPIO initialization
    if (sw == NULL || led1 == NULL || led2 == NULL || led3 == NULL)
    {
        printf("GPIO initialization failed\n");
        return 1;
    }

    // Set GPIO directions
    mraa_gpio_dir(sw, MRAA_GPIO_IN);

    mraa_gpio_dir(led1, MRAA_GPIO_OUT);
    mraa_gpio_dir(led2, MRAA_GPIO_OUT);
    mraa_gpio_dir(led3, MRAA_GPIO_OUT);

    /*
     * Active-low LEDs:
     * 1 = OFF
     * 0 = ON
     */

    // Initially OFF
    mraa_gpio_write(led1, 0);
    mraa_gpio_write(led2, 0);
    mraa_gpio_write(led3, 0);

    printf("Switch and LED program started...\n");

    while (1)
    {
        switch_state = mraa_gpio_read(sw);

        /*
         * Detect button press
         * Assuming:
         * 1 = pressed
         * 0 = released
         */
        if (switch_state == 1 && previous_state == 0)
        {
            switch_count++;

            printf("Switch pressed: %d time(s)\n", switch_count);

            // 1st press -> LED1 blinks 3 times
            if (switch_count == 1)
            {
                for (int i = 0; i < 3; i++)
                {
                    mraa_gpio_write(led1, 1);  // ON
                    usleep(300000);

                    mraa_gpio_write(led1, 0);  // OFF
                    usleep(300000);
                }
            }

            // 2nd press -> LED2 blinks 6 times
            else if (switch_count == 2)
            {
                for (int i = 0; i < 6; i++)
                {
                    mraa_gpio_write(led2, 1);  // ON
                    usleep(300000);

                    mraa_gpio_write(led2, 0);  // OFF
                    usleep(300000);
                }
            }

            // 3rd press -> LED3 blinks 9 times
            else if (switch_count == 3)
            {
                for (int i = 0; i < 9; i++)
                {
                    mraa_gpio_write(led3, 1);  // ON
                    usleep(300000);

                    mraa_gpio_write(led3, 0);  // OFF
                    usleep(300000);
                }

                // Reset count after 3rd press
                switch_count = 0;

                printf("Sequence completed. Counter reset.\n");
            }

            // Wait until button is released
            while (mraa_gpio_read(sw) == 1)
            {
                usleep(10000);
            }
        }

        previous_state = switch_state;

        usleep(50000);
    }

    mraa_gpio_close(sw);
    mraa_gpio_close(led1);
    mraa_gpio_close(led2);
    mraa_gpio_close(led3);

    return 0;
}
