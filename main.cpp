#include <iostream>
#include <string>
#include "Chip8.h"

Chip8 chip8;

int main() {
    std::cout << "###########################################################\n"
                 "#                                                         #\n"
                 "#    #######   ###     ###   #        #    ##########     #\n"
                 "#    #         #  #  #   #   #        #   #          #    #\n"
                 "#    #         #   ##    #   #        #   #          #    #\n"
                 "#    #         #         #   #        #   #          #    #\n"
                 "#    #         #         #   #        #    ##########     #\n"
                 "#    #######   #         #   #        #   #          #    #\n"
                 "#    #         #         #   #        #   #          #    #\n"
                 "#    #         #         #   #        #   #          #    #\n"
                 "#    #         #         #    #      #    #          #    #\n"
                 "#    #######   #         #     ######      ##########     #\n"
                 "#                                                         #\n"
                 "########################################################### By MaximumOverflow\n";

    std::string ROM_Path;
    std::cout << "Please enter ROM path: ";
    std::getline(std::cin, ROM_Path);
    chip8.LoadROM_Test(ROM_Path);
    chip8.run();

    std::cin.get();
    return 0;
}