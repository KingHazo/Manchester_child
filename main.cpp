#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <cstdlib>

#include "simulator.h"

using namespace std;

void menuOptions();
int getValInput();

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

void run_baby(simulator* m_baby)
{

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
        //I would never normally be this pedantic in validation
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


int main(){
    // variables for menu function

    simulator* m_baby;
    string input;
    int result = 0;
    int parseOut;

    while (true)
    {
        menuOptions(); //displays menu options

        result = getValInput();

        switch (result)
        {
            case 1:
                cout << "Option 1 chosen" << endl; // Runs simulator using "marquee.txt"
                m_baby = create_baby("marquee.txt");
                run_baby(m_baby);
                break;
            case 2:
                cout << "Option 2 chosen" << endl;
                cout << "Unsure if this will work yet lol" << endl;
                //(I cannot for the life of me decipher some of the sim functions)
                cout << "Input a filename:";
                cin >> input;
                cout << endl;
                m_baby = create_baby(input);
                run_baby(m_baby);
                break;
            case 3:
                cout << "Option 3 chosen" << endl;
                cout << "To be implemented when assembler exists" << endl;
                break;
            case 4:
                cout << "Option 4 chosen" << endl;
                cout << "To be implemented when assembler exists" << endl;
                break;
            case 5:
                cout << "Option 5 chosen, au revoir!" << endl;
                exit(0);
                break;
            default:
                cout << "Invalid input. Try again" << endl;
                break;
        }
    }
}

/*
* Prints menu options
*
*/
void menuOptions()
{
    cout << "Please select an option: " << endl;
    cout << "1. Simulate a basic, starter program using machine code" << endl;
    cout << "2. Simulate a program of your choice using machine code" << endl;
    cout << "3. Compile a basic program from assembly code" << endl;
    cout << "4. Compile a program of your choice from assembly code" << endl;
    cout << "5. Exit the program" << endl;
}
