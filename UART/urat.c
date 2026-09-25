#include <stdio.h>
#include <mraa/uart.h>

int main()
{
    mraa_uart_context uart;
    char data;
    char message[100];
    int index = 0;

    uart = mraa_uart_init(3);

    if (uart == NULL)
    {
        printf("Unable to initialize UART3\n");
        return 1;
    }

    mraa_uart_set_baudrate(uart, 115200);
    mraa_uart_set_mode(uart, 8, MRAA_UART_PARITY_NONE, 1);

    printf("Receiver started\n");
    printf("Waiting for message...\n");

    while (1)
    {
        if (mraa_uart_data_available(uart, 1000) > 0)
        {
            int ret = mraa_uart_read(uart, &data, 1);

            if (ret > 0)
            {
                if (data == '\n')
                {
                    message[index] = '\0';

                    printf("Received: %s\n", message);

                    index = 0;
                }
                else
                {
                    if (index < 99)
                    {
                        message[index++] = data;
                    }
                    else
                    {
                        message[index] = '\0';
                        printf("Received: %s\n", message);
                        index = 0;
                    }
                }
            }
        }
    }

    mraa_uart_stop(uart);

    return 0;
}
