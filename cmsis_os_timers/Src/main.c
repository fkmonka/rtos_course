#include <stdint.h>
//#include "stm32g4xx.h" // dzieki temu plikowi zamiast adresu rejestru mozemy od razu użyć już stworzone makro
#include "cmsis_os.h"
#include <stdio.h>
#include "serial.h"
//<stringAttribute key="org.eclipse.cdt.dsf.gdb.DEBUG_NAME" value="/usr/bin/gdb-multiarch"/>
//https://arm-software.github.io/CMSIS_5/develop/RTOS2/html/group__CMSIS__RTOS__KernelCtrl.html#ga6f7764e7250c5c5364c00c45a5d1d199

static volatile osEventFlagsId_t event_id;
static volatile uint32_t cnt_master;
static volatile uint32_t cnt_slave;
static volatile uint32_t cnt_callback;

static volatile osTimerId_t timer;
static volatile osTimerId_t timer2;

void Master(void *arg)
{
		cnt_master++;
		osEventFlagsSet(event_id, 0x1); // 0x01 -flags -> flaga na najmlodszym bicie
		osDelay(100);
}

void PeriodicCallback(void* arg)
{
	cnt_callback++;
}

void Slave(void *arg)
{
	while(1)
	{
		osEventFlagsWait(event_id, 0x1, osFlagsWaitAny | osFlagsNoClear, osWaitForever); //ev id, flags, options, timeout

		osEventFlagsClear(event_id, 0x1);
		cnt_slave++;
	}
}
int main(void)
{
	halSerialInit();
	osKernelInitialize();

	event_id = osEventFlagsNew(NULL);

	//funkcja ktora ma byc wywolana, periodic - bedzie sie cyklicznie wolac; once - tylko jeden raz po uplywie czasu
	//timer trzeba osobno wywolac , nie startuje z automatu
	timer = osTimerNew(Master, osTimerPeriodic, NULL, NULL);
	timer2 = osTimerNew(PeriodicCallback, osTimerOnce, NULL, NULL);

	osThreadNew(Slave, NULL, NULL);

	osTimerStart(timer, 1000);
	osTimerStart(timer2, 500);


	printf("system ready\r\n");

	osKernelStart();

    /* Loop forever */
	while(1)
	{
	}
}
