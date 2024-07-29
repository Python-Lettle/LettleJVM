/**
 * @file util.cpp
 * @author Lettle (1071445082@qq.com)
 * @version 0.1
 * @date 2024-07-27
 * @copyright Copyright (c) 2024
 * 
 * @brief Lettle Java VM util function implementation
 */
#include <lvm_util.h>
#include <lvm.h>
#include <lvm_types.h>

#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;
using namespace lvm;

void load_attribute_info(uint8_t * data, attribute_info &info)
{
    int pc_temp = 0;
    info.attribute_name_index = read16(data, pc_temp);
    info.attribute_length = read32(data, pc_temp);
    info.info = data + pc_temp;
}

// 传入常量池中 UTF8 常量的起始地址即可
char * getUTF8(uint8_t * utf8)
{
    return (char *)utf8;
}

// 通过 access flag 输出当前的可见性修饰符
void showModifier(uint16_t access_flag)
{
    if (havFlag(access_flag, ACC_PUBLIC)) cout << "public ";
    else if (havFlag(access_flag, ACC_PRIVATE)) cout << "private ";
    else if (havFlag(access_flag, ACC_PROTECTED)) cout << "protected ";

    if (havFlag(access_flag, ACC_ABSTRACT)) cout << "abstract ";

    if (havFlag(access_flag, ACC_STATIC)) cout << "static ";
    if (havFlag(access_flag, ACC_FINAL)) cout << "final ";
    // if (havFlag(access_flag, ACC_VOLATILE)) cout << "volatile ";
    // if (havFlag(access_flag, ACC_TRANSIDENT)) cout << "transident ";
    if (havFlag(access_flag, ACC_NATIVE)) cout << "native ";
    
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

void print_stack(std::stack<stack_frame>& s)
{
    if (s.empty()) {
        std::cout << "Stack is empty." << std::endl;
        return;
    }

    std::stack<stack_frame> temp;
    while (!s.empty()) {
        temp.push(s.top());
        s.pop();
    }
  
    // 输出元素
    cout << "-----Stack-----" << endl;
    while (!temp.empty()) {
        std::cout << temp.top().value << " ";
        stack_frame topElement = temp.top();
        temp.pop();
        s.push(topElement); // 重新将元素压回原始栈
    }
    cout << endl << "---------------" << endl;

    std::cout << std::endl;
}

string get_instruction_name(uint8_t opcode)
{
    switch(opcode)
    {
        case NOP: return "NOP";
        case POP: return "POP";
        case POP2: return "POP2";
        case DUP: return "DUP";
        case DUP_X1: return "DUP_X1";
        case DUP_X2: return "DUP_X2";
        case DUP2: return "DUP2";
        case DUP2_X1: return "DUP2_X1";
        case DUP2_X2: return "DUP2_X2";
        case SWAP: return "SWAP";
        
        case ACONST_NULL: return "ACONST_NULL";
        case ICONST_M1: return "ICONST_M1";
        case ICONST_0: return "ICONST_0";
        case ICONST_1: return "ICONST_1";
        case ICONST_2: return "ICONST_2";
        case ICONST_3: return "ICONST_3";
        case ICONST_4: return "ICONST_4";
        case ICONST_5: return "ICONST_5";
        case LCONST_0: return "LCONST_0";
        case LCONST_1: return "LCONST_1";
        case FCONST_0: return "FCONST_0";
        case FCONST_1: return "FCONST_1";

        case BIPUSH: return "BIPUSH";
        case SIPUSH: return "SIPUSH";
        case LDC: return "LDC";
        case LDC_W: return "LDC_W";
        case LDC2_W: return "LDC2_W";

        case ALOAD: return "ALOAD";
        case ALOAD_0: return "ALOAD_0";
        case ALOAD_1: return "ALOAD_1";
        case ALOAD_2: return "ALOAD_2";
        case ALOAD_3: return "ALOAD_3";
        case ILOAD: return "ILOAD";
        case LLOAD: return "LLOAD";
        case FLOAD: return "FLOAD";
        case DLOAD: return "DLOAD";
        case ILOAD_0: return "ILOAD_0";
        case ILOAD_1: return "ILOAD_1";
        case ILOAD_2: return "ILOAD_2";
        case ILOAD_3: return "ILOAD_3";
        case LLOAD_0: return "LLOAD_0";
        case LLOAD_1: return "LLOAD_1";
        case LLOAD_2: return "LLOAD_2";
        case LLOAD_3: return "LLOAD_3";
        case FLOAD_0: return "FLOAD_0";
        case FLOAD_1: return "FLOAD_1";
        case FLOAD_2: return "FLOAD_2";
        case FLOAD_3: return "FLOAD_3";
        case DLOAD_0: return "DLOAD_0";
        case DLOAD_1: return "DLOAD_1";
        case DLOAD_2: return "DLOAD_2";
        case DLOAD_3: return "DLOAD_3";
        case IALOAD: return "IALOAD";
        case LALOAD: return "LALOAD";
        case FALOAD: return "FALOAD";
        case DALOAD: return "DALOAD";
        case AALOAD: return "AALOAD";
        case CALOAD: return "CALOAD";
        case SALOAD: return "SALOAD";

        case ISTORE_0: return "ISTORE_0";
        case ISTORE_1: return "ISTORE_1";
        case ISTORE_2: return "ISTORE_2";
        case ISTORE_3: return "ISTORE_3";

        case IADD: return "IADD";
        case ISUB: return "ISUB";
        case IMUL: return "IMUL";
        case IDIV: return "IDIV";
        case IREM: return "IREM";

        case IF_ICMPEQ: return "IF_ICMPEQ";
        case IF_ICMPNE: return "IF_ICMPNE";
        case IF_ICMPLT: return "IF_ICMPLT";
        case IF_ICMPGE: return "IF_ICMPGE";
        case IF_ICMPGT: return "IF_ICMPGT";
        case IF_ICMPLE: return "IF_ICMPLE";
        case IF_ACMPEQ: return "IF_ACMPEQ";
        case IF_ACMPNE: return "IF_ACMPNE";

        case GOTO: return "GOTO";
        case GOTO_W: return "GOTO_W";

        case GETSTATIC: return "GETSTATIC";

        case INVOKEVIRTUAL: return "INVOKEVIRTUAL";
        case INVOKESPECIAL: return "INVOKESPECIAL";
        case INVOKESTATIC: return "INVOKESTATIC";
        case INVOKEINTERFACE: return "INVOKEINTERFACE";

        case RETURN: return "RETURN";

        case MONITORENTER: return "MONITORENTER";
        case MONITOREXIT: return "MONITOREXIT";

        default: return "#" + to_string(opcode);
    }
}

int get_instruction_value_size(uint8_t opcode)
{
    switch (opcode)
    {
        case BIPUSH:
        case SIPUSH:
        case LDC:
            return 1;
        case IF_ICMPEQ:
        case IF_ICMPNE:
        case IF_ICMPLT:
        case IF_ICMPGE:
        case IF_ICMPGT:
        case IF_ICMPLE:
        case IF_ACMPEQ:
        case IF_ACMPNE:
        case GOTO:
        case GETSTATIC:
        case INVOKEVIRTUAL:
        case INVOKESPECIAL:
        case INVOKESTATIC:
            return 2;
    }
    return 0;
}