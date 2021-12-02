#pragma once

#define PWM(port_1, port_2)                     \
    {                                           \
        int toothsaw_sig = 0;                   \
        int impulse_cnt = 0;                    \
        while (impulse_cnt < 10000)             \
        {                                       \
            toothsaw_sig++;                     \
            if (toothsaw_sig == 100)            \
            {                                   \
                toothsaw_sig = 0;               \
                impulse_cnt++;                  \
                usleep(3);                      \
            }                                   \
            if (toothsaw_sig < 20)              \
                IOWR(PIO_GPIO_BASE, 0, port_1); \
            else                                \
                IOWR(PIO_GPIO_BASE, 0, port_2); \
        }                                       \
    }