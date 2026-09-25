#include <stdio.h>
#include <unistd.h>
#include <mraa/uart.h>

int main()
{
    mraa_uart_context uart;
    char tx_data;
    char rx_data;

    /* Initialize UART3 */
    uart = mraa_uart_init(3);

    if (uart == NULL)
    {
        printf("Unable to initialize UART3\n");
        return 1;
    }

    /* Set UART parameters */
    mraa_uart_set_baudrate(uart, 115200);
    mraa_uart_set_mode(uart, 8, MRAA_UART_PARITY_NONE, 1);

    printf("UART loopback started on /dev/ttyS3\n");
    printf("TX and RX must be connected together.\n");
    printf("Type something:\n");

    while (1)
    {
        /* Get keyboard input */
        tx_data = getchar();

        /* Send data through UART */
        mraa_uart_write(uart, &tx_data, 1);

        /* Wait until data is received */
        while (mraa_uart_data_available(uart, 1000) == 0)
        {
            /* Wait */
        }

        /* Read received data */
        mraa_uart_read(uart, &rx_data, 1);

        /* Display received data */
        printf("Received: %c\n", rx_data);
        fflush(stdout);
    }

    /* Close UART */
    mraa_uart_stop(uart);

    return 0;
}
