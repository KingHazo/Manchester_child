#include <iostream>

#include "simulator.h"
#include "memory_line.h"

using namespace std;

simulator::simulator()
{
    for (int i = 0; i < 32; i++){
        this->store[i] = 0;
    }
}

simulator::simulator(int const initial_store[])
{
    for (int i = 0; i < 32; i++){
        this->store[i] = initial_store[i];
    }
}

void simulator::print_store(){
    for (int index = 0; index < 32; index++)
    {
        int line = this->store[index];
        for (int bit = 0; bit < 32; bit++)
        {
            cout << ((line >> 31-bit) & 1);
        }
        cout << endl;
    }
    
}

void simulator::increment(){
    memory_line* control_line = new memory_line(this->control_instruction);

    control_line->set_opcode(control_line->get_opcode() + 1);

    this->control_instruction = control_line->memory_line_to_store_format();

    delete control_line;
}

void simulator::fetch(){
    memory_line* control_line = new memory_line(this->control_instruction);
    memory_line* present_instruction = new memory_line(this->store[control_line->get_operand()]);

    this->present_instruction = present_instruction->memory_line_to_store_format();

    delete control_line;
    delete present_instruction;
}

void simulator::decode(){
    memory_line* present_instruction = new memory_line(this->present_instruction);
    int opcode = present_instruction->get_opcode();

    switch (opcode)
    {
    case 0:
        // function 0: JMP, Copy content of the specified line into the CI
        this->control_instruction = store[present_instruction->get_operand()];
        break;
    case 1:
        // function 1: JRP, Add the content of the specified line into the CI
        break;
    case 2:
        // function 2: LDN, Copy the content of the specifies line, negated, into the accumulator
        break;
    case 3:
        // function 3: STO, Copy the content of the accumulator to the specified store line
        break;
    case 4:
        // function 4: SUB, Subtract the content of the specified line from the accumulator
        break;
    case 5:
        // function 5: SUB, Exactly the same as for function number 4
        break;
    case 6:
        // function 6: CMP, If the accumulator is less than 0 increment the CI
        break;
    case 7:
        // fucntion 7: STP, Halt the Baby and light the 'stop lamp'
        break;
    
    default:
        break;
    }

    delete present_instruction;
}

void simulator::execute(){

}