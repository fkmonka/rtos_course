#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#include "stm32g4xx.h" // dzieki temu plikowi zamiast adresu rejestru mozemy od razu użyć już stworzone makro
#include "cmsis_os.h"

#include "serial.h"
//<stringAttribute key="org.eclipse.cdt.dsf.gdb.DEBUG_NAME" value="/usr/bin/gdb-multiarch"/>


#define QUEUE_SIZE 4

volatile static osMessageQueueId_t queue;


void Producer1(void* args)
{
	uint32_t cnt = 0;
	while(1)
	{
		//mimo ze & to pracuje w srodku na kopii tylko;
		// jak damy zamiast osWaitForever - 0 to po prostu pomijane beda rekordy ktorych nie mozna od razu wpisac
		osMessageQueuePut(queue, &cnt, 0u, 0); //queue, val, priotity, timeout
		cnt++;
		osDelay(rand() % 500); // 0-499
	}
}

void Producer2(void* args)
{
	uint32_t cnt = 1000;
	while(1)
	{
		//mimo ze & to pracuje w srodku na kopii tylko;
		osMessageQueuePut(queue, &cnt, 0u, 0);
		cnt++;
		osDelay(rand() % 500); // 0-499
	}
}


void Consumer(void* args)
{
	uint32_t data;
	while(1)
	{
		osMessageQueueGet(queue, &data, 0u, osWaitForever);
		printf("%d\r\n", data);
		osDelay(rand() %500);
	}
}
int main(void)
{
	halSerialInit();

	osKernelInitialize();

	// KOLEJKA FIFO first in first out
	queue = osMessageQueueNew(QUEUE_SIZE, sizeof(uint32_t), NULL); //count, size , atrr

	osThreadNew(Producer1, NULL, NULL);
	osThreadNew(Producer2, NULL, NULL);
	osThreadNew(Consumer, NULL, NULL);

	printf("system ready\r\n");

	osKernelStart();

    /* Loop forever */
	while(1)
	{
	}
}
