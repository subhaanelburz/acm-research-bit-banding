#include <stdint.h>
#include <stdbool.h>
#include "clock.h"
#include "wait.h"
#include "tm4c123gh6pm.h"

volatile uint32_t LED_state = 0;

/*
 *      Interrupt every second because 4MHz clock (4mil ticks/second) = 250 ns (1 tick)
 *      So for every second you need 4,000,000 ticks
 *      SysTick count from 0 so, reload = 4000000 - 1 = 3999999
 * */

void SysTick_Handler(void)
{
    if (LED_state) // if ON
    {
        GPIO_PORTF_DATA_R &= ~0x04; // turn the blue LED off
        LED_state = 0;
    }
    else
    {
        GPIO_PORTF_DATA_R |= 0x04; // turn the blue LED on; binary 0x04 = 0000 0100, Pin 2 is the Blue LED
        LED_state = 1;
    }
}

/**
 * main.c
 */
int main(void)
{
    // Enable system clock
    initSystemClockTo40Mhz();

    // Set RELOAD
    NVIC_ST_RELOAD_R = (4000000 - 1);

    // Clear Current
    NVIC_ST_CURRENT_R = 0x0;

    // Set Control       4 MHZ Clock
    NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_CLK_SRC;
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE | NVIC_ST_CTRL_INTEN; // enable counter and interrupt

    // Enable clocks
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
    _delay_cycles(3);

    // LED pin setup
    // 2  = RED , 4 = BLUE , 8 = GREEN
    //    0000 1110 = 0x0E  LEDs
    //    The on board LEDs are PF1, PF2, PF3
    GPIO_PORTF_DIR_R |= 0x0E;
    GPIO_PORTF_DEN_R |= 0x0E;

    while (1)
    {

    }

    return 0;
}
