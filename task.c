#include "task.h"
#include "irqService.h"

static TaskTimerTypeDef TaskTimersQueue[MAX_DELAYED_TASK_QUEUE];
static TASK	TaskQueue[MAX_TASK_QUEUE];

void Task_Idle(void)
{
}

static bool IsTaskIdle(TASK task)
{
	return task == Task_Idle;
}

static void Task_TimersInit(void)
{
	for(uint32_t i = 0; i < MAX_DELAYED_TASK_QUEUE; ++i)
	{
		TaskTimersQueue[i].time = 0;
		TaskTimersQueue[i].task = Task_Idle;
	}
}

static void ShiftQueue(void)
{
	for(register uint32_t index = 0; index < MAX_TASK_QUEUE - 1; ++index)
	{
		TaskQueue[index] = TaskQueue[index + 1];
		if (TaskQueue[index + 1] == Task_Idle)
		{
			break;
		}
	}
	TaskQueue[MAX_TASK_QUEUE - 1] = Task_Idle;
}

bool Task_IsEnqued(TASK task)
{
	for (int i = 0; i < MAX_TASK_QUEUE; ++i)
	{
		if (TaskQueue[i] == task) return true;
	}
	return false;
}

void Task_TimersHandler(void)
{
	for(uint32_t i = 0; i < MAX_DELAYED_TASK_QUEUE; ++i)
	{
		if(IsTaskIdle(TaskTimersQueue[i].task)) continue;
		else 
		{
			if(TaskTimersQueue[i].time == 0)
			{
				Task_Add(TaskTimersQueue[i].task);
				TaskTimersQueue[i].task = Task_Idle;
			}
			else
			{
				--TaskTimersQueue[i].time;
			}
		}
	}
}

void Task_QueueInit(void)
{
	for(uint32_t i = 0; i < MAX_TASK_QUEUE; ++i)
	{
		TaskQueue[i] = Task_Idle;
	}
}

void Task_Init(void)
{
	Task_QueueInit();
	Task_TimersInit();
}

bool Task_Add(TASK task)
{
	if (Task_IsEnqued(task)) return false;

	IRQ_Disable();

	for(register uint32_t i = 0; i < MAX_TASK_QUEUE; ++i)
	{
		if(IsTaskIdle(TaskQueue[i]))
		{
			TaskQueue[i] = task;
			IRQ_Enable();
			return true;
		}
	}

	IRQ_Enable();
	return false;
}

void Task_Delete(TASK task)
{
	for (uint32_t i = 0; i < MAX_TASK_QUEUE; ++i)
	{
		if(TaskTimersQueue[i].task == task)
		{
			TaskTimersQueue[i].task = Task_Idle;
			TaskTimersQueue[i].time = 0;
		}
		if(TaskQueue[i] == task)
		{
			TaskQueue[i] = Task_Idle;
		}
		if(IsTaskIdle(TaskQueue[i]))
		{
			for(register uint32_t index = i; index < MAX_TASK_QUEUE - 1; ++index)
			{
				TaskQueue[index] = TaskQueue[index + 1];
				if (TaskQueue[index + 1] == Task_Idle)
				{
					break;
				}
			}
			TaskQueue[MAX_TASK_QUEUE - 1] = Task_Idle;
		}
	}
}

bool Task_AddDelayed(const uint32_t time, TASK task)
{
	for(uint32_t i = 0; i < MAX_DELAYED_TASK_QUEUE; ++i)
	{
		if(TaskTimersQueue[i].task == task)
		{
			TaskTimersQueue[i].time = time;
			return true; 
		}
		else if(TaskTimersQueue[i].task == Task_Idle)
		{
			TaskTimersQueue[i].time = time;
			TaskTimersQueue[i].task = task;
			return true; 
		}
	}
	return false;
}

bool Task_IsEnquedDelayed(TASK task)
{
	for (int i = 0; i < MAX_DELAYED_TASK_QUEUE; ++i)
	{
		if (TaskTimersQueue[i].task == task)
			return true;
	}
	return false;
}

uint32_t Task_GetDelayedTime(TASK task)
{
	for (int i = 0; i < MAX_DELAYED_TASK_QUEUE; ++i)
	{
		if (TaskTimersQueue[i].task == task)
			return TaskTimersQueue[i].time;
	}
	return 0;
}

void Task_Handler(void)
{
	IRQ_Disable();
	TASK GoToTask = TaskQueue[0];

	if(!IsTaskIdle(GoToTask))
	{
		ShiftQueue();
	}
	IRQ_Enable();
 	(GoToTask)();
}

const TASK * Task_GetTaskQueue(void)
{
	return TaskQueue;
}

const TaskTimerTypeDef * Task_GetDelayedTaskQueue(void)
{
	return TaskTimersQueue;
}

uint32_t Task_GetDelayedTaskTime(TASK task)
{
	for(int i = 0; i < MAX_DELAYED_TASK_QUEUE; ++i)
	{
		if(TaskTimersQueue[i].task == task)
			return TaskTimersQueue[i].time;
	}
	return 0;
}

bool Task_IsTaskQueueEmpty(void)
{
	for(uint32_t i = 0; i < MAX_TASK_QUEUE; ++i)
	{
		if(!IsTaskIdle(TaskQueue[i]))
		{
			return false;
		}
	}
	return true;
}
