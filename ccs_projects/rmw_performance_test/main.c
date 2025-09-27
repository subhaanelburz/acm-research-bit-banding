#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "clock.h"
#include "wait.h"

/*
 *  On-board LEDs explained:
 *
 *  The On-board LEDs are PF1, PF2, and PF3 so in binary we enable: [0000 1110] = 0x0E
 *
 *  The colors are as followed: 2 = RED , 4 = BLUE , 8 = GREEN
 */

int main(void)
{
    initSystemClockTo40Mhz(); // Enable system clock

    NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M; // set reload value to the max, 0x00FFFFFF

    NVIC_ST_CURRENT_R = 0x0; // set current to any value

    // enable clock to 40 MHz, and enable the counter
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_ENABLE;

    volatile uint32_t start_ticks = 0;
    volatile uint32_t end_ticks = 0;
    volatile uint32_t ticks = 0;
    // the counter counts down every clock cycle, and the final value of ticks
    // will be the # of clock cycles it takes to complete the code between the
    // start ticks and end ticks

    volatile uint32_t i = 0;

    // ensure that port F is in APB
    SYSCTL_GPIOHBCTL_R &= ~SYSCTL_GPIOHBCTL_PORTF;

    _delay_cycles(3);

    // enable clock for port F
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;

    _delay_cycles(3);

    GPIO_PORTF_DIR_R |= 0x0E;   // enable PF1, PF2, and PF3 as outputs
    GPIO_PORTF_DEN_R |= 0x0E;   // enable PF1, PF2, and PF3 digital functions

    start_ticks = NVIC_ST_CURRENT_R; // start the timer

    for(i = 0; i < 1000000; i++)
    {
        GPIO_PORTF_DATA_R ^= 0x02; // toggle the red LED
    }

    end_ticks = NVIC_ST_CURRENT_R; // end the timer

    ticks = (start_ticks - end_ticks) & 0x00FFFFFF; // will be the raw number of clock cycles / ticks

    while(1) // set a breakpoint here to view values in memory
    {
        // loop infinitely
    }

    return 0;
}
