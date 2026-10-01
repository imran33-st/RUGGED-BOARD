#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <mraa/i2c.h>

#define I2C_BUS 0
#define AHT25_ADDR 0x38

int main()
{
    mraa_i2c_context i2c;
    uint8_t data[7];

    /* Initialize I2C */
    i2c = mraa_i2c_init(I2C_BUS);

    if (i2c == NULL)
    {
        printf("Failed to initialize I2C\n");
        return 1;
    }

    /* Set AHT25 I2C address */
    if (mraa_i2c_address(i2c, AHT25_ADDR) != MRAA_SUCCESS)
    {
        printf("Failed to set I2C address\n");
        mraa_i2c_stop(i2c);
        return 1;
    }

    /* Initialize AHT25 */
    uint8_t init_cmd[3] = {0xBE, 0x08, 0x00};

    if (mraa_i2c_write(i2c, init_cmd, 3) != MRAA_SUCCESS)
    {
        printf("AHT25 initialization failed\n");
        mraa_i2c_stop(i2c);
        return 1;
    }

    usleep(10000);

    while (1)
    {
        /* Trigger measurement */
        uint8_t measure_cmd[3] = {0xAC, 0x33, 0x00};

        if (mraa_i2c_write(i2c, measure_cmd, 3) != MRAA_SUCCESS)
        {
            printf("Measurement command failed\n");
            break;
        }

        /* Wait for measurement */
        usleep(80000);

        /* Read 7 bytes */
        if (mraa_i2c_read(i2c, data, 7) != 7)
        {
            printf("Failed to read sensor data\n");
            break;
        }

        /* Check busy bit */
        if (data[0] & 0x80)
        {
            printf("Sensor is busy\n");
            continue;
        }

        /* Calculate humidity */
        uint32_t humidity_raw =
            ((uint32_t)data[1] << 12) |
            ((uint32_t)data[2] << 4) |
            ((data[3] >> 4) & 0x0F);

        float humidity =
            ((float)humidity_raw * 100.0) / 1048576.0;

        /* Calculate temperature */
        uint32_t temperature_raw =
            (((uint32_t)data[3] & 0x0F) << 16) |
            ((uint32_t)data[4] << 8) |
            data[5];

        float temperature =
            ((float)temperature_raw * 200.0 / 1048576.0) - 50.0;

        printf("Temperature: %.2f °C\n", temperature);
        printf("Humidity   : %.2f %%\n", humidity);
        printf("-------------------------\n");

        sleep(2);
    }

    /* Cleanup */
    mraa_i2c_stop(i2c);
    mraa_deinit();

    return 0;
}
