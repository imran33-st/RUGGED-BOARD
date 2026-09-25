#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mraa/uart.h>

int main()
{
    mraa_uart_context uart;
    char tx_data;
    char rx_data;
    char rx_buffer[100];
    int index = 0;

    uart = mraa_uart_init(3);

    if (uart == NULL)
    {
        printf("Unable to initialize UART3\n");
        return 1;
    }

    mraa_uart_set_baudrate(uart, 115200);
    mraa_uart_set_mode(uart, 8, MRAA_UART_PARITY_NONE, 1);

    printf("UART loopback started on /dev/ttyS3\n");
    printf("Enter a word and press Enter:\n");

    while (1)
    {
        /* Get keyboard character */
        tx_data = getchar();

        /* Send character through UART */
        mraa_uart_write(uart, &tx_data, 1);

        /* Wait for received character */
        while (mraa_uart_data_available(uart, 1000) == 0)
        {
        }

        /* Read received character */
        mraa_uart_read(uart, &rx_data, 1);

        /* Store received character */
        if (rx_data == '\n')
        {
            rx_buffer[index] = '\0';

            printf("Received: %s\n", rx_buffer);

            index = 0;
        }
        else if (rx_data != '\r')
        {
            if (index < 99)
            {
                rx_buffer[index] = rx_data;
                index++;
            }
        }
    }

    mraa_uart_stop(uart);

    return 0;
}
