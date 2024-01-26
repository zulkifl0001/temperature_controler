#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "Nokia5110.h"
#include "sysctl.h"
#include "control.h"
#include "button.h"
#include "LED.h"
#include "input_asm.h"
#include "init heat_cool.h"

// PB0-PB2 for buttons
//PB4 for potentiometer
//
//

int main (void)
{		
	  Nokia5110_Init(); //initalise screen
    ADC0_Init(); //Initalise ADC for potentiometer
		LED_Init();  //Initalise LED
		I2C3_Init(); //I2C3 
    Nokia5110_Clear();//clear screen before starting
		init_heat_cool();//initalise heating and cooling components
		
	  //char arrays needed for printing
		char* min_val="20.00";  //temporary max/min vals
		char* max_val="30.00";
		char* current_val=read(); 
		
		float min_temp;
		float max_temp;
		float current_temp;
		
		
		//convert to float for computations
		min_temp=atof(min_val);
		max_temp=atof(max_val);
		current_temp=atof(current_val);
	
		current_temp=atof(current_val);

		// LED + Temprature control logic
		if (current_temp>=min_temp){
					//within temp range
					if(current_temp<=max_temp){
						B_LED_Off();
						R_LED_Off();
						G_LED_On();
						Heat_off();
						Cool_off();
						
					}
					else{
					//more than range
					B_LED_On();
					R_LED_Off();
					G_LED_Off();
					Cool_on();
					Heat_off();
					}
				}
				else{
				//less than range
				R_LED_On();
				B_LED_Off();
				G_LED_Off();
				Heat_on();
				Cool_off();
				}
    
		//display inital values
		Nokia5110_SetCursor(2,0);
		Nokia5110_OutString("tem=25.00");
		Nokia5110_SetCursor(6,0);
		Nokia5110_OutString(current_val);
		Nokia5110_SetCursor(2,2);
		Nokia5110_OutString("min=20.00");
		Nokia5110_SetCursor(2,4);
		Nokia5110_OutString("max=30.00");
		
    while(1){
				current_val=read(); // get value
				Nokia5110_SetCursor(6,0);
				Nokia5110_OutString(current_val); // set value on screen
				current_temp=atof(current_val);
				int x=button_press(); 
				if(x==0){													//first button
					while(x!=2){
						Nokia5110_SetCursor(6,2);

						min_val=set_val();
						
						Nokia5110_OutString(min_val);	//change min val
						
						x=button_press();
					}
					min_temp=atof(min_val); //convert to float for computations 
				}
				if(x==1){
					while(x!=2){
						Nokia5110_SetCursor(6,4);
						max_val=set_val();
						Nokia5110_OutString(max_val);// change max val
						x=button_press();
					}
					max_temp=atof(max_val);//float
				}
				
				
				
				//control logic again
				if (current_temp>=min_temp){
					if(current_temp<=max_temp){
						B_LED_Off();
						R_LED_Off();
						G_LED_On();
						Heat_off();
						Cool_off();
						
					}
					else{
					B_LED_On();
					R_LED_Off();
					G_LED_Off();
					Cool_on();
					Heat_off();
					}
				}
				else{
				R_LED_On();
				B_LED_Off();
				G_LED_Off();
				Heat_on();
				Cool_off();
				}
				
				
				
    
	}
}
