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
        int pc_temp = 0, utf8_len;
        this->constant_pool[i] = &bytecode[pc];
        uint8_t tag = read8(this->constant_pool[i], pc_temp);
        switch (tag)
        {
        case CONSTANT_UTF8:
            utf8_len = read16(this->constant_pool[i], pc_temp);
            pc += 3+utf8_len;
            break;
        case CONSTANT_Class_info:
        case CONSTANT_String:
            pc += 3; 
            break;
        case CONSTANT_Fieldref:
        case CONSTANT_Methodref:
        case CONSTANT_NameAndType:
            pc += 5; 
            break;
        default:
            cout << "未知的常量tag: ";
            print_u8(tag); cout << endl;
            exit(1);
        }
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
        this->methods[i] = &bytecode[pc];               // 将函数起始部分放入 method_area
        pc += 2;
        string method_name = getUTF8(this->constant_pool[read16(bytecode, pc)]);
        if (method_name == "main") this->main_method_index = i;
        pc += 2;                                        // 跳过 3个u2大小的数据
        int attributes_count = read16(bytecode, pc);
        for (int j=0; j<attributes_count; j++) {
            pc += 2;
            int attribute_length = read32(bytecode, pc);
            pc += attribute_length;
        }

    }

    this->attributes_count = read16(bytecode, pc);
    this->attributes.resize(this->attributes_count);
    for(int i=0; i<this->attributes_count; i++) {
        this->attributes[i] = &bytecode[pc];
    }
    
}

// Execute method
void ClassFile::execute() 
{
    int pc_temp = 0;
    uint8_t * main_method_p = methods[this->main_method_index];
    // Read method info
    method_info main_method;
    main_method.access_flags = read16(main_method_p, pc_temp);
    main_method.name_index = read16(main_method_p, pc_temp);
    main_method.descriptor_index = read16(main_method_p, pc_temp);
    main_method.attributes_count = read16(main_method_p, pc_temp);
    execute_method(main_method);
}

void ClassFile::execute_method(method_info method)
{
    stack<int> runtime_stack;
    for (int i=0; i<method.attributes_count; i++) {
        // Read each instruction and run
    }
}

char * ClassFile::getConstantClassName(CONSTANT_Class c)
{
    return getUTF8(this->constant_pool[c.name_index]);
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
    cout << getUTF8(this->constant_pool[attr_info.info[1]]);

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
    CONSTANT_Class class_temp;

    uint8_t * p1 = this->constant_pool[this->this_class];
    pc_temp = 0;
    class_temp.tag = read8(p1, pc_temp);
    class_temp.name_index = read16(p1, pc_temp);
    cout << this->getConstantClassName(class_temp) << " extends ";

    p1 = this->constant_pool[this->super_class];
    pc_temp = 0;
    class_temp.tag = read8(p1, pc_temp);
    class_temp.name_index = read16(p1, pc_temp);
    cout << this->getConstantClassName(class_temp) << endl;
    cout << "{\n"; tablevel++;

    showTab(tablevel);cout << "constant pool count: " << this->constant_pool_count << endl;
    showTab(tablevel);cout << "interfaces_count: " << this->interfaces_count << endl;
    showTab(tablevel);cout << "fields_count: " << this->fields_count << endl;
    showTab(tablevel);cout << "methods_count: " << this->methods_count << endl << endl;

    // ------------------------
    // Print method info
    // ------------------------
    method_info method_temp;
    for(int i=0; i<this->methods_count; i++) {
        /**
         * Read info
        */
        pc_temp = 0;
        method_temp.access_flags = read16(this->methods[i], pc_temp);
        method_temp.name_index = read16(this->methods[i], pc_temp);
        method_temp.descriptor_index = read16(this->methods[i], pc_temp);
        method_temp.attributes_count = read16(this->methods[i], pc_temp);
        method_temp.attributes = new attribute_info[method_temp.attributes_count];
        for(int i=0; i<method_temp.attributes_count; i++) {
            method_temp.attributes[i].attribute_name_index = read16(this->methods[i], pc_temp);
            method_temp.attributes[i].attribute_length = read32(this->methods[i], pc_temp);
            method_temp.attributes[i].info = this->methods[i] + pc_temp;
        }
        
        /**
         * Show begin
        */

        TAB;showModifier(method_temp.access_flags);
        // Method name
        cout << getUTF8(this->constant_pool[method_temp.descriptor_index]) << " " << getUTF8(this->constant_pool[method_temp.name_index]) << endl;
        TAB;cout << "{\n"; tablevel++;
        
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
