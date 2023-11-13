#include "memory_line.h"

// adapted from an answer in https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
int flip_bits(int bits){
    int bitcount = 32;
    int reversed_bits = 0;

    for (int i = 0; i < bitcount; ++i, bits >>= 1) {
        reversed_bits = (reversed_bits << 1) | (bits & 0x01);
    }

    return reversed_bits;
}

memory_line::memory_line(int bits){
    this->operand = flip_bits(bits & 0xFFF80000);
    this->opcode = flip_bits((bits & 0x00070000) << 13);
    this->unused_bits = flip_bits((bits & 0x0000FFFF) << 16);
}

int memory_line::memory_line_to_store_format(){
    int bits = 0;
    bits |= flip_bits(this->operand);
    bits |= flip_bits(this->opcode >> 13);
    bits |= flip_bits(this->operand >> 16);
    return bits;
}

int memory_line::get_operand(){
    return this->operand;
}

int memory_line::get_opcode(){
    return this->opcode;
}

int memory_line::get_unused(){
    return this->unused_bits;
}