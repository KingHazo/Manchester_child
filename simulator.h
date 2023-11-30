/**
 * Group 1
 * Alex Barczak, Flynn Henderson, Lucy Thomson, Emma Martin, Martyn Bett
*/

#ifndef MANCHESTER_BABY_SIMULATOR
#define MANCHESTER_BABY_SIMULATOR

class simulator
{
private:
    int display[48];
    int store[48];
    int accumulator = 0;
    int control_instruction = 0;
    int present_instruction = 0;
    bool isStopped = false;
    
public:
    simulator();
    simulator(int const initial_store[]);
    void print_store();
    void print_accumulator();
    void print_present_instruction();
    void print_current_instruction();
    void increment();
    void fetch();
    void decode_and_execute();
    bool get_is_stopped();
};

#endif