#include "debug.h"
#include "cpu.h"
#include "memory.h"

#include <stdio.h>

static bool debug_on = false;

void debug_enable(bool enable)
{
    debug_on = enable;
}

void debug_trace(uint16_t pc_before, uint8_t opcode)
{
    if (!debug_on)
        return;

    printf("PC=%04X  ", pc_before);
    debug_print_mnemonic(pc_before, opcode);
    printf("   | A=%02X B=%02X PSW=%02X SP=%02X DPTR=%04X\n",
           cpu.A, cpu.B, cpu.PSW, cpu.SP, cpu.DPTR);
}

static bool step_mode = false;

void debug_set_step(bool enable)
{
    step_mode = enable;
}

void debug_wait_if_step(void)
{
    if (!step_mode)
        return;

    printf("Press Enter to continue...");
    fflush(stdout);
    getchar();
}
static bool breakpoint_enabled = false;
static uint16_t breakpoint_addr = 0;

void debug_set_breakpoint(uint16_t addr)
{
    breakpoint_addr = addr;
    breakpoint_enabled = true;
}

void debug_clear_breakpoint(void)
{
    breakpoint_enabled = false;
}

bool debug_check_breakpoint(uint16_t pc)
{
    return breakpoint_enabled && (pc == breakpoint_addr);
}

void debug_dump_registers(void)
{
    printf("=== REGISTERS ===\n");
    printf("A   = %02X\n", cpu.A);
    printf("B   = %02X\n", cpu.B);
    printf("PSW = %02X\n", cpu.PSW);
    printf("SP  = %02X\n", cpu.SP);
    printf("PC  = %04X\n", cpu.PC);
    printf("DPTR= %04X\n", cpu.DPTR);
}

void debug_dump_memory(uint8_t start, uint8_t end)
{
    printf("=== MEMORY [%02X-%02X] ===\n", start, end);
    for (uint8_t i = start; i <= end; i++)
    {
        printf("%02X:%02X  ", i, ram_memory[i]);
        if ((i & 0x0F) == 0x0F)
            printf("\n");
    }
    printf("\n");
}
void debug_print_mnemonic(uint16_t pc, uint8_t opcode)
{
    switch (opcode)
    {
        case 0x00:
            printf("NOP");
            break;

        case 0x74:
            printf("MOV A,#%02Xh", code_memory[pc + 1]);
            break;

        case 0x24:
            printf("ADD A,#%02Xh", code_memory[pc + 1]);
            break;

        case 0x04:
            printf("INC A");
            break;

        case 0xE4:
            printf("CLR A");
            break;

        case 0x80:
            printf("SJMP %02Xh", code_memory[pc + 1]);
            break;

        case 0xFF:
            printf("HALT");
            break;

        case 0x05:
            printf("INC %02Xh", code_memory[pc + 1]);
            break;

        case 0x15:
            printf("DEC %02Xh", code_memory[pc + 1]);
            break;

        case 0xE5:
            printf("MOV A,%02Xh", code_memory[pc + 1]);
            break;

        case 0xF5:
            printf("MOV %02Xh,A", code_memory[pc + 1]);
            break;

        case 0xC0:
            printf("POP %02Xh", code_memory[pc + 1]);
            break;

        case 0xD0:
            printf("PUSH %02Xh", code_memory[pc + 1]);
            break;

        case 0x12:
            printf("LCALL %02X%02Xh",
                   code_memory[pc + 1],
                   code_memory[pc + 2]);
            break;

        case 0x22:
            printf("RET");
            break;

        default:
            /* MOV A,Rn : E8–EF */
            if ((opcode & 0xF8) == 0xE8)
            {
                printf("MOV A,R%d", opcode & 0x07);
            }
            /* MOV Rn,A : F8–FF (except FF which is HALT) */
            else if ((opcode & 0xF8) == 0xF8 && opcode != 0xFF)
            {
                printf("MOV R%d,A", opcode & 0x07);
            }
            /* ADD A,Rn : 28–2F */
            else if ((opcode & 0xF8) == 0x28)
            {
                printf("ADD A,R%d", opcode & 0x07);
            }
            else
            {
                printf("DB %02Xh", opcode);
            }
            break;
    }
}