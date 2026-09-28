#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#define I2C_BUS      "/dev/i2c-0"
#define EEPROM_ADDR  0x52
#define START_ADDR   0x0000
#define READ_LEN     16

int main()
{
    int fd;
    uint8_t mem_addr[2];
    uint8_t buffer[READ_LEN];

    struct i2c_msg msgs[2];
    struct i2c_rdwr_ioctl_data ioctl_data;

    /* Open I2C bus */
    fd = open(I2C_BUS, O_RDWR);

    if (fd < 0)
    {
        perror("Failed to open I2C bus");
        return 1;
    }

    /* 16-bit EEPROM memory address */
    mem_addr[0] = (START_ADDR >> 8) & 0xFF;
    mem_addr[1] = START_ADDR & 0xFF;

    /* Message 1: Write EEPROM memory address */
    msgs[0].addr  = EEPROM_ADDR;
    msgs[0].flags = 0;
    msgs[0].len   = 2;
    msgs[0].buf   = mem_addr;

    /* Message 2: Read EEPROM data */
    msgs[1].addr  = EEPROM_ADDR;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len   = READ_LEN;
    msgs[1].buf   = buffer;

    /* Perform combined transaction */
    ioctl_data.msgs  = msgs;
    ioctl_data.nmsgs = 2;

    if (ioctl(fd, I2C_RDWR, &ioctl_data) < 0)
    {
        perror("Failed to read EEPROM");
        close(fd);
        return 1;
    }

    /* Display data */
    printf("EEPROM Data:\n");

    for (int i = 0; i < READ_LEN; i++)
    {
        printf("0x%02X ", buffer[i]);
    }

    printf("\n");

    close(fd);

    return 0;
}
