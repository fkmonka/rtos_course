#include <stdint.h>
#include "stm32g4xx.h" // dzieki temu plikowi zamiast adresu rejestru mozemy od razu użyć już stworzone makro

int main(void)
{
	/*
	  #define RCC_AHB2ENR_GPIOAEN_Msk            (0x1UL << RCC_AHB2ENR_GPIOAEN_Pos) // !< 0x00000001
	#define RCC_AHB2ENR_GPIOAEN                  RCC_AHB2ENR_GPIOAEN_Msk
	 */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   //(1<<0); //ustawiamy na 0 bicie wartosc 1

	GPIOA->MODER &= ~GPIO_MODER_MODE5;   // wyzeruj oba bity trybu PA5
	GPIOA->MODER |= GPIO_MODER_MODE5_0; // ustaw 01 — wyjście


    /* Loop forever */
	while(1)
	{
		//Set pin high
		GPIOA->BSRR |= GPIO_BSRR_BS5;

		for(uint32_t i =0; i< 500000;i++); //delay :)

		//reset pin
		GPIOA->BSRR |= GPIO_BSRR_BR5;

		for(uint32_t i =0; i< 500000;i++);
	}
}
