#include "instruction.h"
#include "machine_types.h"
#include "utilities.h"
#include "vm_registers.h"

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
        //systemCall(instruction);
        break;
      
          
    }
}

void systemCall(bin_instr_t instruction){ //needs to be finished
  
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
  int product = GPR[instruction.reg.rs] * GPR[instruction.reg.rt];

  HI = product % GPR[instruction.reg.rt];
  LO = product/GPR[instruction.reg.rt];
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

void systemcallInstructionExecute(bin_instr_t instruction)
{
}

void immediateInstructionExecute(bin_instr_t instruction)
{
}

void jumpInstructionExecute(bin_instr_t instruction)
{
}
