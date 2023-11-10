#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*
    compile code from assembly_filename and write to file output_filename
    throws exceptions in the case of something going wrong
*/
void encode_from_assembly(string assembly_filename, string output_filename){
    // TODO: implement
}

/*
    compare file1 and file2 line by line, to compare and see if they are equivalent
*/
bool compare_files(string file1, string file2){
    // TODO: implement
}

/* 
    main in assembler.cpp is only to be used for the purpose of testing on the branch, 
    it should only be used to call the assembling function or perform tests until 
    further discussion is held
*/ 
int main(){
    string input_filename = "BabyTest1-Assembler.txt";
    string output_filename;
    string comparative_machine_code = "BabyTest1-MC.txt";
    bool assembled_correctly;

    // cout << "what is the name of the test input file: " << endl;
    // cin >> input_filename;
    cout << "what is the desired name of the output file? " << endl;
    cin >> output_filename;

    // use exception throwing, not error code returns to find faults
    try{
        encode_from_assembly(input_filename, output_filename);
        assembled_correctly = compare_files(output_filename, comparative_machine_code);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return 1;
    }
    
    if (assembled_correctly)
    {
        cout << "File " << input_filename << "was compiled correctly, compiled code matches " << comparative_machine_code << endl;
        return 0;
    }
    cout << "File " << input_filename << "was not compiled correctly, compiled code does not match " << comparative_machine_code << endl;
    return 0;
}