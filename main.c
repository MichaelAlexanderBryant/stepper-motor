#include "stm32f4xx.h"

void stepper_init(void);
void button_init(void);
void delay_ms(int n);

int main(void){
	
	const char full_steps[] = {0x80, 0x40, 0x20, 0x10};
	const char half_steps[] = {0x80, 0xC0, 0x40, 0x60, 0x20, 0x30, 0x10, 0x90};

	int is_full_step = 1; // full step = 1, half step = 0
	int i = 0; // index for arrays
	int direction = 1; // counter clockwise = 1, clockwise = -1
	
	stepper_init();
	button_init();
	
	while(1){
		// if switch 1 is pressed and held then full step else half step
		if (GPIOC->IDR & 1) 
			is_full_step = 1;
		else
			is_full_step = 0;
		// if switch 2 is pressed and held then counter clockwise else clockwise
		if (GPIOC->IDR & 2)
			direction=1;
		else
			direction=-1;
		// set electromagnet/increment stepper
		switch(is_full_step){
			case 0:
				GPIOA->ODR = half_steps[i]; // set half step output pattern
				delay_ms(1);
				i = (i+direction)%8; // index 0 to 7
				if (i<0)
					i = 7;
				break;
			case 1:
				GPIOA->ODR = full_steps[i]; // set full step output pattern
				delay_ms(1);
				i = (i+direction)%4; // index 0 to 3 
				if (i<0)
					i = 3;
				break;
		}
	}
}

void stepper_init(void){
	RCC->AHB1ENR |= 1; // enable GPIOA clock
	GPIOA->MODER &= ~0x0000FF00; // clear modes for pins
	GPIOA->MODER |= 0x00005500; // PA4, PA5, PA6, PA7 as output
}

void button_init(void){	
	RCC->AHB1ENR |= 4; // enable GPIOC clock
	GPIOC->MODER &= ~0x0000000F; // PC0, PC1 as input
}

void delay_ms(int n){
	for(;n > 0; n--)
		for (int i = 0; i < 3195; i++);
}