#include "instruction.h"
#include "machine_types.h"
#include "utilities.h"

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

void registerInstructionExecute(bin_instr_t instruction)
{
}

void systemcallInstructionExecute(bin_instr_t instruction)
{
}

void immediateInstructionExecute(bin_instr_t instruction)
{
}

void jumpInstructionExecute(bin_instr_t instruction)
{
}