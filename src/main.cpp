#include <iostream>
#include <lvm.h>
#include <util.h>

using namespace std;
using namespace lvm;

int main ()
{
    cout << "Lettle Java VM V1.0" << endl;
    
    ClassFile test_class = ClassFile("Main17.class");

    if (test_class.isJavaClass()) {
        test_class.showInfo();
    }

    test_class.execute();
    

    return 0;
}