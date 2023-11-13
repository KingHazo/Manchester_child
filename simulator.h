#ifndef MANCHESTER_BABY_SIMULATOR
#define MANCHESTER_BABY_SIMULATOR

class simulator
{
private:
    int display[32];
    int store[32];
    int accumulator = 0;
    int control_instruction = 0;
    int present_instruction = 0;
    
public:
    simulator();
    simulator(int const initial_store[]);
    void print_store();
    void increment();
    void fetch();
    void decode();
    void execute();
};

#endif