#include <stdint.h>
//#include "stm32g4xx.h" // dzieki temu plikowi zamiast adresu rejestru mozemy od razu użyć już stworzone makro
#include "cmsis_os.h"
#include <stdio.h>
#include "serial.h"

static volatile uint32_t cnt1;
static volatile uint32_t cnt2;

void Task1(void* args)
{
	while(1)
	{
		cnt1++;
	}
}

void Task2(void* args)
{
	while(1)
	{
		cnt2++;
	}
}
int main(void)
{
	halSerialInit();
	halSerialPut('A');
	halSerialPut('\r');
	halSerialPut('\n');
	osKernelInitialize();
	osThreadNew(Task1, NULL, NULL);
	osThreadNew(Task2, NULL, NULL);

	printf("system ready\r\n");

	osKernelStart();

    /* Loop forever */
	while(1)
	{
	}
}
