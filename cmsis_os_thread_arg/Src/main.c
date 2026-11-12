#include <stdint.h>
//#include "stm32g4xx.h" // dzieki temu plikowi zamiast adresu rejestru mozemy od razu użyć już stworzone makro
#include "cmsis_os.h"
#include <stdio.h>
#include "serial.h"
//<stringAttribute key="org.eclipse.cdt.dsf.gdb.DEBUG_NAME" value="/usr/bin/gdb-multiarch"/>
static volatile uint32_t cnt1;
static volatile uint32_t cnt2;

typedef struct
{
	uint32_t *ptr;
	uint32_t step;
}TaskFun_args_t;

void TaskFun(void* args)
{
	TaskFun_args_t * task_args = (TaskFun_args_t*)args;
	uint32_t step = task_args->step;
	uint32_t * ptr = task_args->ptr;
	while(1)
	{
		(*ptr)+=step;
	}
}
/* Global variables remain alive for the entire program execution,
   so their addresses can be safely passed to the task function. */
TaskFun_args_t task1_args;
TaskFun_args_t task2_args;

int main(void)
{
	halSerialInit();
	osKernelInitialize();

	task1_args.ptr = &cnt1;
	task1_args.step = 1;

	task2_args.ptr = &cnt2;
	task2_args.step = 2;

	/* oba watki obslugiwane przez funkcje TaskFun , ale z różnymi argumentami*/
	osThreadNew(TaskFun, &task1_args, NULL);
	osThreadNew(TaskFun, &task2_args, NULL);

	printf("system ready\r\n");

	osKernelStart();

    /* Loop forever */
	while(1)
	{
	}
}
