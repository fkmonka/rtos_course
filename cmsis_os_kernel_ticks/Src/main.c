#include <stdint.h>
//#include "stm32g4xx.h" // dzieki temu plikowi zamiast adresu rejestru mozemy od razu użyć już stworzone makro
#include "cmsis_os.h"
#include <stdio.h>
#include "serial.h"

/*https://arm-software.github.io/CMSIS_5/develop/RTOS2/html/group__CMSIS__RTOS__KernelCtrl.html#ga6f7764e7250c5c5364c00c45a5d1d199*/

static volatile uint32_t cnt1;
static volatile uint32_t cnt2;

void Task1()
{
	uint32_t freq = osKernelGetTickFreq(); //frequency in HZ congigured in Inc/FreeRTOSConfig.h = 1000

	while(1)
	{
		cnt1++;
		uint32_t ticks_start = osKernelGetTickCount();

		while(osKernelGetTickCount()< ticks_start + freq)
		{

		}
	}
}

void Task2()
{
	uint32_t freq = osKernelGetTickFreq(); //frequency in HZ congigured in Inc/FreeRTOSConfig.h = 1000

	while(1)
	{
		cnt2++;
		osDelay(freq);
	}
}

int main(void)
{
	halSerialInit();

	osKernelInitialize();

	osThreadNew(Task1, NULL,NULL);
	osThreadNew(Task2, NULL,NULL);
	osKernelStart();

    /* Loop forever */
	while(1)
	{
	}
}
