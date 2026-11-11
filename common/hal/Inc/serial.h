#ifndef _SERIAL_H
#define _SERIAL_H

#include "hal_common.h"


HAL_Status_t halSerialInit(void);
HAL_Status_t halSerialPut(const char data);

#endif
