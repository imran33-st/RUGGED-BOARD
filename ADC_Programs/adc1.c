#include <stdio.h>
#include <unistd.h>
#include <mraa/aio.h>
#include <mraa/gpio.h>

#define AIO_PIN 6

#define LED1 61
#define LED2 62
#define LED3 63

int main()
{
    mraa_aio_context pot;
    mraa_gpio_context led1, led2, led3;

    int value;

   
    pot = mraa_aio_init(AIO_PIN);

  
    led1 = mraa_gpio_init(LED1);
    led2 = mraa_gpio_init(LED2);
    led3 = mraa_gpio_init(LED3);

    if (pot == NULL || led1 == NULL || led2 == NULL || led3 == NULL)
    {
        printf("Initialization failed\n");
        return 1;
    }

   
    mraa_gpio_dir(led1, MRAA_GPIO_OUT);
    mraa_gpio_dir(led2, MRAA_GPIO_OUT);
    mraa_gpio_dir(led3, MRAA_GPIO_OUT);

    while (1)
    {
        value = mraa_aio_read(pot);

        printf("ADC Value : %d\n", value);

        if (value < 20)
        {
            mraa_gpio_write(led1, 0);
            mraa_gpio_write(led2, 1);
            mraa_gpio_write(led3, 1);
        }
        else if (value < 30)
        {
            mraa_gpio_write(led1, 1);
            mraa_gpio_write(led2, 0);
            mraa_gpio_write(led3, 1);
        }
        else
        {
            mraa_gpio_write(led1, 1);
            mraa_gpio_write(led2, 1);
            mraa_gpio_write(led3, 0);
        }

        sleep(1);
    }

    return 0;
}
