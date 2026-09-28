#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <mraa/i2c.h>

#define I2C_BUS 0
#define MPU6050_ADDR 0x68

#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H  0x43

int main()
{
    mraa_i2c_context i2c;
    uint8_t data[6];

    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x, gyro_y, gyro_z;

    /* Initialize I2C */
    i2c = mraa_i2c_init(I2C_BUS);

    if (i2c == NULL)
    {
        printf("Failed to initialize I2C\n");
        return 1;
    }

    /* Set MPU6050 address */
    if (mraa_i2c_address(i2c, MPU6050_ADDR) != MRAA_SUCCESS)
    {
        printf("Failed to set I2C address\n");
        mraa_i2c_stop(i2c);
        return 1;
    }

    /* Wake up MPU6050 */
    if (mraa_i2c_write_byte_data(i2c, 0x00, PWR_MGMT_1) != MRAA_SUCCESS)
    {
        printf("Failed to wake MPU6050\n");
        mraa_i2c_stop(i2c);
        return 1;
    }

    printf("MPU6050 initialized\n");

    while (1)
    {
        /* Read accelerometer data */
        mraa_i2c_read_bytes_data(
            i2c,
            ACCEL_XOUT_H,
            data,
            6
        );

        accel_x = (int16_t)((data[0] << 8) | data[1]);
        accel_y = (int16_t)((data[2] << 8) | data[3]);
        accel_z = (int16_t)((data[4] << 8) | data[5]);

        /* Read gyroscope data */
        mraa_i2c_read_bytes_data(
            i2c,
            GYRO_XOUT_H,
            data,
            6
        );

        gyro_x = (int16_t)((data[0] << 8) | data[1]);
        gyro_y = (int16_t)((data[2] << 8) | data[3]);
        gyro_z = (int16_t)((data[4] << 8) | data[5]);

        printf("\nAccelerometer:\n");
        printf("X = %d\n", accel_x);
        printf("Y = %d\n", accel_y);
        printf("Z = %d\n", accel_z);

        printf("Gyroscope:\n");
        printf("X = %d\n", gyro_x);
        printf("Y = %d\n", gyro_y);
        printf("Z = %d\n", gyro_z);

        printf("------------------------\n");

        sleep(1);
    }

    mraa_i2c_stop(i2c);
    mraa_deinit();

    return 0;
}
