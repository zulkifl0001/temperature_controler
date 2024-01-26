#include "TM4C123GH6PM.h"
#include <stdio.h>
#include "input_asm.h"

extern void I2C3_Init();
extern void I2C3_Write_Multiple(int a,char b ,int c, int* d);
extern char I2C_wait_till_done();
//extern char I2C3_read_Multiple(int a,char b, int c, int* d);

char I2C3_read_Multiple(int slave_addr, char memory_addr, int bytes, int* data)
{
    char error;
    
    if ( bytes <= 0)
        return -1;         // no read was performed 

    
    I2C3->MSA = slave_addr << 1;//shift slave address by one and add read bit 
    I2C3->MDR = memory_addr;
    I2C3->MCS = 3;       // slave address, memory address
    error = I2C_wait_till_done();
    if (error)
        return error;

 
    I2C3->MSA = (slave_addr << 1) + 1;   // restart and add read bit

    if ( bytes == 1)             
        I2C3->MCS = 7;              
    else                           
        I2C3->MCS = 0xB;            
    error = I2C_wait_till_done();
    if (error) return error;

    *data++ = I2C3->MDR;            // store data on assigned address

    if (--bytes == 0)           
    {
        while(I2C3->MCS & 0x40);    // wait until bus is not busy 
        return 0;       // no error 
    }
 

    while (bytes > 1)
    {
        I2C3->MCS = 9;              // data ack
        error = I2C_wait_till_done();
        if (error) return error;
        bytes--;
        *data++ = I2C3->MDR;        // store data 
    }

    I2C3->MCS = 5;                  /* -data-NACK-P */
    error = I2C_wait_till_done();
    *data = I2C3->MDR;              // store data 
    while(I2C3->MCS & 0x40);        // wait until bus is not busy */
    
    return 0;       /* no error */
}

int slave_addr = 0x77;
int temp[3];
int temp_addr2= 0xFC;
int temp_addr1 = 0xFB;
int temp_addr0=0xFA;
int tempclb[6];
signed long adc_T;
unsigned short dig_T1;
short dig_T2,dig_T3;
double T,var1,var2;
int val1 = 0x23;
int val2 = 0;
double temparray [128];
double avgTemp, totalTemp;

extern void OutStr(char* );

void delay_ms(int time){ 
    for(int j=0; j<time;j++){
		   for(int k=0;k<1000;k++){}
		
		}

}

void read_tempcalibs(){ //read calibration registers and obtain dig_T1,dig_T2 and dig_T3 values to calculate temperature
			I2C3_read_Multiple(slave_addr,0x88,1,&tempclb[0]);
			I2C3_read_Multiple(slave_addr,0x89,1,&tempclb[1]);
			I2C3_read_Multiple(slave_addr,0x8A,1,&tempclb[2]);
			I2C3_read_Multiple(slave_addr,0x8B,1,&tempclb[3]);
			I2C3_read_Multiple(slave_addr,0x8C,1,&tempclb[4]);
			I2C3_read_Multiple(slave_addr,0x8D,1,&tempclb[5]);
      dig_T1= tempclb[1]*256+tempclb[0]; 
	    dig_T2= tempclb[3]*256+tempclb[2]; 
	    dig_T3= tempclb[5]*256+tempclb[4]; 
}


char* read(void)
{
	 
	 I2C3_Write_Multiple( slave_addr, 0xF4, 1, &val1); 
	
	 I2C3_Write_Multiple( slave_addr, 0xF5, 1, &val2);
	read_tempcalibs();

	char str[100];
		totalTemp=0;
		
		for(int i=0;i<128;i++){ // Array with 128 elements
			
		I2C3_read_Multiple(slave_addr,temp_addr0,1,&temp[0]); //FA reg
		I2C3_read_Multiple(slave_addr,temp_addr1,1,&temp[1]); //FB reg
		I2C3_read_Multiple(slave_addr,temp_addr2,1,&temp[2]); //FC reg 
		adc_T= temp[0]*65536+temp[1]*256+temp[2];
		adc_T=adc_T>>4; //combining 3 regs via arithmetic shifting and trimming 4 lsb
		 var1=(((double)adc_T)/16384 - ((double)dig_T1)/1024)*((double)dig_T2);
    var2=((((double)adc_T)/131072 - ((double)dig_T1)/8192)*(((double)adc_T)/131072 - ((double)dig_T1)/8192))*((double)dig_T3);
		T=(var1+var2)/5120; 
		temparray[i]=T;
			totalTemp=totalTemp+T;
		}
		avgTemp= totalTemp/128;
		
		sprintf(str, "%.2f ",avgTemp);
		
		return str;
		
	}