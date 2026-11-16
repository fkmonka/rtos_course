#include <stdint.h>
//#include "stm32g4xx.h" // dzieki temu plikowi zamiast adresu rejestru mozemy od razu użyć już stworzone makro
#include "cmsis_os.h"
#include <stdio.h>
#include "serial.h"
//<stringAttribute key="org.eclipse.cdt.dsf.gdb.DEBUG_NAME" value="/usr/bin/gdb-multiarch"/>
//https://arm-software.github.io/CMSIS_5/develop/RTOS2/html/group__CMSIS__RTOS__KernelCtrl.html#ga6f7764e7250c5c5364c00c45a5d1d199

#define MASTER1_FLAG (0x01<<0) // 1 przesunieta na pozycje 0
#define MASTER2_FLAG (0x01<<1) // 1 przesunieta na pozycje 1
#define SLAVE1_FLAG (0x01<<2)
#define SLAVE2_FLAG (0x01<<3)

static volatile osEventFlagsId_t event_id;
static volatile uint32_t cnt_master_1;
static volatile uint32_t cnt_master_2;
static volatile uint32_t cnt_slave;

void Master1(void *arg)
{
	while(1)
	{
		osDelay(1000);
		cnt_master_1++;
		osEventFlagsSet(event_id, MASTER1_FLAG); // 0x01 -flags -> flaga na najmlodszym bicie
		osEventFlagsWait(event_id, SLAVE1_FLAG, osFlagsWaitAny, osWaitForever);


	}
}

void Master2(void *arg)
{
	while(1)
	{
		osDelay(500);
		cnt_master_2++;
		osEventFlagsSet(event_id, MASTER2_FLAG); // 0x10 -flags -> flaga na najmlodszym bicie
		osEventFlagsWait(event_id, SLAVE2_FLAG, osFlagsWaitAny, osWaitForever);

	}
}

void Slave(void *arg)
{
	while(1)
	{
		//czekamy na obie flagi czyli slave dziala tak jak wolniejszy master bo musi mieć obie flagi
		osEventFlagsWait(event_id, 0x1, MASTER1_FLAG | MASTER2_FLAG | osFlagsNoClear, osWaitForever); //ev id, flags, options, timeout

		osEventFlagsClear(event_id, MASTER1_FLAG | MASTER2_FLAG);
		cnt_slave++;

		//ustawiamy dwie flagi dla kazdego mastera bo funkcja wait kasuje flage
		//gdy nie ma opcji wlaczonej wiec drugi slave by sie nie odblokowal
		osEventFlagsSet(event_id, SLAVE1_FLAG | SLAVE2_FLAG);
	}
}
int main(void)
{
	halSerialInit();
	osKernelInitialize();

	event_id = osEventFlagsNew(NULL);

	osThreadNew(Master1, NULL, NULL);
	osThreadNew(Master2, NULL, NULL);
	osThreadNew(Slave, NULL, NULL);

	printf("system ready\r\n");

	osKernelStart();

    /* Loop forever */
	while(1)
	{
	}
}
