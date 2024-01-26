#include "TM4C123GH6PM.h"

void LED_Init(void) {
    //  Enable Clock to GPIO Port F
    SYSCTL->RCGCGPIO |= (1U << 5);  // Enable clock for Port F (bit 5)
    while ((SYSCTL->PRGPIO & (1U << 5)) == 0) {}; // Wait for clock to be set

    //  Set PF3,2,1 as Output
    GPIOF->DIR |= (1U << 3); // Set PF3 as output
		GPIOF->DIR |= (1U << 2); // PF2
		GPIOF->DIR |= (1U << 1); // PF1
			
    // Step 3: Enable Digital Function of PF3,2,1
    GPIOF->DEN |= (1U << 3); // Enable digital function on PF3
		GPIOF->DEN |= (1U << 2); // PF2
		GPIOF->DEN |= (1U << 1); // PF1
}

void G_LED_On(void) {    
    GPIOF->DATA |= (1U << 3); 
}

void B_LED_On(void) {    
    GPIOF->DATA |= (1U << 2); 
}

void R_LED_On(void) {
    GPIOF->DATA |= (1U << 1); 
}

void G_LED_Off(void) {
    GPIOF->DATA &= ~(1U << 3); 
}

void B_LED_Off(void) {
    GPIOF->DATA &= ~(1U << 2); 
}

void R_LED_Off(void) {
    GPIOF->DATA &= ~(1U << 1);
}


