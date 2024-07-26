#include <iostream>

#include <lvm.h>
#include <lvm_util.h>

using namespace std;
using namespace lvm;

int main ()
{
    LettleVM vm;
    cout << "Lettle Java VM V1.0" << endl;
    
    ClassFile test_class("Main.class");

    if (test_class.isJavaClass()) {
        test_class.showInfo();
    }

    test_class.execute(vm);
    

    return 0;
}