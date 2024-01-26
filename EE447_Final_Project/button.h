#include <stdbool.h>

void SysTick_Init(void);
void ButtonPort_Init(void);
bool ReadButtonState(int buttonPin);
int button_press(void);
