#include <stdint.h>
#include "tm4c123gh6pm.h"


//Using PF0 for heat and PF4 for cooling
void init_heat_cool(void)
{
    // Enable Clock for GPIO Port F
    SYSCTL->RCGCGPIO |= (1U << 5);  // Enable clock for Port F (bit 5)
    while ((SYSCTL->PRGPIO & (1U << 5)) == 0) {}; // Wait for clock to be set

    GPIOF->LOCK = 0x4C4F434B; // Unlock GPIO Port F
    GPIOF->CR = 0x11; // Allow changes to PF0 and PF4

    GPIOF->AMSEL &= ~0x11; // Disable analog function on PF0 and PF4
    GPIOF->PCTL &= ~0x000F000F; // GPIO clear bit PCTL on PF0 and PF4
    GPIOF->DIR |= 0x11; // PF0 and PF4 as output
    GPIOF->AFSEL &= ~0x11; // No alternate function on PF0 and PF4
    GPIOF->DEN |= 0x11; // Enable digital function for PF0 and PF4
}

void Heat_on(void){
GPIOF->DATA |= 0x01;
	
}

void Heat_off(void){
	GPIOF->DATA &= ~0x01;

}

void Cool_on(void){
	GPIOF->DATA |= 0x10;

}

void Cool_off(void){
	
	GPIOF->DATA &= ~0x10;

}