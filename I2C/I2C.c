#include <stdio.h>
#include <unistd.h>
#include <mraa/i2c.h>
#define I2C_BUS 0 // I2C bus number, usually 0 or 1
#define EEPROM_ADDR 0x50 // EEPROM I2C address
#define START_ADDR 0x00 // EEPROM memory address to start reading
#define READ_LEN 32 // Number of bytes to read
int main() {
mraa_i2c_context i2c;
int i;
char buffer[READ_LEN + 1] = {0}; // +1 for null terminator
// Initialize I2C
i2c = mraa_i2c_init(I2C_BUS);
if (!i2c) {
fprintf(stderr, "Failed to initialize I2C\n");
return 1;
}
if (mraa_i2c_address(i2c, EEPROM_ADDR) != MRAA_SUCCESS) {
fprintf(stderr, "Failed to set I2C address\n");
mraa_i2c_stop(i2c);
return 1;
}
// Read bytes from EEPROM
for (i = 0; i < READ_LEN; i++) {
int val = mraa_i2c_read_byte_data(i2c, START_ADDR + i);
if (val < 0) {
fprintf(stderr, "Read failed at offset 0x%02X\n", START_ADDR + i);
mraa_i2c_stop(i2c);
return 1;
}
buffer[i] = (char)val;
}
buffer[READ_LEN] = '\0'; // Null terminate for safe string output
printf("EEPROM Data: \"%s\"\n", buffer);
// Clean up
mraa_i2c_stop(i2c);
mraa_deinit();
return 0;
}
