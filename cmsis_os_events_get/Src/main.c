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

void Master(void *arg)
{
	while(1)
	{
		osDelay(1000);
		cnt_master++;
		osEventFlagsSet(event_id, 0x1); // 0x01 -flags -> flaga na najmlodszym bicie


	}
}

void Slave(void *arg)
{
	uint32_t flags;
	while(1)
	{
		osEventFlagsWait(event_id, 0x1, osFlagsWaitAny | osFlagsNoClear, osWaitForever); //ev id, flags, options, timeout

		/*  //polling - nie jest to najlepsze rozwiazanie
		do
		{
			flags = osEventFlagsGet(event_id); //aktualny status wszystkich flag?
			osDelay(1); //Zablokowanie slave-a by oddac czas dla mastera zeby go nie zaglodzic, ale wtedy mamy opoznienie
		}while((flags & 0x01) == 0);
		 */
		osEventFlagsClear(event_id, 0x1);
		cnt_slave++;

	}
}
/*Wysoki priorytet dla slave zeby szybko mogl reagowac na zmiane*/
osThreadAttr_t slave_attr =
{
		.priority = osPriorityHigh7
};

int main(void)
{
	halSerialInit();
	osKernelInitialize();

	event_id = osEventFlagsNew(NULL);

	osThreadNew(Master, NULL, NULL);
	osThreadNew(Slave, NULL, &slave_attr);

	printf("system ready\r\n");

	osKernelStart();

    /* Loop forever */
	while(1)
	{
	}
}
