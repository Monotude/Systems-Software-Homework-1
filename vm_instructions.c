#include "instruction.h"
#include "machine_types.h"
#include "utilities.h"
#include "vm_registers.h"
#include "vm_memory.h"

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
    switch (instruction.reg.func)
    {
      case 33:
        add(instruction);
        break;
      case 35:
        sub(instruction);
        break;
      case 25:
        mul(instruction); //function needs to be checked
        break;
      case 27:
        div(instruction);
        break;
      case 16:
        mfhi(instruction);
        break;
      case 18:
        mflo(instruction);
        break;
      case 36:
        andInstruction(instruction);
        break;
      case 37:
        bor(instruction);
        break;
      case 39:
        nor(instruction);
        break;
      case 38:
        xorInstruction(instruction);
        break;
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
      
          
    }
}

void systemcallInstructionExecute(bin_instr_t instruction) //looks at opcode
{
  switch(instruction.reg.op){
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
    case 4:
      beq(instruction);
      break;
    case 1:
      bgez(instruction);
      break;
    case 7:
      bgtz(instruction);
      break;
    case 6:
      blez(instruction);
      break;
    case 8:
      bltz(instruction);
      break;
    case 5:
      bne(instruction);
      break;
    case 36:
      lbu(instruction);
      break;
    case 35:
      lw(instruction);
      break;
    case 40:
      sb(instruction);
      break;
    case 43:
      sw(instruction);
      break;
    
    
  }
}

void immediateInstructionExecute(bin_instr_t instruction)
{
  
}

void jumpInstructionExecute(bin_instr_t instruction)
{
  
}

void addi(bin_instr_t instruction){
  //Add immediate: GPR[t] ← GPR[s] + sgnExt(i)
  GPR[instruction.immed.rt] = GPR[instruction.immed.rs] + machine_types_sgnExt(instruction.immed.immed);
}

void andi(bin_instr_t instruction){
  //Bitwise And immediate: GPR[t] ← GPR[s] ∧ zeroExt(i)
  GPR[instruction.immed.rt] = GPR[instruction.immed.rs] & machine_types_zeroExt(instruction.immed.immed);
}

void bori(bin_instr_t instruction){
  //Bitwise Or immediate: GPR[t] ← GPR[s] ∨ zeroExt(i)
  GPR[instruction.immed.rt] = GPR[instruction.immed.rs] | machine_types_zeroExt(instruction.immed.immed);
}

void xori(bin_instr_t instruction){
  //Bitwise Xor immediate: GPR[t] ← GPR[s] xor zeroExt(i)
  GPR[instruction.immed.rt] = GPR[instruction.immed.rs] ^ machine_types_zeroExt(instruction.immed.immed);
}

void beq(bin_instr_t instruction){
  //Branch on Equal: if GPR[s] = GPR[t] then PC ← PC + formOffset(o)
  if(GPR[instruction.immed.rs] == GPR[instruction.immed.rt])
    PC = PC + machine_types_formOffset(instruction.immed.immed);
}

void bgez(bin_instr_t instruction){
  //Branch ≥ 0: if GPR[s] ≥ 0 then PC ← PC + formOffset(o)
  if(GPR[instruction.immed.rs] >= 0)
    PC = PC + machine_types_formOffset(instruction.immed.immed);
}

void bgtz(bin_instr_t instruction){
  //Branch > 0: if GPR[s] > 0 then PC ← PC + formOffset(o)
  if(GPR[instruction.immed.rs] > 0)
    PC = PC + machine_types_formOffset(instruction.immed.immed);
}

void blez(bin_instr_t instruction){
  //Branch ≤ 0: if GPR[s] ≤ 0 then PC ← PC + formOffset(o)
  if(GPR[instruction.immed.rs] <= 0)
    PC = PC + machine_types_formOffset(instruction.immed.immed);
}

void bltz(bin_instr_t instruction){
  //Branch < 0: if GPR[s] < 0 then PC ← PC + formOffset(o)
  if(GPR[instruction.immed.rs] < 0)
    PC = PC + machine_types_formOffset(instruction.immed.immed);
}

void bne(bin_instr_t instruction){
  //Branch Not Equal: if GPR[s] ̸= GPR[t] then PC ← PC + formOffset(o)
  if(GPR[instruction.immed.rs] != GPR[instruction.immed.rt])
    PC = PC + machine_types_formOffset(instruction.immed.immed);
}

