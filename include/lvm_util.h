#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <lvm.h>
#include <lvm_types.h>

// 检查有没有flag
bool havFlag(uint16_t access_flag, uint16_t modifier);

// 通过 access flag 输出当前的可见性修饰符
void showModifier(uint16_t access_flag);
// 传入常量池中 UTF8 常量的起始地址即可
char * getUTF8(uint8_t * utf8);
void load_attribute_info(uint8_t * data, lvm::attribute_info &info);


// print hex numbers
void print_u32(lvm::uint32_t hexnum);
void print_u16(lvm::uint16_t hexnum);
void print_u8(lvm::uint8_t hexnum);

// read hex numbers from file
// lvm::uint32_t read32(std::ifstream &file);
// lvm::uint16_t read16(std::ifstream &file);
// lvm::uint8_t read8(std::ifstream &file);

// read hex numbers from uint8_t stream
lvm::uint32_t read32(lvm::uint8_t* bytecode, int& pc);
lvm::uint16_t read16(lvm::uint8_t* bytecode, int& pc);
lvm::uint8_t read8(lvm::uint8_t* bytecode, int& pc);


#endif