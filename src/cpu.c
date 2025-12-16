#include "cpu.h"
#include "memory.h"
#include "opc.h"
#include "debug.h"
#include<stdint.h>
#include<stdio.h>

// define the global cpu from the extern cpu.h (we used extern keyword)

CPU8051 cpu; 

void cpu_reset(void){
    cpu.A=0;
    cpu.B=0;
    cpu.PSW=0;
    cpu.SP=0x07;

    cpu.running=true;

    cpu.DPTR=0x0000;
    cpu.PC=0x0000;

    //when cpu is reset the memory is reseated and opcode should be intialized
    mem_reset();
    init_opcodes();


}

void cpu_step(void){
    if (!cpu.running)
        return;

    //  BREAKPOINT CHECK (BEFORE execution)
    if (debug_is_enabled() && debug_check_breakpoint(cpu.PC))        
{
        printf("\n");
        printf("*** BREAKPOINT HIT at PC=%04X ***\n", cpu.PC);
        printf("\n");   
        debug_set_step(true);      // enter step mode
        debug_wait_if_step();      // pause BEFORE execution
    }

    // 2. Save PC before execution
    uint16_t pc_before = cpu.PC;

    // 3. Fetch opcode
    uint8_t opcode = code_memory[cpu.PC];

    // 4. Execute instruction
    exec_one_opcode();

    // 5. Trace AFTER execution
    debug_trace(pc_before, opcode);

    // 6. STEP MODE PAUSE (every instruction)
    debug_wait_if_step();
}




void cpu_run(uint32_t max_steps){
    uint32_t t;
    for(t=0;t<max_steps && cpu.running;t++){
        cpu_step();
    }

    if(!cpu.running){

    }
}