#include <stdio.h>
#include <unistd.h>
#include <mraa/gpio.h>

int main()
{
    mraa_gpio_context switch_pin;
    mraa_gpio_context led_pin;

    int switch_state;
    int previous_state = -1;

    switch_pin = mraa_gpio_init(31);
    led_pin = mraa_gpio_init(45);

    if (switch_pin == NULL || led_pin == NULL)
    {
        printf("GPIO initialization failed\n");
        return 1;
    }

    mraa_gpio_dir(switch_pin, MRAA_GPIO_IN);
    mraa_gpio_dir(led_pin, MRAA_GPIO_OUT);

    // LED OFF initially
    mraa_gpio_write(led_pin, 1);

    while (1)
    {
        switch_state = mraa_gpio_read(switch_pin);

        // Print only when switch status changes
        if (switch_state != previous_state)
        {
            if (switch_state == 1)
            {
                // Switch ON -> LED ON
                mraa_gpio_write(led_pin, 0);
                printf("LED is OFF\n");
            }
            else
            {
                // Switch OFF -> LED OFF
                mraa_gpio_write(led_pin, 1);
                printf("LED is ON\n");
            }

            previous_state = switch_state;
        }

        usleep(100000);
    }

    mraa_gpio_close(switch_pin);
    mraa_gpio_close(led_pin);

    return 0;
}
