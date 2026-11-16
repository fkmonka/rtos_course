#include <stdint.h>
//#include "stm32g4xx.h" // dzieki temu plikowi zamiast adresu rejestru mozemy od razu użyć już stworzone makro
#include "cmsis_os.h"
#include <stdio.h>
#include "serial.h"
//<stringAttribute key="org.eclipse.cdt.dsf.gdb.DEBUG_NAME" value="/usr/bin/gdb-multiarch"/>
/*https://arm-software.github.io/CMSIS_5/develop/RTOS2/html/group__CMSIS__RTOS__KernelCtrl.html#ga6f7764e7250c5c5364c00c45a5d1d199*/


void Task1()
{
	while(1)
	{
		printf("Ping\r\n");
		osDelay(500);
	}
}
osThreadAttr_t task1_attr={
		.stack_size =64
};

int main(void)
{
	halSerialInit();
	osKernelInitialize();

	osThreadNew(Task1, NULL,&task1_attr);

	osKernelStart();

    /* Loop forever */
	while(1)
	{
	}
}
