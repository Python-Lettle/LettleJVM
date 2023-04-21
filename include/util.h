#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <lvm_types.h>

// print hex numbers
void print_u32(lvm::uint32_t hexnum);
void print_u16(lvm::uint16_t hexnum);
void print_u8(lvm::uint8_t hexnum);

// read hex numbers from file
lvm::uint32_t read32(std::ifstream &file);
lvm::uint16_t read16(std::ifstream &file);

#endif