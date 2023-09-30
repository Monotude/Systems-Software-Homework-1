#include "bof.h"
#include "instruction.h"
#include "machine_types.h"
#include "main.h"
#include "regname.h"
#include "utilities.h"
#include "vm_instructions.h"
#include "vm_memory.h"
#include "vm_registers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printProgram(BOFFILE bf) // this function prints the program, when the -p command is used in the command line
{
    loadInfo(bf); // load all of the file info into memory and registers

    printf("Addr Instruction\n");                                 // begins the outputting
    for (int i = 0; i < header.text_length / BYTES_PER_WORD; ++i) // prints all of the instructions
    {
        printf("%4d %s\n", i * 4, instruction_assembly_form(Memory.instrs[i]));
    }
    for (int i = 0; i <= header.data_length / BYTES_PER_WORD; ++i) // prints all of the data section
    {
        printf("%8d: %d\t", header.data_start_address + (i * BYTES_PER_WORD), Memory.words[header.data_start_address + i]);

        if (Memory.words[header.data_start_address + i] == 0)
        {
            break;
        }

        if (i % 4 == 0 && i != 0)
        {
            printf("\t\n");
        }
    }
    printf("...\n");
}

void traceProgram(BOFFILE bf)
{
    loadInfo(bf);         // load all of the file info into memory and registers
    rewind(file.fileptr); // start the file pointer over so you can read the instructions

    for (int i = 0; i < header.text_length; ++i)
    {
        enforceInvariants();
        printTraceProgram();
        IR = PC;
        PC += 4;
        executeInstruction(instruction_read(file));
    }
}

void loadInfo(BOFFILE bf)
{
    header = bof_read_header(file); // opens header and gets information needed from the header

    for (int i = 0; i < header.text_length / BYTES_PER_WORD; ++i) // inputs all of the text section into memory
    {
        Memory.instrs[i] = instruction_read(file);
    }

    for (int i = 0; i < header.data_length / BYTES_PER_WORD; ++i) // inputs all of the data section into memory
    {
        Memory.words[header.data_start_address + i] = bof_read_word(bf);
    }

    HI = LO = 0; // initialise hi and lo to 0

    for (int i = 0; i < 32; ++i) // initialise GPR to 0
    {
        GPR[i] = 0;
    }

    PC = header.text_start_address; // initialise program counter

    GPR[GP] = header.data_start_address; // initialise global pointer

    GPR[SP] = GPR[FP] = header.stack_bottom_addr; // initialise stack and frame pointer
}

void enforceInvariants() // error checking
{
    if (PC % BYTES_PER_WORD != 0)
    {
        bail_with_error("PC is not divisible by %d", BYTES_PER_WORD);
    }

    if (GPR[GP] % BYTES_PER_WORD != 0)
    {
        bail_with_error("GP is not divisible by %d", BYTES_PER_WORD);
    }

    if (GPR[SP] % BYTES_PER_WORD != 0)
    {
        bail_with_error("SP is not divisible by %d", BYTES_PER_WORD);
    }

    if (GPR[FP] % BYTES_PER_WORD != 0)
    {
        bail_with_error("FP is not divisible by %d", BYTES_PER_WORD);
    }

    if (GPR[GP] < 0)
    {
        bail_with_error("GP is less than 0");
    }

    if (GPR[GP] >= GPR[SP])
    {
        bail_with_error("GP is greater than or equal to SP");
    }

    if (GPR[SP] > GPR[FP])
    {
        bail_with_error("SP is greater than FP");
    }

    if (GPR[FP] >= MEMORY_SIZE_IN_BYTES)
    {
    }

    if (PC < 0)
    {
        bail_with_error("PC is less than 0");
    }

    if (PC >= MEMORY_SIZE_IN_BYTES)
    {
        bail_with_error("PC is too big for memory");
    }

    if (GPR[0] != 0)
    {
        bail_with_error("0 register is not 0");
    }
}

void printTraceProgram() // print the tracing
{
}

int main(int argc, char *argv[])
{
    if (argc == 2) // command line input has 2 (a.out and file name)
    {
        file = bof_read_open(argv[1]);
        traceProgram(file);
    }

    else if (argc == 3) // command line input has 3 (a.out, -p, and file name)
    {
        if (strcmp(argv[1], "-p") != 0) // if the command line argument is not -p
        {
            bail_with_error("Command line argument, %s, not valid", argv[1]);
        }
        file = bof_read_open(argv[2]); // open the file
        printProgram(file);            // call the print program function
    }

    else // bail with error, not the correct number of command line arguments
    {
        bail_with_error("Either too many or not enough command line arguments\n");
    }

    return 0;
}