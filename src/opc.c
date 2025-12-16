#include"opc.h"
#include"cpu.h"
#include"memory.h"

#include<stdint.h>
#include<stdbool.h>
#include<stdio.h>

typedef void (*ophandler_t)(uint8_t opcode);

static ophandler_t op_table[256];


static inline void set_carry_flag(int val){
    if(val){
        cpu.PSW |=0x01;

    }
    else{
        cpu.PSW &=~0x01;
    }
}

static inline int get_carry_flag(void) {
    return (cpu.PSW & 0x01) ? 1 : 0;
}

//CODE FOR UNIMPLEMENTED OPCODE IT MAY BE REMOVED IN FUTURE
static void op_unimpl(uint8_t opcode) {
    uint16_t pc_at = (uint16_t)(cpu.PC ? cpu.PC - 1 : 0);
    fprintf(stderr, "Unimplemented opcode 0x%02X at PC=0x%04X\n", opcode, pc_at);
    cpu.running = false;
}


//OXOO->NOP
static void op_nop(uint8_t opcode) {
    (void)opcode;
    // do nothing 
}

//0x74->MOV A,#DATA
static void op_mov_a_imm(uint8_t opcode){
    (void)opcode ;
    /* we need to make it as void other wise compiler will the
    row an error so that we typecasting into void*/
    uint8_t imm=code_memory[cpu.PC++];
    cpu.A=imm;

}

//0x24 ADD A,#DATA
static void op_add_a_imm(uint8_t opcode){
    (void)opcode ;
    uint8_t imm=code_memory[cpu.PC++];
    unsigned sum=(unsigned)imm + (unsigned)cpu.A;
    set_carry_flag(sum>0xFF);
    //TO SET THE CARRY FLAG ALSO TO FIND THE VALUE WITH THE 8 BIT
    cpu.A=(uint8_t)(sum & 0xFF);

}

//CHAT GPT GENERATED
/* 0x80 - SJMP rel */
static void op_sjmp(uint8_t opcode) {
    (void)opcode;
    int8_t rel = (int8_t)code_memory[cpu.PC++]; /* signed relative */
    cpu.PC = (uint16_t)(cpu.PC + rel);
}

/* 0x04 - INC A */
static void op_inc_a(uint8_t opcode) {
    (void)opcode;
    cpu.A = (uint8_t)((cpu.A + 1) & 0xFF);
    /* Note: INC does not affect Carry in classic 8051; other flags optional */
}

/* 0xE4 - CLR A */
static void op_clr_a(uint8_t opcode) {
    (void)opcode;
    cpu.A = 0;
}


//from A to register rn
static void op_a_to_reg_n(uint8_t opcode){
        uint8_t n=opcode &0x07; 
        uint8_t bank =(cpu.PSW>>3) &0x03;
        uint8_t addr= (bank*8)+n;
        ram_memory[addr]=cpu.A;
}

//from register n to A
static void op_reg_n_to_a(uint8_t opcode){
        uint8_t n=opcode &0x07; 
        uint8_t bank =(cpu.PSW>>3) &0x03;
        uint8_t addr= (bank*8)+n;
        cpu.A=ram_memory[addr];
}

//0xFF--Halt stop the program
static void op_halt(uint8_t opcode)
{
    (void)opcode;
    cpu.running = false;
}

// adding a and rn
static void op_add_a_rn(uint8_t opcode)
{
    uint8_t n = opcode & 0x07;
    uint8_t bank = (cpu.PSW >> 3) & 0x03;
    uint8_t addr = (bank * 8) + n;

    unsigned sum = cpu.A + ram_memory[addr];
    set_carry_flag(sum > 0xFF);
    cpu.A = (uint8_t)(sum & 0xFF);
}

// mov a , direct address
static void mov_addr_to_a(uint8_t opcode){
    (void)opcode;
    uint8_t addr=code_memory[cpu.PC++];
    cpu.A=(uint8_t)ram_memory[addr];

}

//mov direct addr ,a
static void mov_a_to_addr(uint8_t opcode){
    (void)opcode;
    uint8_t addr=code_memory[cpu.PC++];
    ram_memory[addr]=cpu.A;
}


//inc direct address
static void inc_addr(uint8_t opcode){
    (void)opcode;
    uint8_t addr=code_memory[cpu.PC++];
    uint8_t val=(uint8_t)ram_memory[addr];
    val=val+1;
    ram_memory[addr]=val;

}

//dec direct address

static void dec_addr(uint8_t opcode){
    (void)opcode;
    uint8_t addr=code_memory[cpu.PC++];
    uint8_t val=(uint8_t)ram_memory[addr];
    val=val-1;
    ram_memory[addr]=val;

}

//PUSH 
static void push(uint8_t opcode){
    (void)opcode;
    uint8_t addr=code_memory[cpu.PC++];
    uint8_t data=ram_memory[addr];
    cpu.SP++;
    ram_memory[cpu.SP]=data;
}


//POP 
static void pop(uint8_t opcode){
    (void)opcode;
    uint8_t addr=code_memory[cpu.PC++];
    ram_memory[addr]=ram_memory[cpu.SP];
    cpu.SP--;
    
}

//lcall

static void lcall(uint8_t opcode){
    (void)opcode;
    uint8_t high=code_memory[cpu.PC++];
    uint8_t low=code_memory[cpu.PC++];

    uint16_t target=(high<<8)|low;

    uint16_t ret=cpu.PC;

    cpu.SP++;
    ram_memory[cpu.SP]=(uint8_t)(ret & 0xFF);//lower 8 bit
    cpu.SP++;
    ram_memory[cpu.SP]=(uint8_t)(ret >> 8);//higher 8 bit

    cpu.PC=target;


}

//ret
static void ret(uint8_t opcode){
    (void)opcode;
    uint8_t high=(uint8_t)ram_memory[cpu.SP];
    cpu.SP--;
    uint8_t low=(uint8_t)ram_memory[cpu.SP];
    cpu.SP--;  

    uint16_t req=(uint16_t)(high <<8) | low;
    cpu.PC=req;



}






void exec_one_opcode(void){
    uint8_t opcode=code_memory[cpu.PC++];
    ophandler_t h=op_table[opcode];
    if(!h){
        op_unimpl(opcode);
        return;
    }
    h(opcode);
}

void init_opcodes(void){
    for(int i=0;i<256;i++) op_table[i]=op_unimpl;//in future if we completed the project full it will not needed

    op_table[0x00] = op_nop;
    op_table[0x74] = op_mov_a_imm;   
    op_table[0x24] = op_add_a_imm;   
    op_table[0x80] = op_sjmp;        
    op_table[0x04] = op_inc_a;       
    op_table[0xE4] = op_clr_a;       
    for(uint8_t r=0;r<8;r++){
        op_table[0xE8+r]=op_reg_n_to_a;
        op_table[0xF8+r]=op_a_to_reg_n;
        op_table[0x28 + r] = op_add_a_rn;


    } 
    op_table[0xFF] = op_halt;
    op_table[0x05] = inc_addr;
    op_table[0x15] = dec_addr;
    op_table[0xE5] = mov_addr_to_a;
    op_table[0xF5] = mov_a_to_addr;
    op_table[0xD0] = push;
    op_table[0xC0] = pop;
    op_table[0x12] = lcall;
    op_table[0x22] = ret;

    





}