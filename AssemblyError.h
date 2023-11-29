/**
 * Group 1
 * Alex Barczak, Flynn Henderson, Lucy Thomson, Emma Martin, Martyn Bett
*/

#ifndef ASSEMBLY_ERROR_H
#define ASSEMBLY_ERROR_H

#include <exception>
#include <string>

class AssemblyError : public std::exception {
    private:
    const char * message;

    public:
    AssemblyError(const char * msg) : message(msg) {}
    const char * what () {
        return message;
    }
};

#endif