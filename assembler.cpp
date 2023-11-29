/**
 * Group 1
 * Alex Barczak, Flynn Henderson, Lucy Thomson, Emma Martin, Martyn Bett
*/

#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>

#include "assembler.h"
#include "AssemblyError.h"
#include "FileError.h"

using namespace std;

/*
    compile code from assembly_filename and write to file output_filename
    throws exceptions in the case of something going wrong
*/
void encode_from_assembly(string assembly_filename, string output_filename){
    // open assembly_filename
    ifstream infile;
    infile.open(assembly_filename);
    if(!infile) {
        throw FileError("Invalid File name. Failed to create machine code.");
    }
    string line;
    // make instruction table mapping mnemonics to binary opcodes in 'non-reverse'
    map<string, string> instructionSet;
    instructionSet["JMP"] = "0000000000000000000";
    instructionSet["JRP"] = "0000000000000000001";
    instructionSet["LDN"] = "0000000000000000010";
    instructionSet["STO"] = "0000000000000000011";
    instructionSet["SUB"] = "0000000000000000100";
    instructionSet["SUB"] = "0000000000000000101";
    instructionSet["CMP"] = "00000000000000001100000000000000";
    instructionSet["STP"] = "00000000000000001110000000000000";
    instructionSet["ADD"] = "0000000000000001000";
    instructionSet["JPP"] = "0000000000000001001";
    instructionSet["LOD"] = "0000000000000001010";
    instructionSet["ABS"] = "0000000000000001011";

    // declare symbol table that will map labels to addresses
    map<string,string> symbolTable;

    string output[48];
    int outputIndex=0;

    // needs to go twice so it can initialise all labels and then fill them in
    for(int i=0;i<2;i++){
        outputIndex=0;
        while(getline(infile, line)) {
        cout << "Processing line: " << line << endl;
        // if entire line is a comment dont increment counter
        if (line[0] == ';') {
            continue;
        }
        // skip blank lines
        if(line.empty()){
            continue;
        }
        stringstream ss(line);
        string nextWord;
        while(ss >> nextWord){              
            // skip comments
            if(nextWord.compare(";")==0){
                break;
            }
            // check if last character is a colon, if yes then it's a new label
            char ch = nextWord.back();
            if(ch == ':'){
                cout << "Found label: " << nextWord << endl;
                // Take the colon off
                nextWord.pop_back();
                // Check for duplicate labels and throw exception if duplicate found
                if ((symbolTable.find(nextWord) != symbolTable.end()) && (i==0) && (symbolTable[nextWord]!="")){
                    cerr << "Error on line " << outputIndex+1 << endl;
                    throw AssemblyError("Duplicate label enountered");
                }
                // turn current output index into 13 bit binary
                string indexBinary = bitset<13>(outputIndex).to_string();
                // add label and address to symbol table
                symbolTable[nextWord] = indexBinary;
                cout << "Label " << nextWord << " added to symbol table with address " << indexBinary << endl;
                continue;
            }
            if (instructionSet.find(nextWord) != instructionSet.end()) {
                cout << "Found instruction: " << nextWord << endl;
                // found instruction, add appropriate binary to output buffer
                output[outputIndex] = instructionSet[nextWord];
                continue;
            } 
            char c = nextWord.front();
            // allow for immediate addressing
            if(c=='#'){
                nextWord.erase(nextWord.begin());
                int nextWordNum = stoi(nextWord);
                string binaryVariable = bitset<13>(nextWordNum).to_string();
                output[outputIndex] += binaryVariable;
                cout << "Added number: " << nextWord << " with binary code: " << output[outputIndex] << endl;
                continue;
            }
            if(nextWord.compare("VAR")==0){
                // get variable
                ss >> nextWord;
                // make sure there is a number after VAR keyword
                if(!isdigit(nextWord[0])){
                    cerr << "Error on line " << outputIndex+1 << endl;
                    throw AssemblyError("Uninitialised VAR");
                }
                // turn variable to 32 bit binary
                int nextWordNum = stoi(nextWord);
                string binaryVariable = bitset<32>(nextWordNum).to_string();
                // add to output buffer
                output[outputIndex] = binaryVariable;
                cout << "Added variable: " << nextWord << " with binary code: " << output[outputIndex] << endl;
                continue;
            }
            // only valid thing left to deal with is symbol
            if(symbolTable.find(nextWord)!=symbolTable.end()){
                cout << "Found symbol: " << nextWord << endl;
                // add address which corresponds to label to output buffer
                output[outputIndex] += symbolTable[nextWord];
                continue;
            }
            // adds next word to symbol table
            else if((symbolTable.find(nextWord)==symbolTable.end())){
                // new symbol so add it to symbol table and leave associated address value blank as we don't know it yet
                symbolTable[nextWord]="";
                cout << "Added Symbol: " << nextWord << " to symbol table" << endl;
                continue;
            }
            cerr << "Error on line " << outputIndex+1 << endl;
            throw AssemblyError("Invalid assembly language encountered");
        }
        outputIndex++;
    }
    // make sure all labels/symbols etc were initialised, if not after one pass through then invalid and throw exception as label cannot be used
    for (auto i = symbolTable.begin(); i != symbolTable.end(); i++) {
        if((i->second)==""){
            throw AssemblyError("Unintialised label or invalid instruction");
        } 
    }
    // return to start of file to repeat
    infile.clear();
    infile.seekg(0);
    }
    infile.close();
    // too many lines of assembly to use in 48 register manchester baby so throw exception
    if(outputIndex>47){
        throw AssemblyError("Too many lines in program. Failed to assemble.");
    }
    
    // make filewriter to output_filename
    ofstream writer(output_filename);
    if(!writer) {
        throw FileError("Error writing machine code to file.");
    }
    cout << "Writing machine code to " << output_filename << endl;
    for(int i = 0;i<outputIndex;i++){
        // print bits backwards for Manchester Baby format
        for(int j = 31;j>=0;j--){
            char bit = (output[i]).at(j);
            writer << bit;
        }
        cout << "Successfully printed line " << i+1 << " to file " << output_filename << endl;
        writer << endl;
    }
    writer.close();
    cout << "Assembly language in file " << assembly_filename << " successfully encoded. Machine code written to " << output_filename << endl;
}