#include "TM4C123GH6PM.h"  
#include <stdio.h>


#define SYSCTL_RCGCADC_R       (*((volatile unsigned long *)0x400FE638))
#define SYSCTL_RCGCGPIO_R      (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTB_AFSEL_R     (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R       (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AMSEL_R     (*((volatile unsigned long *)0x40005528))
#define ADC0_ACTSS_R           (*((volatile unsigned long *)0x40038000))
#define ADC0_EMUX_R            (*((volatile unsigned long *)0x40038014))
#define ADC0_SSMUX3_R          (*((volatile unsigned long *)0x400380A0))
#define ADC0_SSCTL3_R          (*((volatile unsigned long *)0x400380A4))
#define ADC0_IM_R              (*((volatile unsigned long *)0x40038008))
#define ADC0_PC_R              (*((volatile unsigned long *)0x40038FC4))
#define ADC0_PSSI_R            (*((volatile unsigned long *)0x40038028))
#define ADC0_RIS_R             (*((volatile unsigned long *)0x40038004))
#define ADC0_SSFIFO3_R         (*((volatile unsigned long *)0x400380A8))
#define ADC0_ISC_R             (*((volatile unsigned long *)0x4003800C))

void ADC0_Init(void) {
    // Enable the ADC0 peripheral
    SYSCTL_RCGCADC_R |= 0x01;

    // Enable the clock to GPIO port B
    SYSCTL_RCGCGPIO_R |= 0x02;

    // Configure PB4 for ADC input
    GPIO_PORTB_AFSEL_R |= 0x10;  // Enable alternate function
    GPIO_PORTB_DEN_R &= ~0x10;   // Disable digital function
    GPIO_PORTB_AMSEL_R |= 0x10;  // Enable analog functionality

    // Disable sequencer 3 during configuration
    ADC0_ACTSS_R &= ~0x08;

    // Configure the event multiplexer for software trigger
    ADC0_EMUX_R &= ~0xF000;

    // Select AIN10 (PB4) as the input for sequencer 3
    ADC0_SSMUX3_R = 0x0A;

    // Enable interrupt, and set the end of conversion
    ADC0_SSCTL3_R = 0x06;

    // Set the ADC sample rate to 125 ksps
    ADC0_PC_R = 0x01;

    // Re-enable sequencer 3
    ADC0_ACTSS_R |= 0x08;
}

void delay(void){
	int del=0;
	while (del<1000000){
	del++;
	}
	
}

char* set_val(void) {
    ADC0_Init();
    int result;
    int adjusted_result;
    float voltage;
    char formatted_output[10]; // Buffer to hold the formatted string
    float offset = 1.65;  
    float scaling_factor = 3.3 / 4095;  // Scaling factor for voltage conversion

    while (1) {
				
        // Initiate a sample with sequencer 3
        ADC0_PSSI_R = 0x08;

        // Wait for the conversion to complete
        while ((ADC0_RIS_R & 0x08) == 0) {}

        // Read the result
        result = (ADC0_SSFIFO3_R & 0xFFF);

        // Convert the adjusted result to a voltage value
        voltage = result * scaling_factor - offset;
				voltage=3.03 * voltage + 25;

				char buffer[50];
					
				sprintf(buffer, "%.2f ",voltage);
				
				
        // Clear the interrupt
        ADC0_ISC_R = 0x08;
				
				return buffer;
				
    }
}