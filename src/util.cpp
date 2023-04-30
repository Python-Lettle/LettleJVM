#include <util.h>
#include <lvm.h>
#include <lvm_types.h>

#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;
using namespace lvm;

// 通过 access flag 输出当前的可见性修饰符
void showModifier(uint16_t access_flag)
{
    if (havFlag(access_flag, ACC_PUBLIC)) cout << "public ";
    else if (havFlag(access_flag, ACC_PRIVATE)) cout << "private ";
}

bool havFlag(uint16_t access_flag, uint16_t modifier)
{
    return ((access_flag & modifier) == modifier);
}

// Parse a u1 value from the bytecode and update the program counter
uint8_t read8(uint8_t* bytecode, int& pc) 
{
    return bytecode[pc++];
}

// Parse a u2 value from the bytecode and update the program counter
uint16_t read16(uint8_t* bytecode, int& pc) 
{
    uint16_t value = (bytecode[pc++] << 8) | bytecode[pc++];
    return value;
}

// Parse a u4 value from the bytecode and update the program counter
uint32_t read32(uint8_t* bytecode, int& pc) 
{
    uint32_t value = (bytecode[pc++] << 24) | (bytecode[pc++] << 16) | (bytecode[pc++] << 8) | bytecode[pc++];
    // uint32_t value = (bytecode[pc++]) | (bytecode[pc++] << 8) | (bytecode[pc++] << 16) | (bytecode[pc++] << 24);
    return value;
}

// CA FE BA BE储存是BEBAFECA
// 因此输出的时候从右向左输出
void print_u32(lvm::uint32_t hexnum)
{
    print_u8(hexnum);
    print_u8(hexnum >> 8);
    print_u8(hexnum >> 16);
    print_u8(hexnum >> 24);
}

// 00 34 即代表 0x34
void print_u16(lvm::uint16_t hexnum)
{
    print_u8(hexnum >> 8);
    print_u8(hexnum);
}

void print_u8(lvm::uint8_t hexnum)
{
    char show[10]; 
    snprintf(show, 10, "%02X ", hexnum);
    printf("%s", show);
}


