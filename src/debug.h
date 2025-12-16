#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include <stdbool.h>

/* Enable / disable debugger */
void debug_enable(bool enable);

/* Print CPU state after each instruction */
void debug_trace(uint16_t pc_before, uint8_t opcode);

void debug_set_step(bool enable);
void debug_wait_if_step(void);
void debug_set_breakpoint(uint16_t addr);
void debug_clear_breakpoint(void);
bool debug_check_breakpoint(uint16_t pc);

void debug_print_mnemonic(uint16_t pc, uint8_t opcode);


/* Optional helpers */
void debug_dump_registers(void);
void debug_dump_memory(uint8_t start, uint8_t end);

#endif
