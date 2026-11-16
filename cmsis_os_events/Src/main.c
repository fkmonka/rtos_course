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
	while(1)
	{
		osEventFlagsWait(event_id, 0x1, osFlagsWaitAny | osFlagsNoClear, osWaitForever); //ev id, flags, options, timeout
		/*gdyby bylo wiecej ustawionych wiecej flag - czy wystarczy jedna podniesiona flaga
		 * czy musi byc kilka -> to okreslaja opcje osFlagsWaitAny -default wystarczy jedna flaga podniesione
		 *  ..All - wszystkie flagi,
		 *  ..NoClear - czy maja byc opuszcozne flagi po wykonaniu funkcji? jeden slave moglby odebrac flage
		 *   i ja zdjac a drugi ktory tez czeka na flage moglby juz jej nie dostac
		 */
		cnt_slave++;
	}
}
int main(void)
{
	halSerialInit();
	osKernelInitialize();

	event_id = osEventFlagsNew(NULL);

	osThreadNew(Master, NULL, NULL);
	osThreadNew(Slave, NULL, NULL);

	printf("system ready\r\n");

	osKernelStart();

    /* Loop forever */
	while(1)
	{
	}
}
