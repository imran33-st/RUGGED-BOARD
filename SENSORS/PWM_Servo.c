#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PWM_PATH "/sys/class/pwm/pwmchip0/pwm0"

void write_pwm(const char *file, const char *value)
{
    char path[100];
    FILE *fp;

    snprintf(path, sizeof(path), "%s/%s", PWM_PATH, file);

    fp = fopen(path, "w");

    if (fp == NULL)
    {
        perror("PWM file error");
        exit(1);
    }

    fprintf(fp, "%s", value);
    fclose(fp);
}

int main()
{
    /* Set PWM period = 20 ms = 50 Hz */
    write_pwm("period", "20000000");

    /* Start with servo at 0 degree */
    write_pwm("duty_cycle", "1000000");

    write_pwm("enable", "1");

    while (1)
    {
        /* 0 degree */
        write_pwm("duty_cycle", "1000000");
        printf("Servo: 0 degree\n");
        sleep(2);

        /* 90 degree */
        write_pwm("duty_cycle", "1500000");
        printf("Servo: 90 degree\n");
        sleep(2);

        /* 180 degree */
        write_pwm("duty_cycle", "2000000");
        printf("Servo: 180 degree\n");
        sleep(2);

        /* 90 degree */
        write_pwm("duty_cycle", "1500000");
        printf("Servo: 90 degree\n");
        sleep(2);
    }

    write_pwm("enable", "0");

    return 0;
}
