#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

int main()
{
    int fd;
    uint8_t tx[3] = {100, 123, 75};
    uint8_t rx[3];

    fd = open("/dev/spidev3.0", O_RDWR);

    if (fd < 0)
    {
        perror("Failed to open SPI");
        return 1;
    }

    uint8_t mode = SPI_MODE_0;

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
    {
        perror("Failed to set SPI mode");
        close(fd);
        return 1;
    }

    uint32_t speed = 1000000;

    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
    {
        perror("Failed to set SPI speed");
        close(fd);
        return 1;
    }

    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            struct spi_ioc_transfer transfer = {
                .tx_buf = (unsigned long)&tx[i],
                .rx_buf = (unsigned long)&rx[i],
                .len = 1,
                .speed_hz = speed,
                .bits_per_word = 8
            };

            if (ioctl(fd, SPI_IOC_MESSAGE(1), &transfer) < 0)
            {
                perror("SPI transfer failed");
                close(fd);
                return 1;
            }
        }

        printf("%d %d %d\n", rx[0], rx[1], rx[2]);

        usleep(500000);
    }

    close(fd);

    return 0;
}
