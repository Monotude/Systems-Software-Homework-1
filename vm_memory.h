#ifndef _MEMORY_H
#define _MEMORY_H

#include "bof.h"
#include "instruction.h"
#include "machine_types.h"

#define MEMORY_SIZE_IN_BYTES (65536 - BYTES_PER_WORD)
#define MEMORY_SIZE_IN_WORDS (MEMORY_SIZE_IN_BYTES / BYTES_PER_WORD)

// union to represent the memory
union mem_u
{
  byte_type bytes[MEMORY_SIZE_IN_BYTES];
  word_type words[MEMORY_SIZE_IN_WORDS];
  bin_instr_t instrs[MEMORY_SIZE_IN_WORDS];
};

extern union mem_u Memory;

#endif