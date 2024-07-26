#include <stdint.h>

#include "irqService.h"

static bool irqEnabled;
static uint32_t irqDisabledCount;

void IRQ_Create(void)
{
	//__enable_irq(); Place real implementation
	irqEnabled = true;
	irqDisabledCount = 0;
}

void IRQ_Disable(void)
{
	++irqDisabledCount;
	//__disable_irq(); Place real implementation
	irqEnabled = false;
}

void IRQ_Enable(void)
{
	if(irqDisabledCount <= 1)
	{
		irqDisabledCount = 0;
		//__enable_irq(); Place real implementation
		irqEnabled = true;
	}
	else
	{
		--irqDisabledCount;
	}
}

bool IRQ_IsEnabled(void)
{
	return irqEnabled;
}
