#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "clock.h"
#include "wait.h"

/*
 * This is the SRAM bit-banding test, where we are testing writing a single bit
 * to a variable in SRAM using bit-banding
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

    // Define which bit you want to test (0-31)
    const uint8_t BIT_TO_TEST = 0;

    // Calculate the bit-banded address for the specific bit we want to write to.
    // The formula is: alias_addr = alias_base + (byte_offset * 32) + (bit_num * 4)    fyi in this case the (bit_num * 4) part doesn't matter since the test bit is 0
    volatile uint32_t* bb_address = (volatile uint32_t*)(0x22000000 + (((uint32_t)&sram_variable - 0x20000000) * 32) + (BIT_TO_TEST * 4));

    start_ticks = NVIC_ST_CURRENT_R; // Start the timer

    for(i = 0; i < 10000; i++)
    {
        *bb_address = i & 1;
    }

    end_ticks = NVIC_ST_CURRENT_R; // End the timer

    ticks = (start_ticks - end_ticks) & 0x00FFFFFF; // Will be the raw number of clock cycles / ticks

    while(1) // Set a breakpoint here to view values in memory
    {
        // Loop infinitely
    }

    return 0;
}
