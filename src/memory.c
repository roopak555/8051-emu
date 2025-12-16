#include "memory.h"
#include "cpu.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

uint8_t code_memory[CODE_SIZE];
uint8_t ram_memory[RAM_SIZE];



void mem_reset(void){
    memset(code_memory,0,CODE_SIZE);
    memset(ram_memory,0,RAM_SIZE);


}
void mem_load(const uint8_t *data,unsigned len,uint16_t addr){
    if(addr+len>CODE_SIZE){
        fprintf(stderr,"Error:Attempt to load the progrsm beyond the  size");
        return;
    
    }
    memcpy(&code_memory[addr],data,len);

}