/**
 * @file DataArea.cpp
 * @author Lettle (1071445082@qq.com)
 * @version 0.1
 * @date 2024-07-27
 * @copyright Copyright (c) 2024
 * 
 * @brief Lettle Java VM DataArea class implementation
 */
#include <lvm.h>

using namespace std;
using namespace lvm;

DataArea::DataArea() 
{
    this->heap = new int[16];
    this->pc = -1;
}

DataArea::DataArea(ClassFile * classfile)
{
    this->method_area.resize(classfile->methods_count);
    for (int i=0; i<classfile->methods_count; i++)
    {
        this->method_area[i] = classfile->methods[i];
    }

    // this->constant_pool.resize(classfile->constant_pool_count);
    // for (int i=1; i<=classfile->methods_count-1; i++)
    // {
    //     this->constant_pool[i] = classfile->constant_pool[i];
    // }

    this->constant_pool.swap(classfile->constant_pool);
    this->heap = new int[16];
    this->pc = 0;
}

DataArea::~DataArea() 
{
}


