#include <iostream>
#include <string>
#include <fstream>

#include "simulator.h"

using namespace std;

void convert_file_to_ints(ifstream& file, int machine_code[]){
    string line;
    size_t* position_of_first_char = new size_t;
    for (int i = 0; i < 32; i++){
        file >> line;
        if (line.length() > 32){
            throw new std::out_of_range("machine code string too large");
        }
        // string to long is used here instead of string to int because the function
        // assumes the input is unsigned, and would instead only assume a negative value when
        // there is a leading '-' we'll use string to long and then transfer the bits instead
        machine_code[i] = (int)stol(line, nullptr, 2);
    }
}

simulator* create_baby(string MC_filename){
    ifstream file;
    if (MC_filename == ""){
        return new simulator();
    } else {
        file.open(MC_filename);
        if (file){
            int machine_code[32];
            try{
                convert_file_to_ints(file, machine_code);
                file.close();
                return new simulator(machine_code);
            }
            catch(const std::exception& e){
                std::cerr << e.what() << '\n';
                cout << "initialising simulation with empty store" << endl;
                file.close();
                return new simulator();
            }
        }
        file.close();
        return new simulator();
    }
}

int main(){
    simulator* m_baby;
    string machine_code_filename = "BabyTest1-MC.txt";

    m_baby = create_baby(machine_code_filename);

    m_baby->print_store();
    
    return 0;
}
