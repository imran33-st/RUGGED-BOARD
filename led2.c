#include <stdio.h>
#include <unistd.h>
#include <mraa/gpio.h>

int main()
{
    mraa_gpio_context sw;
    mraa_gpio_context led;

    int switch_count = 0;
    int switch_state;
    int previous_state = 0;

    // Initialize switch - MRAA pin 35
    sw = mraa_gpio_init(31);

    // Initialize onboard LED D4 - MRAA pin 61
    led = mraa_gpio_init(45);

    if (sw == NULL || led == NULL)
    {
        printf("GPIO initialization failed\n");
        return 1;
    }

    // Set directions
    mraa_gpio_dir(sw, MRAA_GPIO_IN);
    mraa_gpio_dir(led, MRAA_GPIO_OUT);

    // Initially LED OFF
    mraa_gpio_write(led, 0);

    printf("Switch and LED program started...\n");

    while (1)
    {
        switch_state = mraa_gpio_read(sw);

        // Detect a new button press
        if (switch_state == 1 && previous_state == 0)
        {
            switch_count++;
            printf("Switch pressed: %d time(s)\n", switch_count);

            // Toggle LED according to switch press count
            for (int i = 0; i < switch_count; i++)
            {
                mraa_gpio_write(led, 1);
                usleep(300000);

                mraa_gpio_write(led, 0);
                usleep(300000);
            }
        }

        previous_state = switch_state;

        usleep(50000);
    }

    mraa_gpio_close(sw);
    mraa_gpio_close(led);

    return 0;
}                     
