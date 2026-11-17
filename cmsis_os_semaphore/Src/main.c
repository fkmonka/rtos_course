#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#include "stm32g4xx.h" // dzieki temu plikowi zamiast adresu rejestru mozemy od razu użyć już stworzone makro
#include "cmsis_os.h"

#include "serial.h"
//<stringAttribute key="org.eclipse.cdt.dsf.gdb.DEBUG_NAME" value="/usr/bin/gdb-multiarch"/>
static volatile uint32_t cnt1;
static volatile uint32_t cnt2;
static volatile uint32_t cnt;

/* QUEUE LIFO Last in First out*/
#define QUEUE_SIZE 4

volatile static uint32_t queue[QUEUE_SIZE];
volatile static uint32_t queue_count = 0;
volatile static uint32_t queue_err_full = 0;
volatile static uint32_t queue_err_empty = 0;

volatile osMutexId_t queue_mutex;
volatile osSemaphoreId_t queue_semaphore_filled;
volatile osSemaphoreId_t queue_semaphore_empty;

void queue_put(uint32_t data)
{
	osMutexAcquire(queue_mutex, osWaitForever);
	if(queue_count < QUEUE_SIZE)
	{
		queue[queue_count] = data;
		queue_count++;
		queue_err_full = 0;
	}
	else
	{
		queue_err_full = 1;
	}
	osMutexRelease(queue_mutex);
}

uint32_t queue_pop()
{
	osMutexAcquire(queue_mutex, osWaitForever);
	uint32_t data =0;
	if(queue_count > 0)
	{
		data = queue[queue_count-1];
		queue_count--;
		queue_err_empty = 0;
	}
	else
	{
		queue_err_empty = 1;
		data = 0;
	}
	osMutexRelease(queue_mutex);
	return data;
}

void Producer(void* args)
{
	uint32_t cnt = 0;
	while(1)
	{
		//empty starts at 4 and can put only 4 without poping
		osSemaphoreAcquire(queue_semaphore_empty, osWaitForever);
		queue_put(cnt);
		if(queue_err_full == 0)
		{
			cnt++;
			osSemaphoreRelease(queue_semaphore_filled);
		}
		osDelay(rand() % 300);
	}
}

void Consumer(void* args)
{
	while(1)
	{
		//mamy gwarancje ze osAcquaire nie zostanie rzerwane - jak atomowa operacja przez os traktowane
		osSemaphoreAcquire(queue_semaphore_filled, osWaitForever);
		uint32_t data =queue_pop();
		if(queue_err_empty == 0)
		{
			printf("%d\r\n",data);
		}
		osDelay(rand() % 900);
		osSemaphoreRelease(queue_semaphore_empty);
	}
}
int main(void)
{
	halSerialInit();

	osKernelInitialize();

	queue_mutex = osMutexNew(NULL);

	queue_semaphore_filled = osSemaphoreNew(QUEUE_SIZE, 0, NULL); //max count, start val, ptr na struct atrybut
	queue_semaphore_empty = osSemaphoreNew(QUEUE_SIZE,QUEUE_SIZE,NULL);
	osThreadNew(Producer, NULL, NULL);
	osThreadNew(Consumer, NULL, NULL);

	printf("system ready\r\n");

	osKernelStart();

    /* Loop forever */
	while(1)
	{
	}
}
