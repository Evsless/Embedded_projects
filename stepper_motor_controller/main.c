#include "sys/alt_stdio.h"
#include <io.h>
#include <system.h>

#include "rotate_motor.h"
#include "coil_pair.h"
#include "pwm.h"

void RotateMotor(int mode, int &cnt);

struct CoilPair COIL_PAIRS[] = {{1, 0}, {3, 1}, {3, 0}, {3, 2}, {2, 0}, {6, 2}, {6, 0}, {6, 4}, {4, 0}, {12, 4}, {12, 0}, {12, 8}, {8, 0}, {9, 8}, {9, 0}, {9, 1}};

const int WAVE_MODE[] = {1, 2, 4, 8};
const int FULL_STEP[] = {3, 6, 12, 9};
const int HALF_STEP[] = {1, 3, 2, 6, 4, 12, 8, 9};

int main(void)
{
    alt_putstr("Stepper Motor Control is ON!\n");
    int sw_base = 0, cnt = 0;
    while (1)
    {
        sw_base = IORD(PIO_SW_BASE, 0);
        RotateMotor(sw_base, &cnt);
    }
}