#ifndef CPU_H
#define CPU_H

#include<stdbool.h>
#include<stdint.h>

 typedef struct{
    //8 BIT REGISTER
    uint8_t A;
    uint8_t B;
    uint8_t PSW;
    uint8_t SP;

    //16 BIT REGISTER;

    uint16_t PC;
    uint16_t DPTR;

    bool running;
    
    


 }CPU8051;

extern CPU8051 cpu;

void cpu_run(uint32_t max_steps);
void cpu_reset(void);
void cpu_step(void);

#endif