#include <stdint.h>
//#include "stm32g4xx.h" // dzieki temu plikowi zamiast adresu rejestru mozemy od razu użyć już stworzone makro
#include "cmsis_os.h"
#include <stdio.h>
#include "serial.h"

/*https://arm-software.github.io/CMSIS_5/develop/RTOS2/html/group__CMSIS__RTOS__KernelCtrl.html#ga6f7764e7250c5c5364c00c45a5d1d199*/
void info (void) {
  char infobuf[100];
  osVersion_t osv;
  osStatus_t status;

  status = osKernelGetInfo(&osv, infobuf, sizeof(infobuf));
  if(status == osOK) {
    printf("Kernel Information: %s\r\n", infobuf);
    printf("Kernel Version    : %ld\r\n", osv.kernel);
    printf("Kernel API Version: %lu\r\n", osv.api);
  }
}

int main(void)
{
	halSerialInit();

	osKernelInitialize();

	info();

	osKernelStart();

    /* Loop forever */
	while(1)
	{
	}
}
