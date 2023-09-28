#ifndef _MACHINE_MAIN_H
#define _MACHINE_MAIN_H

#include "bof.h"
#include "instruction.h"
#include "machine_types.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE_IN_BYTES (65536 - BYTES_PER_WORD)
#define MEMORY_SIZE_IN_WORDS (MEMORY_SIZE_IN_BYTES / BYTES_PER_WORD)

BOFFILE file;
BOFHeader header;
word_type txtLen, txtStart, dataLen, dataStart, stackBot;

// union to represent the memory
static union mem_u {
  byte_type bytes[MEMORY_SIZE_IN_BYTES];
  word_type words[MEMORY_SIZE_IN_WORDS];
  bin_instr_t instrs[MEMORY_SIZE_IN_WORDS];
} Memory;

void printProgram(BOFFILE bf); // this function prints the program, when the -p command is used in the command line
void traceProgram(BOFFILE bf);

#endif