#ifndef JVM_H
#define JVM_H

#include <iostream>
#include <lvm_types.h>

// Var structs in bin
namespace lvm
{

class ClassFile
{
public:
    ClassFile();
    ClassFile(std::string filename);

    bool isJavaClass();
    int getJavaVersion();
    void showInfo();
private:
    uint32_t magic;                 // cafebabe
    uint16_t minor_version;
    uint16_t major_version;         // Java Version
    uint16_t constant_pool_count;   // Amount of constants
};

}

#endif