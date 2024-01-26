
#define CONTRAST                0xAB //increase to make darker

void Nokia5110_Init(void);
void Nokia5110_OutChar(unsigned char data);
void Nokia5110_OutString(unsigned char *ptr);
void Nokia5110_SetCursor(unsigned char newX, unsigned char newY);
void Nokia5110_Clear(void);

