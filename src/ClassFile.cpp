/**
 * @file ClassFile.cpp
 * @author Lettle (1071445082@qq.com)
 * @version 0.1
 * @date 2024-07-27
 * @copyright Copyright (c) 2024
 * 
 * @brief Lettle Java VM ClassFile class implementation
 */
#include <lvm.h>
#include <fstream>
#include <stack>

#include <lvm_util.h>
using namespace std;
using namespace lvm;

#define TAB showTab(tablevel)

ClassFile::ClassFile()
{

}

ClassFile::ClassFile(const char * filename)
{
    // Load the bytecode
    this->bytecode = load_class_file(filename);

    // 获取class文件基础信息
    this->magic = read32(bytecode, pc);
    this->minor_version = read16(bytecode, pc);
    this->major_version = read16(bytecode, pc);
    this->constant_pool_count = read16(bytecode, pc);

    // 创建常量池
    this->constant_pool.resize(this->constant_pool_count);
    // 常量获取
    for (int i=1; i<=constant_pool_count-1; i++) {
        Constant constant;
        constant.tag = read8(bytecode, pc);
        switch (constant.tag)
        {
        case CONSTANT_UTF8:
            constant.index = read16(bytecode, pc);  // 字符串长度
            constant.utf8_const = new uint8_t[constant.index+1];
            // 字符串内容
            for (int j=0; j<constant.index; j++) {
                constant.utf8_const[j] = read8(bytecode, pc);
            }
            constant.utf8_const[constant.index] = '\0';
            break;
        case CONSTANT_Class:
        case CONSTANT_String:
            constant.index = read16(bytecode, pc);  // name_index / string_index
            break;
        case CONSTANT_Fieldref:
        case CONSTANT_Methodref:
        case CONSTANT_NameAndType:
            constant.index = read16(bytecode, pc);  // name_index / class_index
            constant.name_and_type_index = read16(bytecode, pc);  // descriptor_index
            break;
        default:
            cout << "未知的常量tag: ";
            print_u8(constant.tag); cout << endl;
            exit(1);
        }
        this->constant_pool[i] = constant;
    }

    this->access_flags = read16(bytecode, pc);
    this->this_class = read16(bytecode, pc);
    this->super_class = read16(bytecode, pc);
    this->interfaces_count = read16(bytecode, pc);
    this->fields_count = read16(bytecode, pc);

    // Methods read
    this->methods_count = read16(bytecode, pc);
    this->methods.resize(this->methods_count);          // 将函数区重定大小
    for(int i=0; i<this->methods_count; i++) {
        this->methods[i] = new method_info();
        method_info* method = this->methods[i];

        method->access_flags = read16(bytecode, pc);
        method->name_index = read16(bytecode, pc);
        method->descriptor_index = read16(bytecode, pc);
        method->attributes_count = read16(bytecode, pc);

        // Mark main method
        string method_name = getUTF8(this->constant_pool[method->name_index].utf8_const);
        if (method_name == "main") this->main_method_index = i;
        
        // Read method attributes
        int attributes_count = method->attributes_count;
        for (int j=0; j<attributes_count; j++) {
            pc += 2;        // attribute_name_index (skip)
            int attribute_length = read32(bytecode, pc);
            
            method->max_stack = read16(bytecode, pc);
            method->max_locals = read16(bytecode, pc);
            method->code_length = read32(bytecode, pc);
            for (int k=0; k<method->code_length; k++) {
                instruction_info instruction;
                instruction.opcode = read8(bytecode, pc);
                instruction.hav_operand = get_instruction_value_size(instruction.opcode);
                switch (instruction.hav_operand)
                {
                case 0: break;
                case 1: instruction.operand = read8(bytecode, pc); break;
                case 2: instruction.operand = read16(bytecode, pc); break;
                }
                k += instruction.hav_operand;
                method->instructions.push_back(instruction);
            }

            // LineNumberTable (skip)
            uint32_t attributes_count = read32(bytecode, pc);
            uint16_t unknown = read16(bytecode, pc);
            uint16_t attributes_length = read32(bytecode, pc);
            pc += attributes_length;
        }

    }

    // Final attributes read
    this->attributes_count = read16(bytecode, pc);
    this->attributes.resize(this->attributes_count);
    for(int i=0; i<this->attributes_count; i++) {
        this->attributes[i] = &bytecode[pc];
    }
}

