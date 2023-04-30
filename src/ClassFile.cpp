#include <lvm.h>
#include <fstream>

#include <lvm_util.h>
using namespace std;
using namespace lvm;

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
        pc += 6;                                        // 跳过 3个u2大小的数据
        int attributes_count = read16(bytecode, pc);
        for (int j=0; j<attributes_count; j++) {
            pc += 2;
            int attribute_length = read32(bytecode, pc);
            pc += attribute_length;
        }

    }

    this->attributes_count = read16(bytecode, pc);
}

// Execute method
void ClassFile::execute() 
{
    
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

// 输出关于这个Java class文件的信息
void ClassFile::showInfo()
{
    cout << "-----------------------\n";
    cout << "Using Java " << this->getJavaVersion() << endl;

    cout << "magic number: ";
    print_u32(this->magic); cout << endl;
    cout << "minor_version: ";
    print_u16(this->minor_version); cout << endl;
    cout << "major_version: ";
    print_u16(this->major_version); cout << endl << endl;

    // Show modifiers 
    showModifier(this->access_flags); 
    
    // Show class name and the super class name
    CONSTANT_Class temp;

    uint8_t * p1 = this->constant_pool[this->this_class];
    int pc_temp = 0;
    temp.tag = read8(p1, pc_temp);
    temp.name_index = read16(p1, pc_temp);
    cout << this->getConstantClassName(temp) << " extends ";

    p1 = this->constant_pool[this->super_class];
    pc_temp = 0;
    temp.tag = read8(p1, pc_temp);
    temp.name_index = read16(p1, pc_temp);
    cout << this->getConstantClassName(temp) << endl;
    cout << "{\n";

    cout << "  constant pool count: " << this->constant_pool_count << endl;

    cout << "  interfaces_count: " << this->interfaces_count << endl;

    cout << "  fields_count: " << this->fields_count << endl;

    cout << "  methods_count: " << this->methods_count << endl;

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
