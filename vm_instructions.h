#include "instruction.h"
#include "machine_types.h"

extern void executeInstruction(bin_instr_t instruction);
extern void registerInstructionExecute(bin_instr_t instruction);
extern void sll(bin_instr_t instruction);
extern void srl(bin_instr_t instruction);
extern void systemcallInstructionExecute(bin_instr_t instruction);
extern void immediateInstructionExecute(bin_instr_t instruction);
extern void jumpInstructionExecute(bin_instr_t instruction);