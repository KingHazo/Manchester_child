/**
 * Group 1
 * Alex Barczak, Flynn Henderson, Lucy Thomson, Emma Martin, Martyn Bett
*/

#ifndef FILE_ERROR_H
#define FILE_ERROR_H

#include <exception>
#include <string>

class FileError : public std::exception {
    private:
    const char * message;

    public:
    FileError(const char * msg) : message(msg) {}
    const char * what () {
        return message;
    }
};

#endif