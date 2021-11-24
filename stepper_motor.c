#include "sys/alt_stdio.h"
#include <io.h>
#include <system.h>

const int FULL_STEP[] = {3, 6, 12, 9};
const int HALF_STEP[] = {1, 3, 2, 6, 4, 12, 8, 9};

int Pow(const int num, int pow)
{
    int answer = num;
    if (pow != 0)
    {
        for (int i = 0; i < pow - 1; ++i)
        {
            answer *= num;
        }
        return answer;
    }
    return 1;
}

void RotateMotor(int mode, int *cnt)
{
    int direction = mode % 2;
    mode -= direction;

    switch (mode)
    {
    case 2:
        (*cnt) %= 4;
        IOWR(PIO_GPIO_BASE, 0, Pow(2, *cnt));
        break;

    case 4:
        (*cnt) %= 4;
        IOWR(PIO_GPIO_BASE, 0, FULL_STEP[*cnt]);
        break;

    case 8:
        (*cnt) %= 8;
        IOWR(PIO_GPIO_BASE, 0, HALF_STEP[*cnt]);
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
        else
            *cnt == 0 ? (*cnt) += 7 : (*cnt)--;
    }
}

int main()
{
    alt_putstr("Stepper Motor Control is ON!\n");

    int sw_base = 0, cnt = 0;
    while (1)
    {
        sw_base = IORD(PIO_SW_BASE, 0);
        RotateMotor(sw_base, &cnt);
    }
    return 0;
}
