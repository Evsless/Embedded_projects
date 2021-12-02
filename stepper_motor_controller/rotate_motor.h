#pragma once

#include <unistd.h>

#include "pwm.h"
void RotateMotor(int mode, int *cnt)
{
    int direction = mode % 2;
    mode -= direction;

    switch (mode)
    {
    case 2:
        (*cnt) %= 4;
        IOWR(PIO_GPIO_BASE, 0, WAVE_MODE[*cnt]);
        break;

    case 4:
        (*cnt) %= 4;
        IOWR(PIO_GPIO_BASE, 0, FULL_STEP[*cnt]);
        break;

    case 8:
        (*cnt) %= 8;
        IOWR(PIO_GPIO_BASE, 0, HALF_STEP[*cnt]);
        break;

    case 16:
        (*cnt) %= 16;
        if ((*cnt) % 2 == 0) // FULL POWER
            IOWR(PIO_GPIO_BASE, 0, COIL_PAIRS[*cnt].first);
        else // HALF POWER
            PWM(COIL_PAIRS[*cnt].first, COIL_PAIRS[*cnt].second);
        break;

    case 512:
        (*cnt) %= 8;
        if ((*cnt) % 2 == 0)                         // FULL_POWER
            IOWR(PIO_GPIO_BASE, 0, HALF_STEP[*cnt]); // HALF_POWER
        else
            PWM(HALF_STEP[*cnt], 0);

        break;

    default:
        break;
    }

    if (direction == 0)
        (*cnt)++;

    else
    {
        if (mode == 4)
            *cnt == 0 ? (*cnt) += 3 : (*cnt)--;
        else if (mode == 8)
            *cnt == 0 ? (*cnt) += 7 : (*cnt)--;
        else
            *cnt == 0 ? (*cnt) += 15 : (*cnt)--;
    }
}
