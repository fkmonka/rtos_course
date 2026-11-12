#include <stdint.h>
#include <stdio.h>
#include "stm32g4xx.h" // dzieki temu plikowi zamiast adresu rejestru mozemy od razu użyć już stworzone makro
#include "cmsis_os.h"
#include "serial.h"
//change debug tool in .launch file:
//<stringAttribute key="org.eclipse.cdt.dsf.gdb.DEBUG_NAME" value="/usr/bin/gdb-multiarch"/>

/*https://arm-software.github.io/CMSIS_5/develop/RTOS2/html/group__CMSIS__RTOS__KernelCtrl.html#ga6f7764e7250c5c5364c00c45a5d1d199*/

static volatile uint32_t cnt1;
static volatile uint32_t err_cnt1;

void Task1()
{
	uint32_t freq = osKernelGetTickFreq(); //frequency in HZ congigured in Inc/FreeRTOSConfig.h = 1000

	while(1)
	{
		//printf("Ping\r\n");

		HAL_Status_t status = halSerialSend_Timeout("Ping\r\n", 100);
		if(status ==HAL_OK)
		{
			cnt1++;
		}
		else
		{
			err_cnt1++;
		}
		osDelay(freq);
	}
}

void Task2()
{
	uint32_t freq = osKernelGetTickFreq();
	osDelay(5*freq); //5s

	//wyłączenie uartu na cele cwiczenia, żeby zobaczyc co sie stanie z 1
	LPUART1->CR1 &= ~(USART_CR1_UE);

	while(1)
		{

		}

}
int main(void)
{
	halSerialInit();
	//printf("TEST\r\n");
	halSerialSend("TEST\r\n");
	osKernelInitialize();

	osThreadNew(Task1, NULL,NULL);
	osThreadNew(Task2, NULL,NULL);
	osKernelStart();

    /* Loop forever */
	while(1)
	{
	}
}
