// src/main.c
#include <stdio.h>
#include <stdint.h>

#include "cpu.h"
#include "memory.h"
#include "opc.h"
#include "debug.h"

static void print_banner(void)
{
    printf("\n");
    printf(" █████  ████   ██████  █████  \n");
    printf(" █   █ █    █  ██      █  ██  \n");
    printf(" █████ █    █  ██████     ██  \n");
    printf(" █   █ █    █       █     ██  \n");
    printf(" █████  ████   ██████   █████ \n");
    printf("\n");
    printf("      8051 MICROCONTROLLER EMULATOR\n");
    printf("      --------------------------------\n");
    printf("      Language: C\n");
    printf("      Mode    : Debug / Step / Breakpoint\n");
    printf("\n");
}




//print CPU status 
static void cpu_status(void) {
    
    printf("\n=== CPU STATE ===\n");
    printf("PC  = 0x%04X\n", (unsigned)cpu.PC);
    printf("A   = 0x%02X (%u)\n", (unsigned)cpu.A, (unsigned)cpu.A);
    printf("B   = 0x%02X\n", (unsigned)cpu.B);
    printf("PSW = 0x%02X\n", (unsigned)cpu.PSW);
    printf("SP  = 0x%02X\n", (unsigned)cpu.SP);
    printf("DPTR= 0x%04X\n", (unsigned)cpu.DPTR);
    printf("running = %s\n", cpu.running ? "true" : "false");
    printf("register =0x%02x\n",(unsigned)ram_memory[5]);
    printf("=================\n");
}
int main(int argc,char **argv){
    (void) argc;
    (void) argv;

    print_banner();

    cpu_reset();
    debug_enable(true);
    debug_set_step(false);   

    debug_set_breakpoint(0x0006);   


    const uint8_t sample_prog[]={
    0x74, 0x11,       // MOV A,#11h
    0xF5, 0x30,       // MOV 30h,A
    0xD0, 0x30,       // PUSH 30h

    0x12, 0x01, 0x00, // LCALL 0100h

    0xC0, 0x31,       // POP 31h
    0xE5, 0x31,       // MOV A,31h
    0xFF,             // HALT

    // ---- padding until 0x0100 ----
    [0x0100] = 0x74,  // MOV A,#AAh
    [0x0101] = 0xAA,
    [0x0102] = 0x22  // RET
    };
   mem_load(sample_prog, sizeof(sample_prog), 0x0000);

   cpu_run(100);

   cpu_status();

   return 0;



}
