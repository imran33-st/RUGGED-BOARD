#include <mraa/spi.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
int main()
{
       	mraa_spi_context spi; 
	/* Initialize SPI bus 3 */ 
	spi = mraa_spi_init(3); 
	if (spi == NULL) 
	{ 
		printf("Failed to initialize SPI\n"); 
		return 1; 
	} 
	/* SPI configuration */ 
	mraa_spi_mode(spi, 0); 
	mraa_spi_frequency(spi, 1000000); 
	uint8_t tx[3] = {100, 123, 75}; 
	uint8_t rx[3]; 
	while (1) 
	{ 
		/* Send and receive 3 bytes */ 
		for (int i = 0; i < 3; i++) 
		{ 
			rx[i] = mraa_spi_write(spi, tx[i]); 
		} 
		/* Display received data */ 
		printf("%d %d %d\n", rx[0], rx[1], rx[2]);
	       	/* Wait 0.5 second */
	       	usleep(500000);
       	}
       	mraa_spi_stop(spi);
       	mraa_deinit();
       	return 0; 
}
