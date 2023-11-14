#include <iostream>
#include <string>
#include <fstream>
#include <time.h>

#include "simulator.h"

using namespace std;

void convert_file_to_ints(ifstream& file, int machine_code[]){
    string line;
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
    string machine_code_filename = "marquee.txt";

    m_baby = create_baby(machine_code_filename);

    struct timespec remaining, request = {0, 5000000};

    while(true){
        nanosleep(&request, &remaining);
        m_baby->increment();
        m_baby->fetch();
        
        m_baby->print_store();
        cout << "accumulator: " << endl;
        m_baby->print_accumulator();
        cout << "current instruction: " << endl;
        m_baby->print_current_instruction();
        cout << endl <<"present instruction: " << endl;
        m_baby->print_present_instruction();
        
        m_baby->decode_and_execute();

        if (m_baby->get_is_stopped())
        {
            break;
        }
    }

    return 0;
}