// Execute method
void ClassFile::execute(DataArea& vmdata, ExecEngine engine) 
{
    method_info* main_method = methods[this->main_method_index];
    engine.execute_method(vmdata, main_method);
}

char * ClassFile::getConstantClassName(Constant* c)
{
    return getUTF8(c->utf8_const);
}

// Load a class file into memory
uint8_t* ClassFile::load_class_file(const char * filename)
{
    ifstream infile(filename, ios::in | ios::binary);
    if (infile.is_open() == false) {
        cout << "读取class文件失败, 请检查类文件是否存在" << endl;
        exit(1);
    }

    // 重置 infile 的读指针
    infile.seekg(0, std::ios::end);
    int size = infile.tellg();
    uint8_t* data = new uint8_t[size];
    infile.seekg(0, std::ios::beg);
    infile.read(reinterpret_cast<char*>(data), size);
    infile.close();
    return data;
}

void showTab(int level)
{
    for (int i=0; i<level; i++) {
        cout << "  ";
    }
}

// 输出关于这个Java class文件的信息
void ClassFile::showInfo()
{
    int tablevel = 0;
    cout << "-----------------------\n";
    cout << "Using Java " << this->getJavaVersion() << endl;

    int pc_temp = 0;
    attribute_info attr_info;
    load_attribute_info(this->attributes[0], attr_info);
    cout << getUTF8(this->constant_pool[attr_info.info[1]].utf8_const) <<endl;

    cout << "magic number: ";
    print_u32(this->magic); cout << endl;
    cout << "minor_version: ";
    print_u16(this->minor_version); cout << endl;
    cout << "major_version: ";
    print_u16(this->major_version); cout << endl << endl;

    // Show modifiers 
    showModifier(this->access_flags); 
    if (havFlag(this->access_flags, ACC_INTERFACE)) cout << "interface ";
    else cout << "class ";
    // Show class name and the super class name
    Constant* class_temp = &this->constant_pool[this->this_class];
    class_temp = &this->constant_pool[class_temp->index];
    cout << getUTF8(class_temp->utf8_const) << " extends ";
    class_temp = &this->constant_pool[this->super_class];
    class_temp = &this->constant_pool[class_temp->index];
    cout << getUTF8(class_temp->utf8_const) << endl;
    cout << "{\n"; tablevel++;

    showTab(tablevel);cout << "constant pool count: " << this->constant_pool_count << endl;
    showTab(tablevel);cout << "interfaces_count: " << this->interfaces_count << endl;
    showTab(tablevel);cout << "fields_count: " << this->fields_count << endl;
    showTab(tablevel);cout << "methods_count: " << this->methods_count << endl << endl;

    // ------------------------
    // Print method info
    for(method_info* method : this->methods) {
        /**
         * Show begin
        */

        TAB;showModifier(method->access_flags);
        // Method name
        cout << getUTF8(this->constant_pool[method->name_index].utf8_const) << " " << getUTF8(this->constant_pool[method->descriptor_index].utf8_const) << endl;
        TAB;cout << "{\n"; tablevel++;
        
        // Method instructions
        for(instruction_info ins : method->instructions) {
            TAB;cout << get_instruction_name(ins.opcode);
            if (ins.hav_operand) {
                cout << " #" << (int)ins.operand;
            }
            cout << endl;
        }

        tablevel--;
        TAB;cout << "}\n"; 
    }

    cout << "}\n-----------------------\n";
}

// 通过 magic 是否等于 cafebabe 判断这个文件是不是Java class文件
bool ClassFile::isJavaClass()
{
    return !(this->magic ^ 0xcafebabe);
}

// 据说Java的主版本号是连续的，暂且用Java8作为标准进行计算
int ClassFile::getJavaVersion()
{
    return 8 + (this->major_version - 0x34);
}
