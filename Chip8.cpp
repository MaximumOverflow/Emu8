//
// Created by max on 07/01/19.
//

#include <fstream>
#include <iostream>
#include "Chip8.h"

Chip8::Chip8() {
    cycles = 0;

    for (int8 i : RAM)  RAM[i] = 0;
    for (int8 i : V)    V[i] = 0;
    I = 0;
    DT = 0; ST = 0;
    PC = 0x200;
    SP = 0;
    for (int16 i : stack.memory)   stack.memory[i] = 0;
    draw = false;
    OP_Code = 0;

    for (auto &pixel : pixels) for (auto y : pixel) {pixel[y] = false;}
    for (int i = 0; i < 5*16; ++i) {
        RAM[i] = fonts[i];
    }
}

//int Chip8::LoadROM(std::string path)
//{
//    std::ifstream ROM(path, std::ifstream::binary);
//
//    if (!ROM.is_open())
//    {
//        std::cout << "[ROM Error] The file was not found.\n";
//        return -1;
//    }
//    int i = 0x200;
//
//    std::cout << "ROM dump: ";
//    while (ROM>>RAM[i]) {std::cout << RAM[i]; i++;};
//    std::cout << '\n';
//
//    ROM.close();
//}

void Chip8::run() {
    while (true) tick();
}

void Chip8::tick()
{
    cycles++;
    draw = false;

    //TODO Implement OP Codes

    OP_Code = RAM[PC] << 8 | RAM[PC+1];
    dump_OP_Code();

    uint8_t x = (OP_Code & 0x0F00)/16/16;
    uint8_t y = (OP_Code & 0x00F0)/16;
    uint8_t kk = (OP_Code & 0x00FF);

    PC+=2;

    switch (OP_Code & 0xF000){

        //0NNN Ignored

        case 0x0000: //Clear Screen
            if (OP_Code == 0x00E0)
            {
                for (auto &pixel : pixels) for (auto y : pixel) {pixel[y] = false;}
            }

            if (OP_Code == 0x00EE)
            {
                PC = stack.memory[0];
                SP--;
            }
            break;

        case 0x1000:
            PC = (OP_Code & 0x0FFF);
            break;

        case 0x2000:
            SP++;
            stack<<PC;
            PC = (OP_Code & 0x0FFF);
            break;

        case 0x3000:
            if (V[x] == kk)
                PC += 2;
            break;

        case 0x4000:
            if (V[x] != kk)
                PC += 2;
            break;

        case 0x5000:
            if (V[x] == V[y])
                PC += 2;
            break;

        case 0x6000:
            V[x] = kk;
            break;

        case 0x7000:
            V[x] += kk;
            break;

        case 0x8000:
        {
            //TODO Optimize Code
            switch (OP_Code & 0x000F)
            {
                case 0x0000:
                    V[x] = V[y];
                    break;
                case 0x0001:
                    V[x] = V[x] | V[y];
                    break;
                case 0x0002:
                    V[x] = V[x] & V[y];
                    break;
                case 0x0003:
                    V[x] = V[x] ^ V[y];
                    break;
                case 0x0004: {
                    int16_t Vnum = V[x] + V[y];
                    V[0xF] = (Vnum > 0x00FF);
                    V[x] = (Vnum & 0x00FF);
                    break;
                }
                case 0x0005:
                    if (V[x] > V[y])
                        V[0xF] = true;
                    else
                        V[0xF] = false;
                    V[x] -= V[y];
                    break;
                    //TODO Continue 0x8000 op codes
                default:
                    dump_Unimplemented_OP_Code();
                    throw "Error";
                    break;
            }
            break;

            case 0xA000:
                I = (OP_Code & 0x0FFF);
                break;

            case 0xC000:
            {
                int8_t random = rand()%255;
                V[x] = (random & 0x00FF);
                break;
            }


            case 0xD000:
                for (int Y = 0; Y < (OP_Code & 0x000F); ++Y) {
                    int8 spriteLoad;
                    if (I + Y <= 4096)
                        spriteLoad = RAM[I + Y];
                    else
                        Debugger::printError("Interpreter", "RAM Access out of bounds, the program might not work as intended from now on");

                    for (int X = 0; X < 8; ++X) {
                        if (pixels[V[x]+X][V[y]+Y] == true && (pixels[V[x]+X][V[y]+Y] ^ (spriteLoad & 0x80)) == false)
                            V[0xF] = true;
                        pixels[V[x]+X][V[y]+Y] ^= (spriteLoad & 0x80);
                        spriteLoad<<=1;
                    }
                }
                draw = true;
                break;

            case 0xE000:
                switch (OP_Code & 0x00FF)
                {
                    case 0x00A1:
                        if(!key[V[x]])
                            PC+=2;
                        break;
                    case 0x009E:
                        for (int i = 0; i < 16; ++i) {
                            if(key[V[x]] == true)
                            {
                                PC+=2;
                                break;
                            }
                        }
                        break;
                    default:
                        dump_Unimplemented_OP_Code();
                        throw "Error";
                        break;
                }
                break;

            case 0xF000:
            {
                switch (OP_Code & 0x00FF) {
                    case 0x0007:
                        V[x] = DT;
                        break;
                    case 0x0015:
                        DT = V[x];
                        break;
                    case 0x0018:
                        ST = V[x];
                        break;
                    case 0x001E:
                        I+=V[x];
                        break;
                    case 0x0029:
                        I = RAM[V[x] * 5];
                        break;
                    case 0x0033:
                        RAM[I] = (int)(V[x] / 100);
                        RAM[I+1] = (int)(V[x] / 10) % 10;
                        RAM[I+2] = (int)(V[x] % 100) % 10;
                        break;
                    case 0x0065:
                        for (int i = 0; i <= x; ++i) {
                            V[i] = RAM[I+i];
                        }
                        break;
                    default:
                        dump_Unimplemented_OP_Code();
                        throw "Error";
                        break;
                }
            }
            break;
        }
        default:
            dump_Unimplemented_OP_Code();
            throw "Error";
            break;
    }

    if (DT > 0) DT--;
    if (ST >0) ST--;

    //TODO Fix Rendering
    if (draw) {
        system("clear");
        for (int y = 0; y < 32; ++y) {
            for (int x = 0; x < 64; ++x) {
                if (pixels[x][y])
                    std::cout << '#';
                else
                    std::cout << ' ';
            }
            std::cout << '\n';
        }
        draw = false;
    }
    //TODO Implement Input
}

int Chip8::LoadROM_Test(std::string path) {
    //TODO Implement ROM Loading from scratch
    std::ifstream file;
    size_t file_size;

    file.open(path, std::ios::in | std::ios::binary);

    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        file_size = file.tellg();

        // Test if the ROM will feet in RAM
        if (file_size > 0xFFF - 0x200) {
            std::cerr << "Error ROM cannot fit in RAM" << std::endl;
        } else // OK
        {
            char *data_rom = (char *) (&(RAM[0x200]));

            file.seekg(0, std::ios::beg);
            file.read(data_rom, file_size);
        }

        file.close();
        return 0;
    }
}

void Chip8::dump_Unimplemented_OP_Code()
{
    std::cout << "[Interpreter Error] Unimplemented OP Code (" << std::hex << OP_Code << ") at position " << PC << " on cycle " << std::dec << cycles<< "\n";
}

void Chip8::dump_OP_Code() {
    std::cout << "[Debug Info] Current OP Code:" << std::hex << OP_Code << " at position " << PC << " on cycle " << std::dec << cycles<< "\n";
}
