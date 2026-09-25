#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define GPIO_EXPORT "/sys/class/gpio/export"
#define LED_DIR     "/sys/class/gpio/PC13/direction"
#define LED_VALUE   "/sys/class/gpio/PC13/value"
#define GPIO_NUM    77

int main()
{
    int fd;
    char gpio_buf[10];

    /* Export GPIO */
    fd = open(GPIO_EXPORT, O_WRONLY);
    if (fd < 0)
    {
        printf("Unable to open export\n");
        return 1;
    }

    sprintf(gpio_buf, "%d", GPIO_NUM);
    write(fd, gpio_buf, strlen(gpio_buf));
    close(fd);

    /* Set direction */
    fd = open(LED_DIR, O_WRONLY);
    if (fd < 0)
    {
        printf("Unable to open direction\n");
        return 1;
    }

    write(fd, "out", 3);
    close(fd);

    /* Turn LED ON */
    fd = open(LED_VALUE, O_WRONLY);

    write(fd, "1", 1);

    close(fd);

    printf("LED ON\n");

    return 0;
}
