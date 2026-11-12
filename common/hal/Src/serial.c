#include "serial.h"
#include "stm32g4xx.h"
#include "cmsis_helpers.h"
#include "cmsis_os.h"

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
    /*dodatkowo bo pela wyzej nie blokowala tak jak u prowadzacego*/
    while ((LPUART1->CR1 & USART_CR1_UE) == 0U)
    {
    }

    LPUART1->TDR = (uint8_t)data;

    return HAL_OK;
}

HAL_Status_t halSerialPut_Timeout(const char data, uint32_t timeout)
{
	uint32_t freq = osKernelGetTickFreq();
	uint32_t ticks_start = osKernelGetTickCount();


    while (((LPUART1->ISR & USART_ISR_TXE_TXFNF) == 0U) || ((LPUART1->CR1 & USART_CR1_UE) == 0U))
    {
        /* Czekaj na możliwość zapisania danych. */
    	if(osKernelGetTickCount() > ticks_start + timeout)
    	{
    		return HAL_TIMEOUT;
    	}
    }

    LPUART1->TDR = (uint8_t)data;

    return HAL_OK;
}

HAL_Status_t halSerialSend_Timeout(const char * txt,uint32_t timeout)
{
	while(*txt != 0) //the end of string
	{
		HAL_Status_t status = halSerialPut_Timeout(*txt,timeout);
		if(status == HAL_TIMEOUT)
		{
			return HAL_TIMEOUT;
		}
	}
	return HAL_OK;
}

HAL_Status_t halSerialSend_(const char * txt)
{
	while(*txt != 0) //the end of string
	{
		HAL_Status_t status = halSerialPut(*txt);
		if(status == HAL_TIMEOUT)
		{
			return HAL_TIMEOUT;
		}
	}
	return HAL_OK;
}
int __io_putchar(int ch)
{
	halSerialPut_Timeout(ch, 50);
	return ch;
}
