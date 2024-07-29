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

// 该函数返回goto控制的下一个指令索引
int ExecEngine::execute_instruction(DataArea& vmdata, instruction_info instruction)
{
    DEBUG_LOG("%s", get_instruction_name(instruction.opcode).c_str());
    if(instruction.hav_operand){
        DEBUG_LOG(" %d\n", instruction.operand);
    } else {
        DEBUG_LOG("\n");
    }

    method_info * method;
    Constant constant, constant2;
    stack_frame * frame;
    int num1, num2;
    uint32_t ref_index, method_info_index;
    switch (instruction.opcode)
    {
    case NOP: 
        break;
    case POP: 
        this->operand_stack.pop();
        break;
    case POP2: 
        this->operand_stack.pop(); 
        this->operand_stack.pop();
        break;
    case ICONST_M1:
        frame = new stack_frame{FRAME_INT, -1};
        this->operand_stack.push(*frame);
        break;
    case LCONST_0:
        frame = new stack_frame{FRAME_LONG, 0};
        this->operand_stack.push(*frame);
        break;
    case ICONST_0:
        frame = new stack_frame{FRAME_INT, 0};
        this->operand_stack.push(*frame);
        break;
    case FCONST_0:
        frame = new stack_frame{FRAME_FLOAT, 0};
        this->operand_stack.push(*frame);
        break;
    case DCONST_0:
        frame = new stack_frame{FRAME_DOUBLE, 0};
        this->operand_stack.push(*frame);
        break;
    case LCONST_1:
        frame = new stack_frame{FRAME_LONG, 1};
        this->operand_stack.push(*frame);
        break;
    case ICONST_1:
        frame = new stack_frame{FRAME_INT, 1};
        this->operand_stack.push(*frame);
        break;
    case FCONST_1:
        frame = new stack_frame{FRAME_FLOAT, 1};
        this->operand_stack.push(*frame);
        break;
    case DCONST_1:
        frame = new stack_frame{FRAME_DOUBLE, 1};
        this->operand_stack.push(*frame);
        break;
    case ICONST_2:
        frame = new stack_frame{FRAME_INT, 2};
        this->operand_stack.push(*frame);
        break;
    case FCONST_2:
        frame = new stack_frame{FRAME_FLOAT, 2};
        this->operand_stack.push(*frame);
        break;
    case ICONST_3:
        frame = new stack_frame{FRAME_INT, 3};
        this->operand_stack.push(*frame);
        break;
    case ICONST_4:
        frame = new stack_frame{FRAME_INT, 4};
        this->operand_stack.push(*frame);
        break;
    case ICONST_5:
        frame = new stack_frame{FRAME_INT, 5};
        this->operand_stack.push(*frame);
        break;
    case BIPUSH:
    case SIPUSH:
        frame = new stack_frame{FRAME_INT, instruction.operand};
        this->operand_stack.push(*frame);
        break;
    case LDC:
        frame = new stack_frame{FRAME_CONSTANT, instruction.operand};
        this->operand_stack.push(*frame);
        break;
    case ALOAD_0:
        frame = new stack_frame{FRAME_REFERENCE, this->local_vars[0].value};
        this->operand_stack.push(*frame);
        break;
    case ILOAD_0:
        frame = new stack_frame{FRAME_INT, this->local_vars[0].value};
        this->operand_stack.push(*frame);
        break;
    case ILOAD_1:
        frame = new stack_frame{FRAME_INT, this->local_vars[1].value};
        this->operand_stack.push(*frame);
        break;
    case ILOAD_2:
        frame = new stack_frame{FRAME_INT, this->local_vars[2].value};
        this->operand_stack.push(*frame);
        break;
    case ILOAD_3:
        frame = new stack_frame{FRAME_INT, this->local_vars[3].value};
        this->operand_stack.push(*frame);
        break;
    case LLOAD_0:
        frame = new stack_frame{FRAME_LONG, this->local_vars[0].value};
        this->operand_stack.push(*frame);
        break;
    case LLOAD_1:
        frame = new stack_frame{FRAME_LONG, this->local_vars[1].value};
        this->operand_stack.push(*frame);
        break;
    case LLOAD_2:
        frame = new stack_frame{FRAME_LONG, this->local_vars[2].value};
        this->operand_stack.push(*frame);
        break;
    case LLOAD_3:
        frame = new stack_frame{FRAME_LONG, this->local_vars[3].value};
        this->operand_stack.push(*frame);
        break;
    case FLOAD_0:
        frame = new stack_frame{FRAME_FLOAT, this->local_vars[0].value};
        this->operand_stack.push(*frame);
        break;
    case FLOAD_1:
        frame = new stack_frame{FRAME_FLOAT, this->local_vars[1].value};
        this->operand_stack.push(*frame);
        break;
    case FLOAD_2:
        frame = new stack_frame{FRAME_FLOAT, this->local_vars[2].value};
        this->operand_stack.push(*frame);
        break;
    case FLOAD_3:
        frame = new stack_frame{FRAME_FLOAT, this->local_vars[3].value};
        this->operand_stack.push(*frame);
        break;
    case DLOAD_0:
        frame = new stack_frame{FRAME_DOUBLE, this->local_vars[0].value};
        this->operand_stack.push(*frame);
        break;
    case DLOAD_1:
        frame = new stack_frame{FRAME_DOUBLE, this->local_vars[1].value};
        this->operand_stack.push(*frame);
        break;
    case DLOAD_2:
        frame = new stack_frame{FRAME_DOUBLE, this->local_vars[2].value};
        this->operand_stack.push(*frame);
        break;
    case DLOAD_3:
        frame = new stack_frame{FRAME_DOUBLE, this->local_vars[3].value};
        this->operand_stack.push(*frame);
        break;

    case ISTORE_0: 
        this->local_vars[0].type = this->operand_stack.top().type;
        this->local_vars[0].value = this->operand_stack.top().value;
        this->operand_stack.pop();
        break;
    case ISTORE_1:
        this->local_vars[1].type = this->operand_stack.top().type;
        this->local_vars[1].value = this->operand_stack.top().value;
        this->operand_stack.pop();
        break;
    case ISTORE_2:
        this->local_vars[2].type = this->operand_stack.top().type;
        this->local_vars[2].value = this->operand_stack.top().value;
        this->operand_stack.pop();
        break;
    case ISTORE_3:
        this->local_vars[3].type = this->operand_stack.top().type;
        this->local_vars[3].value = this->operand_stack.top().value;
        this->operand_stack.pop();
        break;

    case IADD:
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        num2 = this->operand_stack.top().value; this->operand_stack.pop();
        frame = new stack_frame{FRAME_INT, num1 + num2};
        this->operand_stack.push(*frame);
        break;
    case ISUB:
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        num2 = this->operand_stack.top().value; this->operand_stack.pop();
        frame = new stack_frame{FRAME_INT, num2 - num1};
        this->operand_stack.push(*frame);
        break;
    case IMUL:
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        num2 = this->operand_stack.top().value; this->operand_stack.pop();
        frame = new stack_frame{FRAME_INT, num1 * num2};
        this->operand_stack.push(*frame);
        break;
    case IDIV:
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        num2 = this->operand_stack.top().value; this->operand_stack.pop();
        frame = new stack_frame{FRAME_INT, num2 / num1};
        this->operand_stack.push(*frame);
        break;
    case IREM:
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        num2 = this->operand_stack.top().value; this->operand_stack.pop();
        frame = new stack_frame{FRAME_INT, num2 % num1};
        this->operand_stack.push(*frame);
        break;

    case IINC:
        this->local_vars[instruction.operand].value += instruction.operand2;
        break;

    case IFEQ:
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        if (num1 == 0) return instruction.operand; else return -1;
    case IFNE:
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        if (num1 != 0) return instruction.operand; else return -1;
    case IFLT:
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        if (num1 < 0) return instruction.operand; else return -1;
    case IFGE:
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        if (num1 >= 0) return instruction.operand; else return -1;
    case IFGT:
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        if (num1 > 0) return instruction.operand; else return -1;
    case IFLE:
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        if (num1 <= 0) return instruction.operand; else return -1;

    case IF_ICMPEQ:
        num2 = this->operand_stack.top().value; this->operand_stack.pop();
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        if (num1 = num2)
            return instruction.operand;
        else
            return -1;
    case IF_ICMPNE:
        num2 = this->operand_stack.top().value; this->operand_stack.pop();
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        if (num1 != num2)
            return instruction.operand;
        else
            return -1;
    case IF_ICMPLT:
        num2 = this->operand_stack.top().value; this->operand_stack.pop();
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        if (num1 < num2)
            return instruction.operand;
        else
            return -1;
    case IF_ICMPGE:
        num2 = this->operand_stack.top().value; this->operand_stack.pop();
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        if (num1 >= num2)
            return instruction.operand;
        else
            return -1;
    case IF_ICMPGT:
        num2 = this->operand_stack.top().value; this->operand_stack.pop();
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        if (num1 > num2)
            return instruction.operand;
        else
            return -1;
    case IF_ICMPLE:
        num2 = this->operand_stack.top().value; this->operand_stack.pop();
        num1 = this->operand_stack.top().value; this->operand_stack.pop();
        if (num1 <= num2)
            return instruction.operand;
        else
            return -1;

    case GOTO:
        return instruction.operand;

    case GETSTATIC:
        frame = new stack_frame{FRAME_STATIC, instruction.operand};
        this->operand_stack.push(*frame);
        break;
    case INVOKEVIRTUAL:
        // constant_index = this->operand_stack.top().value;
        ref_index = this->operand_stack.top().value; this->operand_stack.pop();
        method_info_index = this->operand_stack.top().value; this->operand_stack.pop();
        constant = vmdata.constant_pool[instruction.operand];
        constant2 = vmdata.constant_pool[constant.name_and_type_index];
        constant = vmdata.constant_pool[constant2.index];
        if (!strcmp((const char *)constant.utf8_const, "println")) {
            DEBUG_LOG("(System.out.println) ");
            num1 = this->operand_stack.top().value; this->operand_stack.pop();
            constant = vmdata.constant_pool[num1];
            constant2 = vmdata.constant_pool[constant.index];
            cout << constant2.utf8_const << endl;
        } else {
        }
        break;
    case INVOKESPECIAL: 
        execute_method(vmdata, vmdata.method_area[instruction.operand-1]); 
        break;
    case INVOKESTATIC:
        method = vmdata.method_area[instruction.operand-1];
        vmdata.jvm_stack.push(vmdata.pc);
        vmdata.pc = 0;
        execute_method(vmdata, method);
        vmdata.pc = vmdata.jvm_stack.top(); vmdata.jvm_stack.pop();
        break;
    case RETURN: 
        if(DEBUG) print_stack(this->operand_stack);
        break;
    default:break;
    }

    return -1;
}
void ExecEngine::execute_method(DataArea& vmdata, method_info* method)
{
    this->local_vars.resize(method->max_locals);
    while(vmdata.pc <= method->max_line_number) {
        auto it = method->instructions.find(vmdata.pc);
        if (it != method->instructions.end()) {
            instruction_info instruction = method->instructions[vmdata.pc];
            if(DEBUG) print_stack(this->operand_stack);
            DEBUG_LOG("%d ", vmdata.pc);
            int goto_linenum = execute_instruction(vmdata, instruction);
            if (goto_linenum != -1) {
                vmdata.pc += goto_linenum - 1;
            }
        }
        vmdata.pc++;
    }
}

ExecEngine::~ExecEngine() {}