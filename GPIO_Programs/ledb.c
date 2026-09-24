#include <stdio.h>
#include <mraa/gpio.h>

int main()
{
    mraa_gpio_context switch_pin;
    mraa_gpio_context led_pin;

    switch_pin = mraa_gpio_init(35);
    led_pin = mraa_gpio_init(62);

    mraa_gpio_dir(switch_pin, MRAA_GPIO_IN);
    mraa_gpio_dir(led_pin, MRAA_GPIO_OUT);

    while(1)
    {
        if(mraa_gpio_read(switch_pin))
        {
            mraa_gpio_write(led_pin, 1);
        }
        else
        {
            mraa_gpio_write(led_pin, 0);
        }
    }

    return 0;
}

