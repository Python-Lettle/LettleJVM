/**
 * @file main.cpp
 * @author Lettle (1071445082@qq.com)
 * @version 0.1
 * @date 2024-07-27
 * @copyright Copyright (c) 2024
 * 
 * @brief Lettle Java VM execution engine implementation
 */

#include <lvm.h>
#include <lvm_util.h>
#include <string.h>

using namespace std;
using namespace lvm;

ExecEngine::ExecEngine() {}

ExecEngine::ExecEngine(Constant* now_class) 
{
    this->now_class = now_class;
}

uint32_t ExecEngine::execute_instruction(DataArea& vmdata, instruction_info instruction)
{
    DEBUG_LOG("%s", get_instruction_name(instruction.opcode).c_str());
    if(instruction.hav_operand){
        DEBUG_LOG(" #%d\n", instruction.operand);
    } else {
        DEBUG_LOG("\n");
    }

    method_info * method;
    Constant constant, constant2;
    switch (instruction.opcode)
    {
    case NOP: 
        break;
    case POP: 
        vmdata.native_stack.pop();
        break;
    case POP2: 
        vmdata.native_stack.pop(); 
        vmdata.native_stack.pop();
        break;
    case LDC:
        vmdata.native_stack.push(instruction.operand);
        break;
    case ALOAD_0:
        vmdata.native_stack.push(this->local_vars[0]);
        break;
    case GETSTATIC:
        vmdata.native_stack.push(instruction.operand);
        break;
    case INVOKEVIRTUAL:
        constant = vmdata.constant_pool[instruction.operand];
        constant2 = vmdata.constant_pool[constant.name_and_type_index];
        constant = vmdata.constant_pool[constant2.index];
        if (!strcmp((const char *)constant.utf8_const, "println")) {
            cout << constant2.utf8_const << endl;
        } else {
        }
        break;
    case INVOKESPECIAL: 
        execute_method(vmdata, vmdata.method_area[instruction.operand-1]); 
        break;
    case INVOKESTATIC:
        method = vmdata.method_area[instruction.operand-1];
        // if(DEBUG) cout << "goto function " << (int)(instruction.operand-1) << endl;
        vmdata.jvm_stack.push(vmdata.pc);
        vmdata.pc = 0;
        execute_method(vmdata, method);
        vmdata.pc = vmdata.jvm_stack.top(); vmdata.jvm_stack.pop();
        break;
    case RETURN: break;
    default:break;
    }

    return 0;
}
void ExecEngine::execute_method(DataArea& vmdata, method_info* method)
{
    this->operand_stack.resize(method->max_stack);
    operand_stack_top = 0;
    this->local_vars.resize(method->max_locals);
    while(vmdata.pc < method->instructions.size()) {
        instruction_info instruction = method->instructions[vmdata.pc];
        vmdata.pc++;
        execute_instruction(vmdata, instruction);
    }
}

ExecEngine::~ExecEngine() {}