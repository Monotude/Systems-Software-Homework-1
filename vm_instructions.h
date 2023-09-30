#include "instruction.h"
#include "machine_types.h"

extern void executeInstruction(bin_instr_t instruction);
extern void registerInstructionExecute(bin_instr_t instruction);
extern void add(bin_instr_t instruction);
extern void sub(bin_instr_t instruction);
extern void mul(bin_instr_t instruction);
extern void div(bin_instr_t instruction);
extern void mfhi(bin_instr_t instruction);
extern void mflo(bin_instr_t instruction);
extern void andInstruction(bin_instr_t instruction);
extern void bor(bin_instr_t instruction);
extern void nor(bin_instr_t instruction);
extern void xorInstruction(bin_instr_t instruction);
extern void jr(bin_instr_t instruction);
extern void sll(bin_instr_t instruction);
extern void srl(bin_instr_t instruction);
extern void systemcallInstructionExecute(bin_instr_t instruction);
extern void immediateInstructionExecute(bin_instr_t instruction);
extern void jumpInstructionExecute(bin_instr_t instruction);
extern void systemCall(bin_instr_t instruction);
