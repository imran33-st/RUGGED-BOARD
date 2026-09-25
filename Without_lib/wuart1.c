#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>

#define UART_DEVICE "/dev/ttyS0"

#define GPIO_EXPORT    "/sys/class/gpio/export"
#define GPIO_DIRECTION "/sys/class/gpio/gpio37/direction"
#define GPIO_VALUE     "/sys/class/gpio/gpio37/value"

int main()
{
    int uart_fd;
    int gpio_fd;
    char data;
    struct termios uart_config;

    /* ================= UART ================= */

    uart_fd = open(UART_DEVICE, O_RDWR | O_NOCTTY);

    if (uart_fd < 0)
    {
        perror("UART open failed");
        return 1;
    }

    /* Get current UART settings */
    if (tcgetattr(uart_fd, &uart_config) < 0)
    {
        perror("tcgetattr failed");
        close(uart_fd);
        return 1;
    }

    /* 9600 baud */
    cfsetispeed(&uart_config, B9600);
    cfsetospeed(&uart_config, B9600);

    /* 8 data bits */
    uart_config.c_cflag &= ~CSIZE;
    uart_config.c_cflag |= CS8;

    /* No parity */
    uart_config.c_cflag &= ~PARENB;

    /* 1 stop bit */
    uart_config.c_cflag &= ~CSTOPB;

    /* Enable receiver */
    uart_config.c_cflag |= CREAD;

    /* Ignore modem control lines */
    uart_config.c_cflag |= CLOCAL;

    /* Raw input */
    uart_config.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /* Disable software flow control */
    uart_config.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* Raw output */
    uart_config.c_oflag &= ~OPOST;

    /* Wait for at least 1 character */
    uart_config.c_cc[VMIN] = 1;
    uart_config.c_cc[VTIME] = 0;

    if (tcsetattr(uart_fd, TCSANOW, &uart_config) < 0)
    {
        perror("tcsetattr failed");
        close(uart_fd);
        return 1;
    }

    printf("UART /dev/ttyS3 started\n");


    /* ================= GPIO ================= */

    /* Export GPIO 61 */
    gpio_fd = open(GPIO_EXPORT, O_WRONLY);

    if (gpio_fd < 0)
    {
        perror("GPIO export open failed");
        close(uart_fd);
        return 1;
    }

    if (write(gpio_fd, "61", 2) < 0)
    {
        /*
         * If GPIO 61 is already exported, continue.
         */
        if (errno != EBUSY)
        {
            perror("GPIO export failed");
            close(gpio_fd);
            close(uart_fd);
            return 1;
        }
    }

    close(gpio_fd);


    /* Set GPIO 61 as output */
    gpio_fd = open(GPIO_DIRECTION, O_WRONLY);

    if (gpio_fd < 0)
    {
        perror("GPIO direction open failed");
        close(uart_fd);
        return 1;
    }

    if (write(gpio_fd, "out", 3) < 0)
    {
        perror("GPIO direction failed");
        close(gpio_fd);
        close(uart_fd);
        return 1;
    }

    close(gpio_fd);


    /* Initially LED OFF */
    gpio_fd = open(GPIO_VALUE, O_WRONLY);

    if (gpio_fd < 0)
    {
        perror("GPIO value open failed");
        close(uart_fd);
        return 1;
    }

    write(gpio_fd, "0", 1);
    close(gpio_fd);

    printf("GPIO 61 configured as OUTPUT\n");
    printf("Waiting for N or F...\n");


    /* ================= MAIN LOOP ================= */

    while (1)
    {
        int ret = read(uart_fd, &data, 1);

        if (ret < 0)
        {
            perror("UART read failed");
            break;
        }

        if (ret == 1)
        {
            printf("Received command: %c\n", data);

            /* N = LED ON */
            if (data == 'N')
            {
                gpio_fd = open(GPIO_VALUE, O_WRONLY);

                if (gpio_fd < 0)
                {
                    perror("GPIO open failed");
                    continue;
                }

                write(gpio_fd, "1", 1);
                close(gpio_fd);

                printf("LED ON\n");
            }

            /* F = LED OFF */
            else if (data == 'F')
            {
                gpio_fd = open(GPIO_VALUE, O_WRONLY);

                if (gpio_fd < 0)
                {
                    perror("GPIO open failed");
                    continue;
                }

                write(gpio_fd, "0", 1);
                close(gpio_fd);

                printf("LED OFF\n");
            }

            else
            {
                printf("Invalid command: %c\n", data);
            }

            fflush(stdout);
        }
    }

    close(uart_fd);

    return 0;
}
