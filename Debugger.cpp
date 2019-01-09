//
// Created by max on 09/01/19.
//

#include <iostream>
#include "Debugger.h"

void Debugger::printInfo(std::string info) {
    std::cout << "[Debug Info] "<<info<<'\n';
}

void Debugger::printError(std::string component, std::string error) {
    std::cout << "[" << component << " Error] " << error << '\n';
}
