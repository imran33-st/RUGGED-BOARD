#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <mraa/uart.h>
#include <mraa/gpio.h>

#define UART_DEVICE "/dev/ttyS0"
#define LED_GPIO 41
#define BAUD_RATE 115200

int main()
{
    mraa_uart_context uart;
    mraa_gpio_context led;
    char buffer[100];
    int n;

    /* Initialize UART */
    uart = mraa_uart_init_raw(UART_DEVICE);

    if (uart == NULL) {
        fprintf(stderr, "Failed to initialize UART\n");
        return 1;
    }

    /* Configure UART */
    if (mraa_uart_set_baudrate(uart, BAUD_RATE) != MRAA_SUCCESS) {
        fprintf(stderr, "Failed to set baud rate\n");
        mraa_uart_stop(uart);
        return 1;
    }

    if (mraa_uart_set_mode(uart, 8,
                            MRAA_UART_PARITY_NONE, 1) != MRAA_SUCCESS) {
        fprintf(stderr, "Failed to set UART mode\n");
        mraa_uart_stop(uart);
        return 1;
    }

    /* Disable flow control */
    mraa_uart_set_flowcontrol(uart, 0, 0);

    /* Initialize GPIO 61 */
    led = mraa_gpio_init(LED_GPIO);

    if (led == NULL) {
        fprintf(stderr, "Failed to initialize GPIO %d\n", LED_GPIO);
        mraa_uart_stop(uart);
        return 1;
    }

    /* Set GPIO as output */
    mraa_gpio_dir(led, MRAA_GPIO_OUT);

    /* Initially LED OFF */
    mraa_gpio_write(led, 0);

    printf("UART: %s\n", UART_DEVICE);
    printf("Baud rate: %d\n", BAUD_RATE);
    printf("LED GPIO: %d\n", LED_GPIO);
    printf("Waiting for commands...\n");
    printf("N = LED ON\n");
    printf("F = LED OFF\n");

    while (1)
    {
        /* Check whether UART data is available */
        if (mraa_uart_data_available(uart, 1000))
        {
            /* Read received data */
            n = mraa_uart_read(uart, buffer, sizeof(buffer) - 1);

            if (n > 0)
            {
                buffer[n] = '\0';

                /* Process each received character */
                for (int i = 0; i < n; i++)
                {
                    if (buffer[i] == 'N' || buffer[i] == 'n')
                    {
                        mraa_gpio_write(led, 1);

                        printf("Received command: N\n");
                        printf("LED: ON\n");
                    }
                    else if (buffer[i] == 'F' || buffer[i] == 'f')
                    {
                        mraa_gpio_write(led, 0);

                        printf("Received command: F\n");
                        printf("LED: OFF\n");
                    }
                }
            }
        }
    }

    /* Cleanup */
    mraa_gpio_write(led, 0);
    mraa_gpio_close(led);
    mraa_uart_stop(uart);

    return 0;
}
