#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <stdint.h>
#include <cstdlib>
#include <list>
#include <limits>

using namespace std;

string decimalToBinary(int value) {
    string binaryValue[32];
  
    // https://www.geeksforgeeks.org/program-decimal-binary-conversion/
    // counter for binary array 
    int i = 0;
    while (value && i < 32) { 
        if (value & 1) // 1 
            binaryValue[i] = "1"; 
        else // 0 
            binaryValue[i] = "0"; 
        
        value >>= 1; // Right Shift by 1   
        i++;
    }

    string str;
    for (int i = 0; i < 32; i++) {
        str += binaryValue[i];
    }

    return str;
}

string operand13Long(string num) {
    stringstream intValue(num);
    int number = 0;
    intValue >> number;
    string str = decimalToBinary(number);
    int i = str.length();
    while (i < 13) {
        str += "0";
        i++;
    }

    return str;
}

fstream& GotoLine(fstream& file, unsigned int num){
    file.seekg(ios::beg); 
    for(int i=0; i < num - 1; ++i){
        file.ignore(numeric_limits<streamsize>::max(),'\n'); 
    }
    return file;
} 

list<int> findLocations(string input_filename, list<string> linesToChange, list<string> linesToChangeValues) {
    list<int> locations;
    
    ifstream inputFile;
    inputFile.open(input_filename);

    int lineNum = 0;
    while (linesToChangeValues.size() != 0) {
        if (inputFile.eof()) {
            inputFile.seekg(ios::beg);
            lineNum = 0;
        }
        char line[256];
        inputFile.getline(line, 256);
        char value[256];

        char *pLine = (char*)malloc(sizeof(char));
        pLine = strtok(line, " ");
        char *pValue = (char*)malloc(sizeof(char));
        pValue = new char[linesToChangeValues.back().length()];
        strcpy(pValue, (linesToChangeValues.back()+':').c_str());

        while (pLine != NULL) {
            if (strcmp(pLine, ";") == 0) break;
            else if (strcmp(pLine, "\n") == 0) break;

            if (strcmp(pLine, pValue) == 0) {
                locations.push_front(lineNum);
                linesToChangeValues.pop_back();
            }

            pLine = strtok(NULL, " "); 
        }
        free(pLine);
        free(pValue);
        lineNum++;
    }

    inputFile.close(); 

    return locations;
} 

