#pragma once

void *malloc(size_t);
void free(void *);
#define RAND_MAX 0x7fffffff
void srand(unsigned int);
unsigned int rand();

float strtof(char const* _String,char** _EndPtr);

#define __disable_irq()
#define __set_MSP(x)
#define __enable_irq()
