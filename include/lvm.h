#ifndef JVM_H
#define JVM_H

#include <iostream>
#include <vector>
#include <lvm_types.h>

#define STACK_SIZE 100
#define METHOD_AREA_SIZE 10
#define CONST_POOL_SIZE 10
#define RUNTIME_CONST_POOL_SIZE 10

// Var structs in bin
namespace lvm
{

// -------------------------
// Enums
// -------------------------
enum CONSTANT_TYPES
{
    CONSTANT_UTF8      = 0x01,
    CONSTANT_Class_info= 0x07,
    CONSTANT_String    = 0x08,
    CONSTANT_Fieldref  = 0x09,
    CONSTANT_Methodref = 0x0A,
    CONSTANT_NameAndType = 0x0C
};

// 可见行修饰符列表
enum MODIFIER_CONST
{
    ACC_PUBLIC = 1,
    ACC_PRIVATE = 2,
    ACC_PROTECTED = 4,
    ACC_STATIC = 8,
    ACC_FINAL = 16,
    ACC_SUPER = 32,
    ACC_VOLATILE = 64,
    ACC_TRANSIDENT = 128,
    ACC_NATIVE = 256,
    ACC_INTERFACE = 512,
    ACC_ABSTRACT = 1024,
    ACC_STRICT = 2048
};

// -------------------------
// Data Structs
// -------------------------

typedef struct cp_info
{
    uint8_t tag;
    uint16_t class_index;
    uint16_t name_and_type_index;
}cp_info;

typedef struct method_info
{
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
}method_info;


// -------------------------
// ClassFile class
// -------------------------

class ClassFile
{
public:
    ClassFile();
    ClassFile(const char * filename);

    uint8_t* load_class_file(const char * filename);
    void execute();
    void execute(uint8_t * bytecode, int& pc);

    bool isJavaClass();
    int getJavaVersion();
    void showInfo();
private:
    int pc = 0;
    uint8_t * bytecode;

    // Define the JVM's stack
    std::vector<int> stack = std::vector<int>(STACK_SIZE);
    int sp = -1; // Stack pointer

    // Define the runtime constant pool
    std::vector<int> runtime_const_pool = std::vector<int>(RUNTIME_CONST_POOL_SIZE);
    int num_runtime_constants = 0;

    uint32_t magic;                             // cafebabe
    uint16_t minor_version;                     // 副版本号
    uint16_t major_version;                     // 主版本号

    uint16_t constant_pool_count;               // 常量池元素个数
    std::vector<uint8_t*> constant_pool;        // 常量池

    uint16_t access_flags;
    uint16_t this_class;
    uint16_t super_class;
    uint16_t interfaces_count;
    // Here might something add
    uint16_t fields_count;
    // Here might something add
    uint16_t methods_count;
    std::vector<uint8_t*> methods;          // 方法区
    
    uint16_t attributes_count;
    std::vector<uint8_t*> attributes;
};

}

#endif