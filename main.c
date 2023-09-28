#include "machine_main.h"

//this function prints the program, when the -p command is used in the command line
void printProgram(BOFFILE bf){
  //opens header and gets information needed from the header
  header = bof_read_header(file);
  txtLen = header.text_length;
  dataStart = header.data_start_address; 
  dataLen = header.data_length;

  //inputs all of the text section into memory
  for (int i = 0; i < txtLen / BYTES_PER_WORD; i++){
    Memory.instrs[i] = instruction_read(file);
  }

  //inputs all of the data section into memory
  for (int i = 0; i < dataLen / BYTES_PER_WORD; i++){
    Memory.words[dataStart + i] = bof_read_word(bf);
  }

  //begins the outputting
  printf("ADDR\tINSTR\n");

  //prints all of the instructions
  for (int i = 0; i < txtLen / BYTES_PER_WORD; i++){
    printf("\t%d\t%s \n", i * 4, instruction_assembly_form(Memory.instrs[i]));
  }  
  //prints all of the data section
  int i = -1;
  do{
    i++;
    printf("\t\t%d: %d ", dataStart + (i * BYTES_PER_WORD), Memory.words[dataStart + i]);
  } while (i < dataLen / BYTES_PER_WORD);
  printf("...\n");
}

void traceProgram(BOFFILE bf){
  header = bof_read_header(file);
  txtLen = header.text_length;
  txtStart = header.text_start_address;
  dataStart = header.data_start_address; 
  dataLen = header.data_length;
  stackBot = header.stack_bottom_addr;
  
  for (int i = 0; i < txtLen / BYTES_PER_WORD; i++){
    Memory.instrs[i] = instruction_read(file);
  }

  for (int i = 0; i < dataLen / BYTES_PER_WORD; i++){
    Memory.words[dataStart + i] = bof_read_word(bf);
  }
}

int main(int argc, char* argv[]){
    if (argc == 2){ //command line input has 2 (a.out and file name)
        file = bof_read_open(argv[1]);
      traceProgram(file);
    }
    else if (argc == 3){    //command line input has 3 (a.out, -p, and file name)
      if (strcmp(argv[1], "-p") != 0) bail_with_error("Command line argument, %s, not valid", argv[1]); //if the command line argument is not -p
      file = bof_read_open(argv[2]);  //open the file
      printProgram(file);  //call the print program function
    }
    else{ //bail with error, not the correct number of command line arguments
      bail_with_error("Either too many or not enough command line arguments\n");
    } 
    return 0;
}