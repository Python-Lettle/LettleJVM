#include <lvm.h>

using namespace std;
using namespace lvm;

LettleVM::LettleVM() {
    this->heap = new int[10];
}

LettleVM::~LettleVM() {
}