#include <stdlib.h>
#include "instruction.h"
#include "machine_types.h"
#include "main.h"
#include "utilities.h"
#include "vm_instructions.h"
#include "vm_memory.h"
#include "vm_registers.h"

void pstr(bin_instr_t instruction)
{
    GPR[2] = printf("%s", &Memory.words[GPR[4]]);
}

void pch(bin_instr_t instruction)
{
    GPR[2] = fputc(GPR[4], stdout);
}

void rch(bin_instr_t instruction)
{
    GPR[2] = getc(stdin);
}

void systemcallInstructionExecute(bin_instr_t instruction)
{
    switch (instruction.syscall.code)
    {
    case 4:
    {
        pstr(instruction);
        break;
    }
    case 10:
    {
        exit(0);
        break;
    }
    case 11:
    {
        pch(instruction);
        break;
    }
    case 12:
    {
        rch(instruction);
        break;
    }
    case 256:
    {
        tracing = 1;
        break;
    }
    case 257:
    {
        tracing = 0;
        break;
    }
    }
}

void sll(bin_instr_t instruction)
{
    GPR[instruction.reg.rd] = GPR[instruction.reg.rt] << instruction.reg.shift;
}

void srl(bin_instr_t instruction)
{
    GPR[instruction.reg.rd] = GPR[instruction.reg.rt] >> instruction.reg.shift;
}

void jr(bin_instr_t instruction)
{
    PC = GPR[instruction.reg.rs];
}

void mfhi(bin_instr_t instruction)
{
    GPR[instruction.reg.rd] = HI;
}

void mflo(bin_instr_t instruction)
{
    GPR[instruction.reg.rd] = LO;
}

void mul(bin_instr_t instruction)
{
    long product = GPR[instruction.reg.rs] * GPR[instruction.reg.rt];
    HI = (product & 0xFFFFFFFF00000000) >> 32;
    LO = product & 0x00000000FFFFFFFF;
}

void divInstruction(bin_instr_t instruction)
{
    HI = GPR[instruction.reg.rs] % GPR[instruction.reg.rt];
    LO = GPR[instruction.reg.rs] / GPR[instruction.reg.rt];
}

void add(bin_instr_t instruction)
{
    GPR[instruction.reg.rd] = GPR[instruction.reg.rs] + GPR[instruction.reg.rt];
}

void sub(bin_instr_t instruction)
{
    GPR[instruction.reg.rd] = GPR[instruction.reg.rs] - GPR[instruction.reg.rt];
}

void andInstruction(bin_instr_t instruction)
{
    GPR[instruction.reg.rd] = GPR[instruction.reg.rs] & GPR[instruction.reg.rt];
}

void bor(bin_instr_t instruction)
{
    GPR[instruction.reg.rd] = GPR[instruction.reg.rs] | GPR[instruction.reg.rt];
}

void xorInstruction(bin_instr_t instruction)
{
    GPR[instruction.reg.rd] = GPR[instruction.reg.rs] ^ GPR[instruction.reg.rt];
}

void nor(bin_instr_t instruction)
{
    GPR[instruction.reg.rd] = ~(GPR[instruction.reg.rs] | GPR[instruction.reg.rt]);
}

void registerInstructionExecute(bin_instr_t instruction)
{
    switch (instruction.reg.func)
    {
    case 0:
        sll(instruction);
        break;
    case 3:
        srl(instruction);
        break;
    case 8:
        jr(instruction);
        break;
    case 12:
        systemcallInstructionExecute(instruction);
        break;
    case 16:
        mfhi(instruction);
        break;
    case 18:
        mflo(instruction);
        break;
    case 25:
        mul(instruction);
        break;
    case 27:
        divInstruction(instruction);
        break;
    case 33:
        add(instruction);
        break;
    case 35:
        sub(instruction);
        break;
    case 36:
        andInstruction(instruction);
        break;
    case 37:
        bor(instruction);
        break;
    case 38:
        xorInstruction(instruction);
        break;
    case 39:
        nor(instruction);
        break;
    }
}

void bgez(bin_instr_t instruction)
{
    if (GPR[instruction.immed.rs] >= 0)
    {
        PC = PC + machine_types_formOffset(instruction.immed.immed);
    }
}

