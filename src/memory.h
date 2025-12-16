#ifndef MEMORY_H
#define MEMORY_H

#include<stdint.h>

#define CODE_SIZE 0x10000 //ROM SIZE
#define RAM_SIZE 0x100 //RAM SIZE

extern uint8_t code_memory[CODE_SIZE];
extern uint8_t ram_memory[RAM_SIZE];


void mem_reset(void);
void mem_load(const uint8_t *data,unsigned len,uint16_t addr);

#endif