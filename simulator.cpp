/**
 * Group 1
 * Alex Barczak, Flynn Henderson, Lucy Thomson, Emma Martin, Martyn Bett
*/

#include <iostream>

#include "simulator.h"
#include "memory_line.h"

using namespace std;


//Base Constructor
simulator::simulator()
{
    for (int i = 0; i < 48; i++){
        this->store[i] = 0;
    }
}

simulator::simulator(int const initial_store[])
{
    for (int i = 0; i < 48; i++){
        this->store[i] = initial_store[i];
    }
}
//Debug function to print the store
void simulator::print_store(){
    for (int index = 0; index < 48; index++)
    {
        int line = this->store[index];
        for (int bit = 0; bit < 32; bit++)
        {
            cout << ((((line >> 31-bit) & 1) == 1) ? '1':'0') ;
        }
        cout << endl;
    }
    
}
//Debug function to print the accumulator
void simulator::print_accumulator(){
    int line = this->accumulator;
    for (int bit = 0; bit < 32; bit++)
    {
        cout << ((line >> 31-bit) & 1);
    }
    cout << " value " << flip_bits(this->accumulator);
    cout << endl;
}
//Debug function printing the present instruction
void simulator::print_present_instruction(){
    int line = this->present_instruction;
    for (int bit = 0; bit < 32; bit++)
    {
        cout << ((line >> 31-bit) & 1);
    }
    cout << endl;
}
//Debug function printing the current instruction
void simulator::print_current_instruction(){
    int line = this->control_instruction;
    for (int bit = 0; bit < 32; bit++)
    {
        cout << ((line >> 31-bit) & 1);
    }
    cout << endl;
}
//Function to increment the operand
void simulator::increment(){
    //Memory line function to convert from Man Baby format to our PC's format
    memory_line* control_line = new memory_line(this->control_instruction);
    //Increment operand by 1
    control_line->set_operand(control_line->get_operand() + 1);
    //Convert control instruction variable from memory line to store format
    this->control_instruction = control_line->memory_line_to_store_format();
    //Cleanup
    delete control_line;  
}

void simulator::fetch(){
    //Memory line function to convert from Man Baby format to our PC's format
    memory_line* control_line = new memory_line(this->control_instruction);
    //Setting present instruction to the desired instruction the control line points to
    this->present_instruction = this->store[control_line->get_operand()];
    //Cleanup
    delete control_line;
}

