#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "clock.h"
#include "wait.h"

/*
 * This is a test to measure the performance of a read-modify-write (RMW)
 * operation on a single bit of a variable in SRAM.
 */

// Declaring a global variable so its memory address is fixed/permanent.
volatile uint32_t sram_variable;

int main(void)
{
    initSystemClockTo40Mhz(); // Enable system clock

    NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M; // Set reload value to the max, 0x00FFFFFF
    NVIC_ST_CURRENT_R = 0x0;            // Set current to any value

    // Enable clock to 40 MHz, and enable the counter
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_ENABLE;

    volatile uint32_t start_ticks = 0;
    volatile uint32_t end_ticks = 0;
    volatile uint32_t ticks = 0;

    // The counter counts down every clock cycle. The final value of ticks
    // will be the # of clock cycles it takes to complete the code between the
    // start_ticks and end_ticks.

    volatile uint32_t i = 0;

    sram_variable = 0; // Clear the variable initially


    // this is just the bit we want to test. Could be 0-31
    const uint8_t BIT_TO_TEST = 0;

    // creates a mask with a 1 in the position of the bit we want to modify.
    const uint32_t mask = (1 << BIT_TO_TEST);

    start_ticks = NVIC_ST_CURRENT_R; // Start the timer

    for(i = 0; i < 10000; i++)
    {
        sram_variable ^= mask;  //toggle sram variable using the mask
    }

    end_ticks = NVIC_ST_CURRENT_R; // End the timer


    ticks = (start_ticks - end_ticks) & 0x00FFFFFF; // Will be the raw number of clock cycles / ticks

    while(1) // Set a breakpoint here to view values in memory
    {
        // Loop infinitely
    }

    return 0;
}
