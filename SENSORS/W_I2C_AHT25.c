#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#define I2C_BUS    "/dev/i2c-0"
#define AHT25_ADDR 0x38

int main()
{
    int fd;
    uint8_t init_cmd[3] = {0xBE, 0x08, 0x00};
    uint8_t measure_cmd[3] = {0xAC, 0x33, 0x00};
    uint8_t data[7];

    /* Open I2C bus */
    fd = open(I2C_BUS, O_RDWR);

    if (fd < 0)
    {
        perror("Failed to open I2C bus");
        return 1;
    }

    /* Set AHT25 address */
    if (ioctl(fd, I2C_SLAVE, AHT25_ADDR) < 0)
    {
        perror("Failed to set AHT25 address");
        close(fd);
        return 1;
    }

    /* Initialize AHT25 */
    if (write(fd, init_cmd, 3) != 3)
    {
        perror("AHT25 initialization failed");
        close(fd);
        return 1;
    }

    usleep(10000);

    while (1)
    {
        /* Start measurement */
        if (write(fd, measure_cmd, 3) != 3)
        {
            perror("Measurement command failed");
            break;
        }

        /* Wait for measurement */
        usleep(80000);

        /* Read sensor data */
        if (read(fd, data, 7) != 7)
        {
            perror("Failed to read AHT25");
            break;
        }

        /* Check sensor busy status */
        if (data[0] & 0x80)
        {
            printf("Sensor busy\n");
            continue;
        }

        /* Calculate humidity raw value */
        uint32_t humidity_raw =
            ((uint32_t)data[1] << 12) |
            ((uint32_t)data[2] << 4) |
            ((uint32_t)data[3] >> 4);

        /* Convert humidity */
        float humidity =
            (humidity_raw * 100.0f) / 1048576.0f;

        /* Calculate temperature raw value */
        uint32_t temperature_raw =
            (((uint32_t)data[3] & 0x0F) << 16) |
            ((uint32_t)data[4] << 8) |
            data[5];

        /* Convert temperature */
        float temperature =
            (temperature_raw * 200.0f / 1048576.0f) - 50.0f;

        printf("Temperature: %.2f C\n", temperature);
        printf("Humidity   : %.2f %%\n", humidity);
        printf("------------------------\n");

        sleep(2);
    }

    close(fd);

    return 0;
}
