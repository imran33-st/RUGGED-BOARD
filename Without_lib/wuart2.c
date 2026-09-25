#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int configure_uart(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) == -1)
    {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetispeed(&tty, speed);
    cfsetospeed(&tty, speed);

    /* UART configuration: 8 data bits, no parity, 1 stop bit */
    tty.c_cflag |= CREAD | CLOCAL;
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);

    /* Input and output settings */
    tty.c_iflag = IGNPAR;
    tty.c_lflag = 0;
    tty.c_oflag = 0;

    /* Read timeout */
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 10;

    if (tcsetattr(fd, TCSANOW, &tty) == -1)
    {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

int main()
{
    const char *portname = "/dev/ttyS3";
    int fd;
    ssize_t written, received;
    unsigned char buffer[100];
    char input[100];

    /* Open UART */
    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);

    if (fd == -1)
    {
        printf("Error opening %s: %s\n",
               portname, strerror(errno));
        return -1;
    }

    /* Configure UART */
    if (configure_uart(fd, B9600) == -1)
    {
        close(fd);
        return -1;
    }

    printf("Enter text to send via serial (press ENTER to send, Ctrl+D to quit):\n");

    while (fgets(input, sizeof(input), stdin) != NULL)
    {
        size_t length = strlen(input);

        /* Remove newline */
        if (length && input[length - 1] == '\n')
        {
            input[--length] = '\0';
        }

        if (length == 0)
        {
            printf("Empty input, please enter something.\n");
            continue;
        }

        /* Send data */
        written = write(fd, input, length);

        if (written != (ssize_t)length)
        {
            printf("Error from write: %ld, %s\n",
                   written, strerror(errno));
        }

        /* Receive loopback data */
        received = read(fd, buffer, sizeof(buffer) - 1);

        if (received > 0)
        {
            buffer[received] = '\0';
            printf("Received: %s\n", buffer);
        }
        else if (received < 0)
        {
            printf("Error from read: %s\n",
                   strerror(errno));
        }
    }

    close(fd);

    return 0;
}