void lbu(bin_instr_t instruction){
  //Load Byte Unsigned: GPR[t] ← zeroExt(memory[GPR[b] + formOffset(o)])
  GPR[instruction.immed.rt] = machine_types_zeroExt(Memory.bytes[GPR[instruction.immed.rs] + machine_types_formOffset(instruction.immed.immed)]);
}

void lw(bin_instr_t instructions){
  //Load Word (4 bytes): GPR[t] ← memory[GPR[b] + formOffset(o)
  GPR[instructions.immed.rt] = Memory.words[GPR[instructions.immed.rs] + machine_types_formOffset(instructions.immed.immed)];
}

void sb(bin_instr_t instructions){
  //Store Byte (least significant byte of GPR[t]): memory[GPR[b] + formOffset(o)] ← GPR[t]
  Memory.bytes[GPR[instructions.immed.rs] + machine_types_formOffset(instructions.immed.immed)] = GPR[instructions.immed.rt];
}

void sw(bin_instr_t instructions){
  // Store Word (4 bytes): memory[GPR[b] + formOffset(o)] ← GPR[t]
  Memory.words[GPR[instructions.immed.rs] + machine_types_formOffset(instructions.immed.immed)] = GPR[instructions.immed.rt];
}

void mfhi(bin_instr_t instruction){
  //Move from HI: GPR[d] ← HI
  GPR[instruction.reg.rd] = HI;
}

void mflo(bin_instr_t instruction){
  //Move from LO: GPR[d] ← LO
  GPR[instruction.reg.rd] = LO;
}

void andInstruction(bin_instr_t instruction){
  //Bitwise And: GPR[d] ← GPR[s] ∧ GPR[t]
  GPR[instruction.reg.rd] = GPR[instruction.reg.rs] & GPR[instruction.reg.rt];
}

void bor(bin_instr_t instruction){
  //Bitwise Or: GPR[d] ← GPR[s] ∨ GPR[t]
  GPR[instruction.reg.rd] = GPR[instruction.reg.rs] | GPR[instruction.reg.rt];
}

void nor(bin_instr_t instruction){
  //Bitwise Not-Or: GPR[d] ← ¬(GPR[s] ∨ GPR[t])
  GPR[instruction.reg.rd] = ~(GPR[instruction.reg.rs] | GPR[instruction.reg.rt]);
}

void xorInstruction(bin_instr_t instruction){
  //Bitwise Exclusive-Or: GPR[d] ← GPR[s] xor GPR[t]
  GPR[instruction.reg.rd] = GPR[instruction.reg.rs] ^ GPR[instruction.reg.rt];
}

void mul(bin_instr_t instruction){ //Needs to be checked********************
  
  /*Multiply: Multiply GPR[s] and GPR[t],
  putting the least significant bits in LO
  and the most significant bits in HI.
  (HI, LO) ← GPR[s] × GPR[t]*/
  long product = GPR[instruction.reg.rs] * GPR[instruction.reg.rt];
  HI = (product & 0xFFFFFFFF00000000) >> 32;
  LO = product & 0x00000000FFFFFFFF;
}

void div(bin_instr_t instruction){
    /*Divide (remainder in HI, quotient in LO):
    HI ← GPR[s] % GPR[t]
    LO ← GPR[s]/GPR[t])*/
    HI = GPR[instruction.reg.rs] % GPR[instruction.reg.rt];
    LO = GPR[instruction.reg.rs]/GPR[instruction.reg.rt];
}

void sub(bin_instr_t instruction){
  //Subtract: GPR[d] ← GPR[s] − GPR[t]
  GPR[instruction.reg.rd] = GPR[instruction.reg.rs] - GPR[instruction.reg.rt];
}

void add(bin_instr_t instruction){
  //Add: GPR[d] ← GPR[s] + GPR[t]
  GPR[instruction.reg.rd] = GPR[instruction.reg.rs] + GPR[instruction.reg.rt];
}

void jr(bin_instr_t instruction){
  //Jump Register: PC ← GPR[s]
  PC = GPR[instruction.reg.rs];
}

void sll(bin_instr_t instruction)
{
    GPR[instruction.reg.rd] = GPR[instruction.reg.rt] << instruction.reg.shift;
}

void srl(bin_instr_t instruction)
{
    GPR[instruction.reg.rd] = GPR[instruction.reg.rt] >> instruction.reg.shift;
}
