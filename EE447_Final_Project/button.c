#include "TM4C123GH6PM.h"  
#include <stdbool.h>
#include "button.h"



#define BUTTON_PORT      GPIOB
#define BUTTON_PIN_1     (1U << 0)  // PB0
#define BUTTON_PIN_2     (1U << 1)  // PB1
#define BUTTON_PIN_3     (1U << 2)  // PB2




void SysTick_Init(void) {
   
    SysTick->CTRL = 0;

    
    SysTick->LOAD = 8000 ;  

 
    SysTick->VAL = 0;

    // Enable SysTick
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}







void ButtonPort_Init(void) {
    // Enable Clock 
    SYSCTL->RCGCGPIO |= (1U << 1); //  B

    // Set PB5,PB6 and PB7 as input
    GPIOB->DIR &= ~(BUTTON_PIN_1 | BUTTON_PIN_2|BUTTON_PIN_3);
    GPIOB->DEN |= (BUTTON_PIN_1 | BUTTON_PIN_2|BUTTON_PIN_3); 
	
}


bool ReadButtonState(int buttonPin) {
    
    for (int i = 0; i < 1000; i++); // debouncing

    return (GPIOB->DATA & buttonPin) == buttonPin;
}



int button_press(void) {


  ButtonPort_Init(); // button 
	volatile int count=0;
  volatile bool prev1=0;
	volatile bool prev2=0;
	volatile bool prev3=0;
	
	while (count<2){
				//get current stats
        volatile bool currentStateButton1 = ReadButtonState(BUTTON_PIN_1);
        volatile bool currentStateButton2 = ReadButtonState(BUTTON_PIN_2);
				volatile bool currentStateButton3 = ReadButtonState(BUTTON_PIN_3);

				//if same as previous state then return accordingly
        if (!currentStateButton1&&!prev1) {
						return 0;
            
        }
        else if (!currentStateButton2&&!prev2) {
            return 1;
        }
				else if(!currentStateButton3&&!prev3){
					return 2;
				}
				 prev1=currentStateButton1;
				prev2=currentStateButton2;
				prev3=currentStateButton3;
				
				count++;
			}
			return 4;
}