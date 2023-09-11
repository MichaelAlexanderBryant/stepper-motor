#include "stm32f4xx.h"

void delay_ms(int n);

int main(void){
	
	//const char steps[] = {0x80, 0x40, 0x20, 0x10}; // 4 steps per rotation (full step)
	const char steps[] = {0x80, 0xC0, 0x40, 0x60, 0x20, 0x30, 0x10, 0x90}; // 8 steps per rotation (half step)
	
	int i = 0;
	int N = sizeof(steps)/sizeof(steps[0]);
	
	
	RCC->AHB1ENR |= 1; // enable GPIOA clock
	GPIOA->MODER &= ~0x0000FF00; // clear modes for pins
	GPIOA->MODER |= 0x00005500; // PA4, PA5, PA6, PA7 as output
	
	while(1){
		GPIOA->ODR = steps[i]; // set the output pattern
		delay_ms(1);
		i = (i+1)%N;
	}
}

void delay_ms(int n){
	for(;n > 0; n--)
		for (int i = 0; i < 3195; i++);
}