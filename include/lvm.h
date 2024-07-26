#ifndef JVM_H
#define JVM_H

#include <iostream>
#include <vector>
#include <stack>
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

// 常量类型
enum CONSTANT_TYPES
{
    CONSTANT_UTF8      = 0x01,
    CONSTANT_Class_info= 0x07,
    CONSTANT_String    = 0x08,
    CONSTANT_Fieldref  = 0x09,
    CONSTANT_Methodref = 0x0A,
    CONSTANT_NameAndType = 0x0C
};

// 可见性修饰符列表
enum MODIFIER_CONST
{
    // Class
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
    ACC_STRICT = 2048,

    // TODO: need some const for methods
};

// 指令与助记符
enum INSTRUCTION_ENUM
{
    // 通用(无类型)栈操作指令
    NOP = 0x00,             // 空操作
    POP = 0x57,             // 从栈顶弹出一个字长的数据。
    POP2,                   // 从栈顶弹出两个字长的数据。
    DUP,                    // 复制栈顶一个字长的数据，将复制后的数据压栈。
    DUP_X1,                 // 复制栈顶一个字长的数据，弹出栈顶两个字长数据，先将复制后的数据压栈，再将弹出的两个字长数据压栈。
    DUP_X2,                 // 复制栈顶一个字长的数据，弹出栈顶三个字长的数据，将复制后的数据压栈，再将弹出的三个字长的数据压栈。
    DUP2,                   // 复制栈顶两个字长的数据，将复制后的两个字长的数据压栈。
    DUP2_X1,                // 复制栈顶两个字长的数据，弹出栈顶三个字长的数据，将复制后的两个字长的数据压栈，再将弹出的三个字长的数据压栈。
    DUP2_X2,                // 复制栈顶两个字长的数据，弹出栈顶四个字长的数据，将复制后的两个字长的数据压栈，再将弹出的四个字长的数据压栈。
    SWAP = 0x5f,            // 交换栈顶两个字长的数据的位置。Java指令中没有提供以两个字长为单位的交换指令。
    // 常量入栈指令
    ACONST_NULL = 1,
    ICONST_M1,
    ICONST_0,
    ICONST_1,
    ICONST_2,
    ICONST_3,
    ICONST_4,
    ICONST_5,
    LCONST_0,
    LCONST_1,
    FCONST_0,
    FCONST_1,
    FCONST_2,
    DCONST_0,
    DCONST_1,
    BIPUSH,
    SIPUSH,
    LDC,
    LDC_W,
    LDC2_W = 0x14,

    // 局部变量值转载到栈中指令
    ALOAD = 0x19,
    ALOAD_0 = 0x2a,
    ALOAD_1,
    ALOAD_2,
    ALOAD_3,
    ILOAD = 0x15,
    LLOAD,
    FLOAD,
    DLOAD,
    ILOAD_0 = 0x1a,
    ILOAD_1,
    ILOAD_2,
    ILOAD_3,
    LLOAD_0 = 0x1e,
    LLOAD_1,
    LLOAD_2,
    LLOAD_3,
    FLOAD_0 = 0x22,
    FLOAD_1,
    FLOAD_2,
    FLOAD_3,
    DLOAD_0 = 0x26,
    DLOAD_1,
    DLOAD_2,
    DLOAD_3,
    IALOAD = 0x2e,          // 从int类型数组中装载指定项的值。
    LALOAD,                 // 从long类型数组中装载指定项的值。
    FALOAD,                 // 从float类型数组中装载指定项的值。
    DALOAD,                 // 从double类型数组中装载指定项的值。
    AALOAD,                 // 从引用类型数组中装载指定项的值。
    CALOAD,                 // 从char类型数组中装载指定项的值（先转换为int类型值，后压栈）。
    SALOAD = 0x35,          // 从short类型数组中装载指定项的值（先转换为int类型值，后压栈）。
    
    // 将栈顶值保存到局部变量中指令 TODO

