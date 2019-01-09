//
// Created by max on 09/01/19.
//

#ifndef EMU8_DEBUGGER_H
#define EMU8_DEBUGGER_H


#include <string>

class Debugger {
public:
    static void printInfo(std::string info);
    static void printError(std::string component, std::string error);
};


#endif //EMU8_DEBUGGER_H