/*
    compile code from assembly_filename and write to file output_filename
    throws exceptions in the case of something going wrong
*/
void encode_from_assembly(string assembly_filename, string output_filename){ 
    // TODO: implement
    ifstream inputFile;
    fstream outputFile;
    inputFile.open(assembly_filename);
    outputFile.open(output_filename);

    list<string> linesToChange;
    list<string> linesToChangeValues;

    int lineNum = 0;
    while (!inputFile.eof()) { // for each line 
        char line[256]; // current line
        inputFile.getline(line, 256);
        

        string previousTok;
        string instruction;
        string operand;

        // splits each line into string tokens 
        char *pLine = (char*)malloc(sizeof(char));
        pLine = strtok(line, " ");
        while (pLine != NULL) {
            // skips/breaks to next line if its a comment next
            if (strcmp(pLine, ";") == 0) break;
            else if (strcmp(pLine, "\n") == 0) break;
            else if (strcmp(pLine, "VAR") == 0) instruction = "VAR";
            else if (strcmp(pLine, "JMP") == 0) instruction = "JMP";    
            else if (strcmp(pLine, "JRP") == 0) instruction = "JRP";
            else if (strcmp(pLine, "LDN") == 0) instruction = "LDN";
            else if (strcmp(pLine, "STO") == 0) instruction = "STO";
            else if (strcmp(pLine, "SUB") == 0) instruction = "SUB";
            else if (strcmp(pLine, "CMP") == 0) instruction = "CMP";
            else if (strcmp(pLine, "STP") == 0) instruction = "STP";

            
            if (previousTok == instruction) operand = pLine;

            previousTok = pLine;
            cout << pLine << endl;
            pLine = strtok(NULL, " ");
        }
        free(pLine);

        if (instruction == "VAR") {
            string str = operand13Long(operand);
            outputFile << str << "0000000000000000000" << endl;
        }
        else if (instruction == "JMP" && operand != "") {
            string str = operand13Long(operand);
            outputFile << str << endl;
        }
        else if (instruction == "JRP" && operand != "") {
            stringstream intValue(operand);
            int num = 0;
            intValue >> num;
            string str = decimalToBinary(num);
            int i = str.length();
            while (i < 14) {
                str += "0";
                i++;
            } 
            str += "100";
            str += "0000000000000000";
            outputFile << str << endl;
        }
        else if (instruction == "LDN" && operand != "") { 
            string string = "00000000000000100000000000000000";
            linesToChange.push_front(to_string(lineNum));
            linesToChangeValues.push_front(operand);
            outputFile << string << endl; 
        }
        else if (instruction == "STO" && operand != "") {
            string string = "00000000000001100000000000000000"; // lets see if it crashes
            linesToChange.push_front(to_string(lineNum));
            linesToChangeValues.push_front(operand);
            outputFile << string << endl;
        }
        else if (instruction == "SUB") {
            string str = "00000000000000010000000000000000";
            linesToChange.push_front(to_string(lineNum));
            linesToChangeValues.push_front(operand);
            outputFile << str << endl;
        }
        else if (instruction == "CMP") {
            string str = "00000000000000110000000000000000";
            outputFile << str << endl;
        }
        else if (instruction == "STP") {
            string str = "00000000000001110000000000000000";
            outputFile << str << endl;
        }
        lineNum++;
        operand = "";
        instruction = "";
    } 

    list<int> locations = findLocations(assembly_filename, linesToChange, linesToChangeValues);

    while (!linesToChange.empty()) {
        stringstream intValue(linesToChange.front());
        int lineInt = 0;
        intValue >> lineInt;

        GotoLine(outputFile, lineInt+1);
        string currentLine;
        outputFile >> currentLine;
        currentLine.erase(currentLine.begin(), currentLine.begin()+13);

        string str = operand13Long(to_string(locations.front()));

        outputFile.seekp(ios::beg);
        for(int i=0; i < lineInt - 1; ++i){ 
            outputFile.ignore(numeric_limits<streamsize>::max(),'\n');
        }
        long pos = outputFile.tellp();
        outputFile.seekp(pos+33);
        outputFile << str << currentLine;

        linesToChange.pop_front();
        locations.pop_front();
    } // 
    outputFile.close();
    inputFile.close(); 
} 

bool compare_files(string file1Str, string file2Str) {
    fstream File1;
    fstream File2;
    File1.open(file1Str);
    File2.open(file2Str);

    string file1Line;
    string file2Line;
    int lineNum = 0;
    while (!File1.eof()) {
        getline(File1, file1Line, '\n');
        getline(File2, file2Line, '\n');
        file2Line.erase(remove(file2Line.begin(), file2Line.end(), '\r'));

        if (file1Line != file2Line) {
            cout << "Files not equivelant at line: " << lineNum << endl;
            return false;
        }

        lineNum++;
    }

    File2.close();
    File1.close();

    return true;
}

/* 
    main in assembler.cpp is only to be used for the purpose of testing on the branch, 
    it should only be used to call the assembling function or perform tests until 
    further discussion is held
*/ 
int main(){
    string input_filename = "BabyTest1-Assembler.txt";
    string output_filename = "output-test.txt";
    string comparative_machine_code = "BabyTest1-MC.txt";
    bool assembled_correctly;

    // cout << "what is the name of the test input file: " << endl;
    // cin >> input_filename;
    //cout << "what is the desired name of the output file? " << endl;
    //cin >> output_filename;

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
        cout << "File " << input_filename << " was compiled correctly, compiled code matches " << comparative_machine_code << endl;
        return 0;
    }
    cout << "File " << input_filename << "was not compiled correctly, compiled code does not match " << comparative_machine_code << endl;
    return 0;
}