#include <stdio.h>
#include <stdlib.h>
#include <mraa/pwm.h>

#define PWM_PIN 72

int main()
{
    mraa_pwm_context pwm;
    char input[20];
    char *end;
    long duty;

    mraa_init();

    pwm = mraa_pwm_init(PWM_PIN);

    if (pwm == NULL)
    {
        printf("PWM initialization failed\n");
        return 1;
    }

    /* 1 kHz PWM */
    mraa_pwm_period_us(pwm, 1000);

    /* Enable PWM */
    mraa_pwm_enable(pwm, 1);

    while (1)
    {
        printf("Enter duty cycle (0-100) or -1 to exit: ");
        fflush(stdout);

        /* Read input */
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        /* Convert input to number */
        duty = strtol(input, &end, 10);

        /* Check if input is a valid number */
        if (end == input)
        {
            printf("Invalid input! Enter a number.\n");
            continue;
        }

        /* Exit */
        if (duty == -1)
        {
            break;
        }

        /* Check range */
        if (duty < 0 || duty > 100)
        {
            printf("Invalid duty cycle! Enter 0-100.\n");
            continue;
        }

        /* Set duty cycle */
        mraa_pwm_write(pwm, duty / 100.0);

        printf("Duty cycle set to %ld%%\n", duty);
    }

    /* Turn PWM off before exiting */
    mraa_pwm_write(pwm, 0.0);
    mraa_pwm_enable(pwm, 0);
    mraa_pwm_close(pwm);

    return 0;
}
