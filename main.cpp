/**
 * Group 1
 * Alex Barczak, Flynn Henderson, Lucy Thomson, Emma Martin, Martyn Bett
*/

#include <iostream>
#include <fstream>
#include <time.h>
#include <cstdlib>

#include "simulator.h"
#include "assembler.h"
#include "AssemblyError.h"
#include "FileError.h"
#include "MachineCodeError.h"

using namespace std;

void convert_file_to_ints(ifstream& file, int machine_code[]){
    string line;
    for (int i = 0; i < 48; i++){
        file >> line;
        // if line length isnt 32 then not enough or too many bits so throw exception
        if (line.length() != 32){
            throw MachineCodeError("Machine code isn't the right length. Aborting.");
        }
        // string to long is used here instead of string to int because the function
        // assumes the input is unsigned, and would instead only assume a negative value when
        // there is a leading '-' we'll use string to long and then transfer the bits instead
        machine_code[i] = (int)stol(line, nullptr, 2);
    }
}

simulator* create_baby(string MC_filename){
    ifstream file;
    file.open(MC_filename);
    if(!file){
        throw FileError("Invalid File name. Failed to run baby.");
    }
    else{
        // 48 registers in store so 48 long arrat
        int machine_code[48];
        try{
            convert_file_to_ints(file, machine_code);
            file.close();
            return new simulator(machine_code);
        }
        // this will be catch machine code error and break;
        catch(MachineCodeError& mce){
        std::cerr << mce.what() << endl;
            file.close();
            return nullptr;
        }
    }
    file.close();
    return nullptr;
    }

void run_baby(simulator* m_baby)
{
    //Runs the created Manchester Baby sim
    //Follows the basic cycle of the Man Baby for a duration of time
    struct timespec remaining, request = {0, 5000000};

    while(true){
        nanosleep(&request, &remaining);
        m_baby->increment();
        m_baby->fetch();
        
        m_baby->print_store();
        cout << "accumulator: " << endl;
        m_baby->print_accumulator();
        cout << "control instruction: " << endl;
        m_baby->print_current_instruction();
        cout << endl <<"present instruction: " << endl;
        m_baby->print_present_instruction();
        
        m_baby->decode_and_execute();

        if (m_baby->get_is_stopped())
        {
            break;
        }
    }
}



/*
* Validates input from user for main menu
* @return int the validated integer value from the user
*/
int getValInput()
{
    string input ="";
    bool isInt = false;

    do
    {
        //Gets input, goes through the input checking each character is a digit and not a char
        cin >> input;

        int inputLength = input.length();

        for (int i = 0; i < inputLength; i++)
        {
            if (!isdigit(input[i]))
            {
                cout << "INVALID INPUT - CHAR DETECTED" << endl;
                break;
            }
            else
            {
                isInt = true;
            }
        }
    } while (!isInt);

    return atoi(input.c_str());
    
}

/*
* Prints menu options
*/
void menuOptions()
{
    cout << "Please select an option: " << endl;
    cout << "1. Simulate a program of your choice using machine code" << endl;
    cout << "2. Compile a program of your choice from assembly code" << endl;
    cout << "3. Exit the program" << endl;
}

int main(){
    // variables for menu function

    simulator* m_baby;
    string input;
    string output;
    int result = 0;
    int parseOut;

    while (true)
    {
        menuOptions(); //displays menu options

        result = getValInput();

        switch (result)
        {
            case 1:
                cout << "Option 1 chosen" << endl;
                cout << "Input name of machine code file you would like to run: ";
                cin >> input;
                // flush input stream after getting user input
                cin.ignore( 256, '\n' );
                try{
                    m_baby = create_baby(input);
                    if(m_baby!=nullptr){
                        run_baby(m_baby);
                    }
                }
                catch(FileError& fe){
                    cerr << fe.what() << endl;
                }
                break;
            case 2:
                cout << "Option 2 chosen" << endl;
                cout << "Input name of assembly language file you would like to turn into machine code: ";
                cin >> input;
                cin.ignore( 256, '\n' );
                cout << "Input name of file you would like to write machine code to (WARNING: any existing file with the same name will be overwitten): ";
                cin >> output;
                cin.ignore( 256, '\n' );
                try{
                    encode_from_assembly(input,output);
                }
                catch (AssemblyError& ae) {
                    cerr << ae.what() << endl;
                }
                catch(FileError& fe){
                    cerr << fe.what() << endl;
                }
                break;
            case 3:
                cout << "Option 3 chosen, au revoir!" << endl;
                exit(0);
                break;
            default:
                cout << "Invalid input. Try again" << endl;
                break;
        }
    }
}