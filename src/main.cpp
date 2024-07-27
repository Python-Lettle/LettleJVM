/**
 * @file main.cpp
 * @author Lettle (1071445082@qq.com)
 * @version 0.1
 * @date 2024-07-27
 * @copyright Copyright (c) 2024
 * 
 * @brief Lettle Java VM main function
 */
#include <iostream>

#include <lvm.h>
#include <lvm_util.h>

using namespace std;
using namespace lvm;

int main ()
{
    // Load the class file
    ClassFile main_class("Main.class");

    if(DEBUG)
    if (main_class.isJavaClass()) {
        main_class.showInfo();
    }

    // Preparation
    DataArea vmdata = DataArea(&main_class);
    Constant *now_class = &vmdata.constant_pool[main_class.this_class];
    ExecEngine exec_engine(now_class);

    // Run
    main_class.execute(vmdata, exec_engine);
    

    return 0;
}