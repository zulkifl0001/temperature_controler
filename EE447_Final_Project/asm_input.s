SYSCTL_RCGCGPIO EQU 		0x400FE608
SYSCTL_PRGPIO   EQU 		0x400FEA08
SYSCTL_RCGCI2C  EQU 		0x400FE620
SYSCTL_PRI2C    EQU 		0x400FEA20
GPIOD_DEN       EQU 		0x4000751C
GPIOD_AFSEL     EQU 		0x40007420
GPIOD_PCTL      EQU 		0x4000752C
GPIOD_ODR       EQU 		0x4000750C
I2C3_MCR        EQU 		0x40023020
I2C3_MTPR       EQU 		0x4002300C
I2C3            EQU 		0x40023000
I2C3_MSA        EQU     	0x40023000
I2C3_MDR        EQU     	0x40023008
I2C3_MCS        EQU    	 	0x40023004
	
            AREA        i2c3_Init, READONLY, CODE
            THUMB
			EXPORT 		I2C3_Init
			EXPORT		I2C_wait_till_done
			EXPORT		I2C3_Write_Multiple
			;EXPORT		I2C3_read_Multiple

I2C3_Init   
			LDR R1, =SYSCTL_RCGCGPIO; Enable the clock for port D 
			LDR R0, [R1]
			ORR R0, #0x08
			STR R0, [R1]
			NOP
			NOP
			NOP
			NOP
			


			
			LDR R1, =SYSCTL_RCGCI2C ; Enable the clock for I2C3 
			LDR R0, [R1]
			ORR R0, #0x08
			STR R0, [R1]

			

			
			LDR R1, =GPIOD_DEN ; Assert DEN for Port D (pins 0 and 1)
			LDR R0, [R1]
			ORR R0, #0x03
			STR R0, [R1]
			
			LDR R1, =GPIOD_AFSEL	; Configure the Port D pins 0 and 1 as I2C3
			LDR R0, [R1]
			ORR R0, #0x03
			STR R0, [R1]
			
			LDR R1, =GPIOD_PCTL	; Configure the Port D pins 0 and 1 as I2C3
			LDR R0, [R1]
			ORR R0, #0x33
			STR R0, [R1]
			
			LDR R1, =GPIOD_ODR	; Set SDA(PD1) pin as open drain
			LDR R0, [R1]
			ORR R0, #0x02
			STR R0, [R1]
			
			LDR R1, =I2C3_MCR	; Enable I2C3 master function 
			MOV R0, #0x0010
			STR R0, [R1]

			LDR R1, =I2C3_MTPR ;Configure I2C3 Clock Frequency
			MOV R0, #0x07	;Time Period = 16,000,000 / (2*(6+4)*100,000)-1 = 7
			STR R0, [R1]

			BX 	LR  ; Return
			
I2C_wait_till_done
                LDR     R0, =0x40023000           ; Load the base address of I2C3 into R0
WaitLoop        LDR     R1, [R0, #0x04]     ; Load the MCS register value into R1
                TST     R1, #1              ; Test the busy bit (bit 0)
                BNE     WaitLoop            ; Branch back to WaitLoop if the busy bit is set
                LDR     R0, [R0, #0x04]     ; Load the MCS register value into R0
                LSRS    R0, R0, #1          ; Shift right by 1 to get the error code
                AND     R0, R0, #0xE         ; Mask out the busy bit and other irrelevant bits
                BX      LR                  ; Return with the error code in R0
                
			

			
I2C3_Write_Multiple
				PUSH {R4-R7, LR}   ; Save registers and return address
				CMP R2, #0         ; Compare bytes with 0
				BLE no_bytes       ; If bytes <= 0, branch to no_bytes

				LSL R4, R0, #1     ; R4 = slave_addr << 1
				LDR R5, =I2C3 ; Load base address of I2C3

				; Write to I2C3->MSA
				ADD R6, R5, #0
				STR R4, [R6]

				; Write to I2C3->MDR
				ADD R6, R5, #0x008
				STR R1, [R6]

				; Write to I2C3->MCS with 3
				MOV R7, #3
				ADD R6, R5, #0x004
				STR R7, [R6]

				; Call I2C_wait_till_done
				BL I2C_wait_till_done
				CMP R0, #0
				BNE write_error    ; If error, branch to write_error

write_loop
				CMP R2, #1         ; Compare bytes with 1
				BLE last_byte      ; If 1 or less, branch to last_byte

				; Write a byte from data
				LDRB R4, [R3], #1  ; Load byte from data, post-increment R3
				ADD R6, R5, #0x008
				STRB R4, [R6]

				; Write to I2C3->MCS with 1
				MOV R7, #1
				ADD R6, R5, #0x004
				STR R7, [R6]

				; Call I2C_wait_till_done
				BL I2C_wait_till_done
				CMP R0, #0
				BNE write_error    ; If error, branch to write_error

				SUBS R2, R2, #1    ; Decrement bytes
				B write_loop

last_byte
				; Write the last byte
				LDRB R4, [R3]
				ADD R6, R5, #0x008
				STRB R4, [R6]

				; Write to I2C3->MCS with 5
				MOV R7, #5
				ADD R6, R5, #0x004
				STR R7, [R6]

				; Call I2C_wait_till_done
				BL I2C_wait_till_done
				CMP R0, #0
				BNE write_error    ; If error, branch to write_error

				; Wait until bus is free
				ADD R6, R5, #0x004
wait_bus_free1
				LDR R4, [R6]
				ANDS R4, R4, #0x40
				BNE wait_bus_free1

				MOV R0, #0         ; Return 0
				B exit_function1

no_bytes
				MOV R0, #-1        ; Return -1
				B exit_function1

write_error
				; Handle write error
				MOV R0, R7         ; Return error code

exit_function1
				POP {R4-R7, PC}    ; Restore registers and return			



            ALIGN
            END