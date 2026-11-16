#include <stdint.h>
//#include "stm32g4xx.h" // dzieki temu plikowi zamiast adresu rejestru mozemy od razu użyć już stworzone makro
#include "cmsis_os.h"
#include <stdio.h>
#include "serial.h"

static volatile uint32_t cnt1;
static volatile uint32_t cnt2;
static volatile uint32_t cnt;

static volatile osMutexId_t mutex;

void Task1(void* args)
{
	while(1)
	{
		cnt1++;

		if(cnt1 <= 100000)
		{
			osMutexAcquire(mutex, osWaitForever); //mutex, timeout
			cnt++;
			osMutexRelease(mutex);
			//mutex zablokowany dopoki nie
			//zostanie zwolniony w tym samym tasku co było acquire
		}
	}
}

void Task2(void* args)
{
	while(1)
	{
		cnt2++;

		if(cnt2 <= 100000)
		{
			osMutexAcquire(mutex, osWaitForever);
			cnt++;
			osMutexRelease(mutex);
		}
	}
}
int main(void)
{
	halSerialInit();

	osKernelInitialize();

	mutex = osMutexNew(NULL);

	osThreadNew(Task1, NULL, NULL);
	osThreadNew(Task2, NULL, NULL);

	printf("system ready\r\n");

	osKernelStart();

    /* Loop forever */
	while(1)
	{
	}
}