    // wide 指令
    WIDE = 0xc4,            // 使用附加字节扩展局部变量索引（iinc指令特殊）。

    // 类型转换指令 TODO
    // 整数运算 TODO
    // 浮点运算 TODO
    // 逻辑运算——移位运算 TODO
    // 逻辑运算——按位布尔运算 TODO
    // 控制流指令——条件跳转指令 TODO
    // 控制流指令——比较指令 TODO
    // 控制流指令——无条件跳转指令
    GOTO = 0xa7,
    GOTO_W = 0xc8,
    // 对象操作指令
    NEW = 0xbb,
    CHECKCAST = 0xc0,
    INSTANCEOF,
    GETFIELD = 0xb4,
    PUTFIELD = 0xb5,
    GETSTATIC = 0xb2,
    PUTSTATIC = 0xb3,
    // 数组操作指令
    NEWARRAY = 0xbc,        // 创建type类型的数组。
    ANEWARRAY,              // 创建引用类型的数组。
    ARRAYLENGTH,            // array length: 获取一维数组的长度。
    MULTIANEWARRAY = 0xc5,  // 创建dimension维度的数组。
    // 方法调用指令
    INVOKESPECIAL = 0xb7,          // 编译时方法绑定调用方法。
    INVOKEVIRTUAL = 0xb6,          // 运行时方法绑定调用方法。
    INVOKESTATIC = 0xb8,           // 调用静态方法。
    INVOKEINTERFACE = 0xb9,        // 调用接口方法。
    // 方法返回指令 TODO
    RETURN = 0xb1,
    // 线程同步指令
    MONITORENTER = 0xc2,
    MONITOREXIT = 0XC3
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

typedef struct attribute_info
{
    uint16_t attribute_name_index;
    uint32_t attribute_length;
    uint8_t *info;
}attribute_info;

typedef struct method_info
{
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
    attribute_info * attributes;
}method_info;

typedef struct CONSTANT_Class
{
    uint8_t tag;
    uint16_t name_index;
}CONSTANT_Class;

class LettleVM
{
    public:
    LettleVM();
    ~LettleVM();

    private:
    std::stack<int> jvm_stack;
    int * heap;
};


// -------------------------
// ClassFile class
// -------------------------

class ClassFile
{
public:
    ClassFile();
    ClassFile(const char * filename);

    uint8_t* load_class_file(const char * filename);
    void execute(LettleVM &vm);
    void execute(LettleVM &vm, uint8_t * bytecode, int& pc);
    void execute_method(LettleVM &vm, method_info method);

    char * getConstantClassName(CONSTANT_Class c);
    char * getSuperClassName(CONSTANT_Class c);

    bool isJavaClass();
    int getJavaVersion();
    void showInfo();
private:
    int pc = 0;
    uint8_t * bytecode;

    // Define the JVM's stack
    // std::vector<int> jvm_stack = std::vector<int>(STACK_SIZE);
    // int sp = -1; // Stack pointer

    // Define the runtime constant pool
    // std::vector<int> runtime_const_pool = std::vector<int>(RUNTIME_CONST_POOL_SIZE);
    // int num_runtime_constants = 0;

    uint32_t magic;                             // cafebabe
    uint16_t minor_version;                     // 副版本号
    uint16_t major_version;                     // 主版本号

    uint16_t constant_pool_count;               // 常量池元素个数
    std::vector<uint8_t*> constant_pool;        // 常量池

    uint16_t access_flags;
    uint16_t this_class;
    uint16_t super_class;
    uint16_t interfaces_count;
    std::vector<uint8_t *> interfaces;

    uint16_t fields_count;
    std::vector<uint8_t *> fields;

    // 类方法
    uint16_t methods_count;
    int main_method_index;                  // 方法区中main方法所在下标
    std::vector<uint8_t*> methods;          // 方法区
    
    uint16_t attributes_count;
    std::vector<uint8_t*> attributes;
};

}

#endif