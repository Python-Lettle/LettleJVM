#include <lvm.h>
#include <fstream>
#include <util.h>
using namespace std;
using namespace lvm;

ClassFile::ClassFile()
{

}

ClassFile::ClassFile(string filename)
{
    ifstream infile(filename, ios::in | ios::binary);
    if (infile.is_open() == false) {
        cout << "创建文件类实例失败，请检查类文件是否存在" << endl;
    }
    
    this->magic = read32(infile);
    this->minor_version = read16(infile);
    this->major_version = read16(infile);
    this->constant_pool_count = read16(infile);
}

// 输出关于这个Java class文件的信息
void ClassFile::showInfo()
{
    cout << "-----------------------\n";
    cout << "magic number: ";
    print_u32(this->magic); cout << endl;
    cout << "minor_version: ";
    print_u16(this->minor_version); cout << endl;
    cout << "major_version: ";
    print_u16(this->major_version); cout << endl;
    cout << "constant pool count: " << this->constant_pool_count << endl;
}

// 通过 magic 是否等于 cafebabe 判断这个文件是不是Java class文件
bool ClassFile::isJavaClass()
{
    return !(this->magic ^ 0xbebafeca);
}

// 据说Java的主版本号是连续的，暂且用Java8作为标准进行计算
int ClassFile::getJavaVersion()
{
    return 8 + (this->major_version - 0x34);
}
