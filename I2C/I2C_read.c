#include <stdio.h>
#include <stdint.h>
#include <mraa/i2c.h>

#define I2C_BUS       0
#define EEPROM_ADDR   0x52
#define START_ADDR    0x0000
#define READ_LEN      16

int main()
{
    mraa_i2c_context i2c;
    uint8_t buffer[READ_LEN];

    /* Initialize I2C */
    i2c = mraa_i2c_init(I2C_BUS);

    if (i2c == NULL)
    {
        printf("Failed to initialize I2C\n");
        return 1;
    }

    /* Set EEPROM I2C address */
    if (mraa_i2c_address(i2c, EEPROM_ADDR) != MRAA_SUCCESS)
    {
        printf("Failed to set EEPROM address\n");
        mraa_i2c_stop(i2c);
        return 1;
    }

    /* Set EEPROM memory address: 0x0000 */
    if (mraa_i2c_write_byte(i2c, (START_ADDR >> 8) & 0xFF) != MRAA_SUCCESS)
    {
        printf("Failed to send memory address MSB\n");
        mraa_i2c_stop(i2c);
        return 1;
    }

    if (mraa_i2c_write_byte(i2c, START_ADDR & 0xFF) != MRAA_SUCCESS)
    {
        printf("Failed to send memory address LSB\n");
        mraa_i2c_stop(i2c);
        return 1;
    }

    /* Read 16 bytes */
    if (mraa_i2c_read(i2c, buffer, READ_LEN) != READ_LEN)
    {
        printf("Failed to read EEPROM data\n");
        mraa_i2c_stop(i2c);
        return 1;
    }

    /* Display data in hexadecimal */
    printf("EEPROM Data:\n");

    for (int i = 0; i < READ_LEN; i++)
    {
        printf("0x%02X ", buffer[i]);
    }

    printf("\n");

    /* Cleanup */
    mraa_i2c_stop(i2c);
    mraa_deinit();

    return 0;
}
