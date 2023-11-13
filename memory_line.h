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
    int get_operand();
    int get_opcode();
    int get_unused();
    int memory_line_to_store_format();
};


#endif