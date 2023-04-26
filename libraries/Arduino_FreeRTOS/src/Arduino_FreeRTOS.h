/*
 * Copyright (c) 2022 by Alexander Entinger <a.entinger@arduino.cc>
 * CAN library for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOSConfig.h"

#include "lib/FreeRTOS-Kernel-v10.5.1/FreeRTOS.h"
#include "lib/FreeRTOS-Kernel-v10.5.1/croutine.h"
#include "lib/FreeRTOS-Kernel-v10.5.1/event_groups.h"
#include "lib/FreeRTOS-Kernel-v10.5.1/queue.h"
#include "lib/FreeRTOS-Kernel-v10.5.1/semphr.h"
#include "lib/FreeRTOS-Kernel-v10.5.1/task.h"
#include "lib/FreeRTOS-Kernel-v10.5.1/timers.h"

#ifdef __cplusplus
}
#endif

// NOTE: memory_regions.ld  SOMETHING FISHY HERE, smaller sizes lead to collissions with vector table. Probably error in vector table length.
