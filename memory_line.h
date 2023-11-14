#ifndef MEMORY_LINE_H
#define MEMORY_LINE_H

class memory_line
{
private:
    int operand;
    int opcode;
    int unused_bits;
public:
    memory_line(int bits);
    
    int get_value();
    int get_operand();
    int get_opcode();
    int get_unused();
    
    void set_value(int value);
    void set_operand(int operand);
    void set_opcode(int opcode);
    void set_unused(int unused);
    
    int memory_line_to_store_format();
};

int flip_bits(int bits);

#endif