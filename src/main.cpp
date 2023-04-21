#include <iostream>
#include <lvm.h>
#include <util.h>

using namespace std;
using namespace lvm;

int main ()
{
    cout << "Lettle Java VM V1.0" << endl;
    
    ClassFile test_class("Main.class");

    if (test_class.isJavaClass()) {
        cout << "This file using Java " << test_class.getJavaVersion() << endl;
        test_class.showInfo();
    }
    

    return 0;
}