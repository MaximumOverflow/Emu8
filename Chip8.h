//
// Created by max on 07/01/19.
//

#ifndef EMU8_CHIP8_H
#define EMU8_CHIP8_H

#include "Debugger.h"

#include <tiff.h>
#include <string>

struct Stack {
    int16 memory[16];
    void operator<<(int16 data) {
        for (int i = 15; i > 0; --i)
            memory[i] = memory[i-1];
        memory[0] = data;
    }
};

class Chip8 {
    //Debug
    uint32_t cycles;
    void dump_OP_Code();
    void dump_Unimplemented_OP_Code();

    //Architecture
    uint8_t RAM [4096];
    uint8_t V[16];
    uint16_t I;
    Stack stack;
    uint8_t DT, ST;
    uint16_t PC;
    uint8_t SP;

    bool draw;
    uint16_t OP_Code;


    //Keyboard
    bool key[16];

    //Graphics
    bool pixels[64][32];
    unsigned char fonts[80] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, //0
            0x20, 0x60, 0x20, 0x20, 0x70, //1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
            0x90, 0x90, 0xF0, 0x10, 0x10, //4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
            0xF0, 0x10, 0x20, 0x40, 0x40, //7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
            0xF0, 0x90, 0xF0, 0x90, 0x90, //A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
            0xF0, 0x80, 0x80, 0x80, 0xF0, //C
            0xE0, 0x90, 0x90, 0x90, 0xE0, //D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
            0xF0, 0x80, 0xF0, 0x80, 0x80  //F
    };

public:
    Chip8 ();
    int LoadROM(std::string path);
    int LoadROM_Test (std::string path);
    void run();

    void tick();
};


#endif //EMU8_CHIP8_H
