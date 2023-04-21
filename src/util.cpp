#include <util.h>
#include <lvm_types.h>
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;
using namespace lvm;

uint32_t read32(ifstream &file)
{
    uint32_t temp;
    file.read((char*)&temp, sizeof(uint32_t));
    return temp;
}

uint16_t read16(ifstream &file)
{
    uint16_t result = 0;
    uint8_t temp = 0;
    file.read((char*)&temp, sizeof(uint8_t));
    result = temp << 2;
    file.read((char*)&temp, sizeof(uint8_t));
    result += temp;
    return result;
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