void beq(bin_instr_t instruction)
{
    if (GPR[instruction.immed.rs] == GPR[instruction.immed.rt])
    {
        PC = PC + machine_types_formOffset(instruction.immed.immed);
    }
}

void bne(bin_instr_t instruction)
{
    if (GPR[instruction.immed.rs] != GPR[instruction.immed.rt])
    {
        PC = PC + machine_types_formOffset(instruction.immed.immed);
    }
}

void blez(bin_instr_t instruction)
{
    if (GPR[instruction.immed.rs] <= 0)
    {
        PC = PC + machine_types_formOffset(instruction.immed.immed);
    }
}

void bgtz(bin_instr_t instruction)
{
    if (GPR[instruction.immed.rs] > 0)
    {
        PC = PC + machine_types_formOffset(instruction.immed.immed);
    }
}

void bltz(bin_instr_t instruction)
{
    if (GPR[instruction.immed.rs] < 0)
    {
        PC = PC + machine_types_formOffset(instruction.immed.immed);
    }
}

void addi(bin_instr_t instruction)
{
    GPR[instruction.immed.rt] = GPR[instruction.immed.rs] + machine_types_sgnExt(instruction.immed.immed);
}

void andi(bin_instr_t instruction)
{
    GPR[instruction.immed.rt] = GPR[instruction.immed.rs] & machine_types_zeroExt(instruction.immed.immed);
}

void bori(bin_instr_t instruction)
{
    GPR[instruction.immed.rt] = GPR[instruction.immed.rs] | machine_types_zeroExt(instruction.immed.immed);
}

void xori(bin_instr_t instruction)
{
    GPR[instruction.immed.rt] = GPR[instruction.immed.rs] ^ machine_types_zeroExt(instruction.immed.immed);
}

void lw(bin_instr_t instructions)
{
    GPR[instructions.immed.rt] = Memory.words[(GPR[instructions.immed.rs] + machine_types_formOffset(instructions.immed.immed)) / BYTES_PER_WORD];
}

void lbu(bin_instr_t instruction)
{
    GPR[instruction.immed.rt] = machine_types_zeroExt(Memory.bytes[GPR[instruction.immed.rs] + machine_types_formOffset(instruction.immed.immed)]);
}

void sb(bin_instr_t instructions)
{
    Memory.bytes[GPR[instructions.immed.rs] + machine_types_formOffset(instructions.immed.immed)] = GPR[instructions.immed.rt];
}

void sw(bin_instr_t instructions)
{
    Memory.words[(GPR[instructions.immed.rs] + machine_types_formOffset(instructions.immed.immed)) / BYTES_PER_WORD] = GPR[instructions.immed.rt];
}

void immediateInstructionExecute(bin_instr_t instruction)
{
    switch (instruction.immed.op)
    {
    case 1:
        bgez(instruction);
        break;
    case 4:
        beq(instruction);
        break;
    case 5:
        bne(instruction);
        break;
    case 6:
        blez(instruction);
        break;
    case 7:
        bgtz(instruction);
        break;
    case 8:
        bltz(instruction);
        break;
    case 9:
        addi(instruction);
        break;
    case 12:
        andi(instruction);
        break;
    case 13:
        bori(instruction);
        break;
    case 14:
        xori(instruction);
        break;
    case 35:
        lw(instruction);
        break;
    case 36:
        lbu(instruction);
        break;
    case 40:
        sb(instruction);
        break;
    case 43:
        sw(instruction);
        break;
    }
}

void jmp(bin_instr_t instruction)
{
    PC = machine_types_formAddress(PC, instruction.jump.addr);
}

void jal(bin_instr_t instruction)
{
    GPR[31] = PC;
    PC = machine_types_formAddress(PC, instruction.jump.addr);
}

void jumpInstructionExecute(bin_instr_t instruction)
{
    switch (instruction.jump.op)
    {
    case 2:
        jmp(instruction);
        break;
    case 3:
        jal(instruction);
        break;
    }
}

void executeInstruction(bin_instr_t instruction)
{
    instr_type type = instruction_type(instruction);

    switch (type)
    {
    case 0:
        registerInstructionExecute(instruction);
        break;
    case 1:
        systemcallInstructionExecute(instruction);
        break;
    case 2:
        immediateInstructionExecute(instruction);
        break;
    case 3:
        jumpInstructionExecute(instruction);
        break;
    case 4:
        bail_with_error("Error instruction");
        break;
    }
}