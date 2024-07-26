#ifndef __task_h
#define __task_h

#include <stdbool.h>
#include <stdint.h>

#define MAX_TASK_QUEUE	20
#define MAX_DELAYED_TASK_QUEUE 20

typedef void (*TASK)(void);

typedef struct
{
	volatile uint32_t time;
	volatile TASK task;
}
 TaskTimerTypeDef;
 
void Task_Idle(void);
void Task_Init(void);
void Task_TimersHandler(void);
void Task_Handler(void);
bool Task_Add(TASK task);
void Task_Delete(TASK task);
bool Task_IsEnqued(TASK task);
bool Task_IsQueueEmpty(void);
bool Task_AddDelayed(const uint32_t time, TASK task);
bool Task_IsEnquedDelayed(TASK task);
uint32_t Task_GetDelayedTime(TASK task);

bool Task_IsTaskQueueEmpty(void);
const TASK * Task_GetTaskQueue(void);
const TaskTimerTypeDef * Task_GetDelayedTaskQueue(void);
uint32_t Task_GetDelayedTaskTime(TASK task);

#endif
