#ifndef IRQ_SERVICE_H_
#define IRQ_SERVICE_H_

#include <stdbool.h>

void IRQ_Create(void);
void IRQ_Disable(void);
void IRQ_Enable(void);
bool IRQ_IsEnabled(void);

#endif
