/*--------------------------------------------------------------------*/
/* "stopwatch" - Grzegorz Karpiel                                     */
/* https://youtu.be/ZUK2JYRgXK8                                       */
/*--------------------------------------------------------------------*/
//#define DE10
/*--------------------------------------------------------------------*/
#ifdef DE10
#include <io.h>
#include "system.h"
#include "sys/alt_stdio.h"
#else
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#endif
/*--------------------------------------------------------------------*/
/* Do not edit this function */
void Show7D(unsigned char a, unsigned char b,
            unsigned char c, unsigned char d,
            unsigned char e, unsigned char f)
{
#ifdef DE10
    const unsigned char DispTab[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
    IOWR(PIO_A_BASE, 0, DispTab[a % 10]);
    IOWR(PIO_B_BASE, 0, DispTab[b % 10]);
    IOWR(PIO_C_BASE, 0, DispTab[c % 10] & 0x7F);
    IOWR(PIO_D_BASE, 0, DispTab[d % 10]);
    IOWR(PIO_E_BASE, 0, DispTab[e % 10] & 0x7F);
    IOWR(PIO_F_BASE, 0, DispTab[f % 10]);
#else
    /* Yoy can change \r to \n */
    printf("%i%i:%i%i.%i%i    \r", f % 10, e % 10, d % 10, c % 10, b % 10, a % 10);
    fflush(stdout);
#endif
}
/*--------------------------------------------------------------------*/
/* Do not edit this function */
unsigned char GetButton(void)
{
    unsigned char ret;
#ifdef DE10
    ret = ((IORD(PIO_KEY_BASE, 0) ^ 0x03) ? 1 : 0);
#else
    static unsigned int i = 0;
    unsigned char c = 0;
    /* Button Simulator */
    if (i < 14000)
    {
        i++;
    }
    else
    {
        i = 0;
    };
    if (((i > 200) && (i < 208)) || ((i > 800) && (i < 810)) || ((i > 1000) && (i < 1010)) || ((i > 1200) && (i < 1208)) || ((i > 13800) && (i < 13810)) || ((i > 13900) && (i < 13910)))
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    };
    if (ret)
    {
        c = '1';
    }
    else
    {
        c = '0';
    };
    printf("  [%c]   ", c);
#endif
    return ret;
}
/*--------------------------------------------------------------------*/
/* Do not edit this function */
void Wait5ms(void)
{
#ifdef DE10
    static unsigned int t = 0;
    while (IORD(PIO_TRIGGER_BASE, 0) == t)
        ;
    t = IORD(PIO_TRIGGER_BASE, 0);
#else
    usleep(5000);
#endif
}
/*--------------------------------------------------------------------*/
void UpdateNum(int pos, char *nums)
{
    nums[pos]++;
    if (nums[pos] % 10 == 0 && pos != 3)
    {
        nums[pos++] %= 10;
        UpdateNum(pos, nums);
    }
    else if (nums[pos] % 6 == 0 && pos == 3)
    {
        nums[pos - 1] = 0;
        nums[pos++] %= 6;
        UpdateNum(pos, nums);
    }
}

void RefreshNums(char *nums)
{
    for (int i = 0; i < 6; ++i)
    {
        nums[i] = 0;
    }
}

struct ButtonSignal
{
    unsigned char left;
    unsigned char right;
};
/*--------------------------------------------------------------------*/
int main()
{
    char stopper_nums[6] = {0, 0, 0, 0, 0, 0};

    unsigned char button;
    struct ButtonSignal signal = {'0', '0'};

    int click_count = 0;
    size_t curr_pos = 0;

    while (1)
    {
        Wait5ms();
        button = GetButton();

        signal.left = signal.right;
        signal.right = button;

        if (signal.left == 1 && signal.right == 0) //  __
            click_count < 3 ? click_count++ : 0;   //    |__

        if (click_count == 1)
        {
            curr_pos = 0;
            UpdateNum(curr_pos, stopper_nums);
        }

        else if (click_count == 3)
        {
            click_count = 0;
            RefreshNums(stopper_nums);
        }

        Show7D(stopper_nums[0], stopper_nums[1], stopper_nums[2],
               stopper_nums[3], stopper_nums[4], stopper_nums[5]);
    };
    return 0;
}
/*--------------------------------------------------------------------*/

int main()
{
    unsigned char a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
    unsigned char button = 0, pressedButton = 0;
    int task = 0, ms = 0, s = 0, min = 0;
    while (1)
    {
        Wait5ms();
        pressedButton = button;
        button = GetButton();
        if (button == 1 && pressedButton == 0)
        {
            task++;
        }

        /*switch(task)
        {
            case'0':
                ms=0;
                s=0;
                min=0;
                break;
            case '1':
                ms=ms+5;
                if(ms>=1000)
                {
                    s++;
                    ms=0;
                }
                if(s>=59)
                {
                    min++;
                    s=0;
                }
                break;
            case '3':
                task=0;
                break;
        }*/
        if (task == 3) // reset
        {
            task = 0;
        }
        if (task == 0) // waiting
        {
            ms = 0;
            s = 0;
            min = 0;
        }
        else if (task == 1) // running
        {
            ms = ms + 5;
            if (ms >= 1000)
            {
                s++;
                ms = 0;
            }
            if (s >= 59)
            {
                min++;
                s = 0;
            }
        }
        a = ms / 10;
        b = ms / 100;
        c = s % 10;
        d = s / 10;
        e = min % 10;
        f = min / 10;
        Show7D(a, b, c, d, e, f);
    };
    return 0;
}