void simulator::decode_and_execute(){
    //Memory line function to convert from Man Baby format to our PC's format
    memory_line* present_instruction = new memory_line(this->present_instruction);
    //Find which opcode to be decoded and executed
    int opcode = present_instruction->get_opcode();
    cout << "opcode: " << opcode << endl;

    switch (opcode)
    {
    case 0:
    {
        //Debug info
        cout << "jumping to line: " << flip_bits(store[present_instruction->get_operand()]) << endl;
        // function 0: JMP, Copy content of the specified line into the CI
        this->control_instruction = store[present_instruction->get_operand()];
        
        break;
    }
    case 1:
    {
        // function 1: JRP, Add the content of the specified line into the CI
        //Convert both control instruction and memory value to memory line format
        memory_line* CI_line = new memory_line(this->control_instruction);
        memory_line* operand_value = new memory_line(this->store[present_instruction->get_operand()]);
        //Debug info
        cout << "incrementing CI by: " << flip_bits(store[present_instruction->get_operand()]) << endl;
        // if the CI is set to a value above the size of the store variable,
        // the program will have a seg fault and break
        //Set operand to current value + operand's line value
        CI_line->set_operand(CI_line->get_operand()+operand_value->get_value());
        
        this->control_instruction = CI_line->memory_line_to_store_format();
        //Cleanup
        delete CI_line;
        delete operand_value;
        break;
    }
    case 2:
    {
        // function 2: LDN, Copy the content of the specified line, negated, into the accumulator
        //Convert operand value to memory line format
        memory_line* operand_value = new memory_line(this->store[present_instruction->get_operand()]);
        //Set operand value to the negative of what it is
        operand_value->set_value(-(operand_value->get_value()));
        //Convert back into manchester baby format, and then into the accumulator
        this->accumulator = operand_value->memory_line_to_store_format();
        //Debug info
        cout << "loading value: " << flip_bits(operand_value->memory_line_to_store_format()) << " to accumulator"<< endl;
        //Cleanup
        delete operand_value;
        break;
    }
    case 3:
    {
        // function 3: STO, Copy the content of the accumulator to the specified store line
        //Taking the position store at the present instruction's operand pointer, setting the value to accumulator's
        this->store[present_instruction->get_operand()] = this->accumulator;
        //Debug info
        cout << "storing accumulator: " << flip_bits(this->accumulator) << " in line: " << present_instruction->get_operand() << endl;
        break;
    }
    case 4:
    {
        // function 4: SUB, Subtract the content of the specified line from the accumulator
        //Convert accumulator and operand value into memory line format
        memory_line* accumulator = new memory_line(this->accumulator);
        memory_line* operand_value = new memory_line(this->store[present_instruction->get_operand()]);
        //Debug info
        cout << "subtracting " << operand_value->get_value() << " from " << accumulator->get_value() << endl;
        //Subtracting the line pointed to from the actual value in the accumulator
        //Setting the accumulator value to that value
        accumulator->set_value(accumulator->get_value()-operand_value->get_value());
        //Converting it back to store format
        this->accumulator = accumulator->memory_line_to_store_format();
        //Cleanup
        delete accumulator;
        delete operand_value;
        break;
    }
    case 5:
    {
        // function 5: SUB, Exactly the same as for function number 4
        //Convert accumulator and operand value into memory line format
        memory_line* accumulator = new memory_line(this->accumulator);
        memory_line* operand_value = new memory_line(this->store[present_instruction->get_operand()]);
         //Debug info
        cout << "subtracting " << operand_value->get_value() << " from " << accumulator->get_value() << endl;
        //Subtracting the line pointed to from the actual value in the accumulator
        //Setting the accumulator value to that value
        accumulator->set_value(accumulator->get_value()-operand_value->get_value());
        //Converting it back to store format
        this->accumulator = accumulator->memory_line_to_store_format();
        //Cleanup
        delete accumulator;
        delete operand_value;
        break;
    }
    case 6:
    {
        // function 6: CMP, If the accumulator is less than 0 increment the CI
        //Convert accumulator to memory line format
        memory_line* accumulator = new memory_line(this->accumulator);
        cout << "if " << accumulator->get_value() << " < 0, increment counter. ";
        //If accumulator < 0, increment control instruction
        if (accumulator->get_value() < 0)
        {
            cout << "                  incrementing";
            this->increment();
        }
        cout << endl;
        //Cleanup
        delete accumulator;
        break;
    }
    case 7:
    {
        // function 7: STP, Halt the Baby and light the 'stop lamp'
        cout << "HALTED" << endl;
        this->isStopped = true;
        break;
    }
    case 8:
    {
        // Add contents of specified memory space to accumulator
        memory_line* accumulator = new memory_line(this->accumulator);
        memory_line* operand_value = new memory_line(this->store[present_instruction->get_operand()]);

        cout << "adding " << operand_value->get_value() << " to " << accumulator->get_value() << endl;

        accumulator->set_value(accumulator->get_value()+operand_value->get_value());
        this->accumulator = accumulator->memory_line_to_store_format();
        //Cleanup
        delete accumulator;
        delete operand_value;
        break;
    }
    case 9:
    {
        // indirect jump if positive
        memory_line* accumulator = new memory_line(this->accumulator);
        cout << "if " << accumulator->get_value() << " >= 0, jumping to line: " << flip_bits(store[present_instruction->get_operand()]) << endl;
        
        if (accumulator->get_value() >= 0)
        {
            this->control_instruction = store[present_instruction->get_operand()];
        }
        cout << endl;
        //Cleanup
        delete accumulator;
        break;
    }
    case 10:
    {
        // Just load instead of load negative
        memory_line* operand_value = new memory_line(this->store[present_instruction->get_operand()]);
        operand_value->set_value(operand_value->get_value());

        this->accumulator = operand_value->memory_line_to_store_format();
        cout << "loading value: " << flip_bits(operand_value->memory_line_to_store_format()) << " to accumulator"<< endl;
        //Cleanup
        delete operand_value;
        break;
    }
    case 11:
    {
        // absolute jump, just jumps to line specified in current instruction operand
        cout << "jumping to line: " << present_instruction->get_operand() << endl;
        this->control_instruction = flip_bits(present_instruction->get_operand());
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