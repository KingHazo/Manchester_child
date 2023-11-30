/**
 * Group 1
 * Alex Barczak, Flynn Henderson, Lucy Thomson, Emma Martin, Martyn Bett
*/

#ifndef MACHINE_CODE_ERROR_H
#define MACHINE_CODE_ERROR_H

#include <exception>
#include <string>

class MachineCodeError : public std::exception {
    private:
    const char * message;

    public:
    MachineCodeError(const char * msg) : message(msg) {}
    const char * what () {
        return message;
    }
};

#endif