#include <iostream>

#include "simulator.h"

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

}

void simulator::fetch(){

}

void simulator::decode(){

}

void simulator::execute(){

}