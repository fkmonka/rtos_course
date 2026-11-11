#include "serial.h"
#include "stm32g4xx.h"
#include "cmsis_helpers.h"

HAL_Status_t halSerialInit(void)
{
	/*Turn on GPIO clock*/
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	/*Configure GPIO pins for UART function*/
			// register, bit position =2, alternate function
	FILL_REG(GPIOA->MODER, GPIO_MODER_MODE2, 2U); //GPIO_MODER_AF);
	FILL_REG(GPIOA->MODER, GPIO_MODER_MODE3, 2U); //GPIO_MODER_AF);
	//GPIOA->MODER |= GPIO_MODER_MODE2_1; // starszy bit = 1
	//GPIOA->MODER &= ~(GPIO_MODER_MODE2_0); // młodszy bit = 0

	/* Set alternate function type to AF12 for PA2 (LPUART_TX) and PA3 (LPUART_RX)*/
	FILL_REG(GPIOA->AFR[0], GPIO_AFRL_AFSEL2, 12); //0b1100
	FILL_REG(GPIOA->AFR[0], GPIO_AFRL_AFSEL3, 12);

	/*Turn-on clock for UART module*/
	RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;

	/*Configure UART module*/
	/*config uart baudrate*/
	uint32_t uart_div = (uint32_t)(
		    (256ULL * CONFIG_APB1_CLOCK + CONFIG_SERIAL_CLOCK / 2U)
		    / CONFIG_SERIAL_CLOCK
		);

	LPUART1->BRR = uart_div;
	volatile uint32_t debug_brr = LPUART1->BRR;
	/**/
	LPUART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);

	/*Turn-on UART module*/
	LPUART1->CR1 |= USART_CR1_UE;
	return HAL_OK;
}

HAL_Status_t halSerialPut(const char data)
{
    while ((LPUART1->ISR & USART_ISR_TXE_TXFNF) == 0U)
    {
        /* Czekaj na możliwość zapisania danych. */
    }

    LPUART1->TDR = (uint8_t)data;

    return HAL_OK;
}

int __io_putchar(int ch)
{
	halSerialPut(ch);
	return ch;
}
