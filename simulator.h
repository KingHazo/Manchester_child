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
    ~simulator();
};

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

simulator::~simulator()
{
}


#endif