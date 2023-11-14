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
            cout << ((((line >> 31-bit) & 1) == 1) ? '#':' ') ;
        }
        cout << endl;
    }
    
}

void simulator::print_accumulator(){
    int line = this->accumulator;
    for (int bit = 0; bit < 32; bit++)
    {
        cout << ((line >> 31-bit) & 1);
    }
    cout << " value " << flip_bits(this->accumulator);
    cout << endl;
}

void simulator::print_present_instruction(){
    int line = this->present_instruction;
    for (int bit = 0; bit < 32; bit++)
    {
        cout << ((line >> 31-bit) & 1);
    }
    cout << endl;
}

void simulator::print_current_instruction(){
    int line = this->control_instruction;
    for (int bit = 0; bit < 32; bit++)
    {
        cout << ((line >> 31-bit) & 1);
    }
    cout << endl;
}

void simulator::increment(){
    memory_line* control_line = new memory_line(this->control_instruction);

    control_line->set_operand(control_line->get_operand() + 1);
    this->control_instruction = control_line->memory_line_to_store_format();

    delete control_line;  
}

void simulator::fetch(){
    memory_line* control_line = new memory_line(this->control_instruction);

    this->present_instruction = this->store[control_line->get_operand()];

    delete control_line;
}

void simulator::decode_and_execute(){
    memory_line* present_instruction = new memory_line(this->present_instruction);
    int opcode = present_instruction->get_opcode();
    cout << "opcode: " << opcode << endl;

    switch (opcode)
    {
    case 0:
    {
        cout << "jumping to line: " << flip_bits(store[present_instruction->get_operand()]) << endl;
        // TESTED AND SEEMS FINE
        // function 0: JMP, Copy content of the specified line into the CI
        this->control_instruction = store[present_instruction->get_operand()];
        
        break;
    }
    case 1:
    {
        // NOT TESTED YET
        // function 1: JRP, Add the content of the specified line into the CI
        memory_line* CI_line = new memory_line(this->control_instruction);
        memory_line* operand_value = new memory_line(this->store[present_instruction->get_operand()]);
        
        // this can potentially go horribly wrong, but I don't remember 
        // one of the requirements being prevent the simulation from breaking
        // if the CI is set to a value above the size of the store variable,
        // the program will have a seg fault and break
        CI_line->set_operand(CI_line->get_operand()+operand_value->get_value());

        this->control_instruction = CI_line->memory_line_to_store_format();
        
        delete CI_line;
        delete operand_value;
        break;
    }
    case 2:
    {
        // TESTED AND SEEMS FINE
        // function 2: LDN, Copy the content of the specified line, negated, into the accumulator
        memory_line* operand_value = new memory_line(this->store[present_instruction->get_operand()]);
        operand_value->set_value(-(operand_value->get_value()));

        this->accumulator = operand_value->memory_line_to_store_format();
        cout << "loading value: " << flip_bits(operand_value->memory_line_to_store_format()) << " to accumulator"<< endl;

        delete operand_value;
        break;
    }
    case 3:
    {
        // TESTED AND SEEMS FINE
        // function 3: STO, Copy the content of the accumulator to the specified store line
        this->store[present_instruction->get_operand()] = this->accumulator;
        cout << "storing accumulator: " << flip_bits(this->accumulator) << " in line: " << present_instruction->get_operand() << endl;
        break;
    }
    case 4:
    {
        // TESTED AND SEEMS FINE
        // function 4: SUB, Subtract the content of the specified line from the accumulator
        memory_line* accumulator = new memory_line(this->accumulator);
        memory_line* operand_value = new memory_line(this->store[present_instruction->get_operand()]);
        
        cout << "subtracting " << operand_value->get_value() << " from " << accumulator->get_value() << endl;
        
        accumulator->set_value(accumulator->get_value()-operand_value->get_value());
        this->accumulator = accumulator->memory_line_to_store_format();
        
        delete accumulator;
        delete operand_value;
        break;
    }
    case 5:
    {
        // TESTED AND SEEMS FINE
        // function 5: SUB, Exactly the same as for function number 4
        memory_line* accumulator = new memory_line(this->accumulator);
        memory_line* operand_value = new memory_line(this->store[present_instruction->get_operand()]);

        cout << "subtracting " << operand_value->get_value() << " from " << accumulator->get_value() << endl;

        accumulator->set_value(accumulator->get_value()-operand_value->get_value());
        this->accumulator = accumulator->memory_line_to_store_format();
        
        delete accumulator;
        delete operand_value;
        break;
    }
    case 6:
    {
        // TESTED AND SEEMS FINE
        // function 6: CMP, If the accumulator is less than 0 increment the CI
        memory_line* accumulator = new memory_line(this->accumulator);
        cout << "if " << accumulator->get_value() << " < 0, increment counter. ";
        
        if (accumulator->get_value() < 0)
        {
            cout << "                  incrementing";
            this->increment();
        }
        cout << endl;
        delete accumulator;
        break;
    }
    case 7:
    {
        // TESTED AND SEEMS FINE
        // function 7: STP, Halt the Baby and light the 'stop lamp'
        this->isStopped = true;
        break;
    }
    default:
        break;
    }

    delete present_instruction;
}

bool simulator::get_is_stopped(){
    return this->isStopped;
}