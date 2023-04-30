#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

using std::cout;
using std::endl;

using u1 = std::uint8_t;
using u2 = std::uint16_t;
using u4 = std::uint32_t;

// Define the JVM's stack
#define STACK_SIZE 100
std::vector<int> stack(STACK_SIZE);
int sp = -1; // Stack pointer

std::vector<int> operand_stack(100);
int top = 0;

// Define the method area
#define METHOD_AREA_SIZE 10
std::vector<u1*> method_area(METHOD_AREA_SIZE);
int num_methods = 0;

// Define the constant pool
#define CONST_POOL_SIZE 10
std::vector<u1*> const_pool(CONST_POOL_SIZE);
int num_constants = 0;

// Define the runtime constant pool
#define RUNTIME_CONST_POOL_SIZE 10
std::vector<int> runtime_const_pool(RUNTIME_CONST_POOL_SIZE);
int num_runtime_constants = 0;

// Load a class file into memory
u1* load_class_file(const char* filename) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        exit(1);
    }
    infile.seekg(0, std::ios::end);
    int size = infile.tellg();
    u1* data = new u1[size];
    infile.seekg(0, std::ios::beg);
    infile.read(reinterpret_cast<char*>(data), size);
    infile.close();
    return data;
}

// Parse a u1 value from the bytecode and update the program counter
u1 read_u1(u1* bytecode, int& pc) {
    return bytecode[pc++];
}

// Parse a u2 value from the bytecode and update the program counter
u2 read_u2(u1* bytecode, int& pc) {
    u2 value = (bytecode[pc++] << 8) | bytecode[pc++];
    return value;
}

// Parse a u4 value from the bytecode and update the program counter
u4 read_u4(u1* bytecode, int& pc) {
    u4 value = (bytecode[pc++] << 24) | (bytecode[pc++] << 16) | (bytecode[pc++] << 8) | bytecode[pc++];
    return value;
}

#include <iostream>
#include <vector>

// Execute the main method
void execute_main(u1* bytecode, int& pc) {
    std::vector<int> results;
    while (pc < num_methods) {
        u1 opcode = read_u1(bytecode, pc);
        int a,b;
        u1* method;
        u1 index;
        u2 method_index;
        switch (opcode) {
            case 0x12: // ldc
                index = read_u1(bytecode, pc);
                runtime_const_pool[++num_runtime_constants] = index;
                operand_stack[++top] = runtime_const_pool[index];
                break;
            case 0x60: // iadd
                b = operand_stack[top--];
                a = operand_stack[top--];
                operand_stack[++top] = a + b;
                break;
            case 0x64: // isub
                b = operand_stack[top--];
                a = operand_stack[top--];
                operand_stack[++top] = a - b;
                break;
            case 0xb6: // invokevirtual
                method_index = read_u2(bytecode, pc);
                method = method_area[method_index];
                execute_main(method, pc);
                break;
            case 0xb1: // return
                // Print the results
                std::cout << "The results are: ";
                for (int i = 0; i <= top; i++)
                    results.push_back(operand_stack[i]);
                for (int i = 0; i < results.size(); i++)
                    std::cout << results[i] << " ";
                
                std::cout << std::endl;
                return;
            default: // Handle unknown opcodes
                break;
        }
    }
}



int main() {
    // Load the bytecode
    u1* bytecode = load_class_file("HelloWorld.class");

    // Load the main method
    method_area[++num_methods] = bytecode;

    // Execute the main method
    int pc = 0;
    execute_main(bytecode, pc);

    // Print the output
    cout << "Hello, World!" << endl;

    // Clean up
    delete[] bytecode;

    return 0;
}
