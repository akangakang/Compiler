#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "temp.h"
#include "errormsg.h"
#include "tree.h"
#include "assem.h"
#include "frame.h"
#include "codegen.h"
#include "table.h"
#include "string.h"

/*
    instruction selection
*/

// last指想最后一个 每次新加都加到最后一个
static AS_instrList iList = NULL, last = NULL;
static void emit(AS_instr inst)
{
    if (last != NULL)
        last = last->tail = AS_InstrList(inst, NULL);
    else
        last = iList = AS_InstrList(inst, NULL);
}

static Temp_temp munchExp(T_exp e);
static void munchStm(T_stm s);

// 把所有实在参数传递到正确位置
static Temp_tempList munchArgs(int i, T_expList args, int *stack_size)
{
    if (!args)
        return NULL;
    // 重复处理下一个参数
    // 先把后面的参数压栈
    Temp_tempList recur = munchArgs(i + 1, args->tail, stack_size);
    Temp_temp cur = munchExp(args->head);
    Temp_temp para = NULL;
    if (i >= F_formalRegNum)
    {
        emit(AS_Oper("\tpush `s0", Temp_TempList(F_SP(), NULL), Temp_TempList(cur, Temp_TempList(F_SP(), NULL)), NULL));
        *stack_size += F_wordSize;
        // 如果要压栈 则增加栈的大小
    }
    else
    {
        Temp_tempList paramReg = F_paramReg();
        for (int j = 0; j < i; j++)
        {
            paramReg = paramReg->tail;
        }

        // 用第i个参数寄存器
        para = paramReg->head;
        emit(AS_Move("\tmovq `s0, `d0", Temp_TempList(para, NULL), Temp_TempList(cur, NULL)));
    }
    if (para)
    {
        return Temp_TempList(para, recur);
    }
    else
    {
        return recur;
    }
}

static Temp_temp munchExp(T_exp e)
{
    char buf[100];
    switch (e->kind)
    {
    case T_BINOP:
    {
        Temp_temp r = Temp_newtemp();
        Temp_temp left = munchExp(e->u.BINOP.left);
        Temp_temp right = munchExp(e->u.BINOP.right);

        // D <- D op s 里
        switch (e->u.BINOP.op)
        {
        case T_plus:
        {
            emit(AS_Move("\tmovq `s0, `d0", Temp_TempList(r, NULL), Temp_TempList(left, NULL)));
            emit(AS_Oper("\taddq `s0, `d0", Temp_TempList(r, NULL), Temp_TempList(right, Temp_TempList(r, NULL)), NULL));
            return r;
        }
        case T_minus:
        {
            emit(AS_Move("\tmovq `s0, `d0", Temp_TempList(r, NULL), Temp_TempList(left, NULL)));
            emit(AS_Oper("\tsubq `s0, `d0", Temp_TempList(r, NULL), Temp_TempList(right, Temp_TempList(r, NULL)), NULL));
            return r;
        }
        case T_mul:
        {
            emit(AS_Move("\tmovq `s0, `d0", Temp_TempList(r, NULL), Temp_TempList(left, NULL)));
            emit(AS_Oper("\timulq `s0, `d0", Temp_TempList(r, NULL), Temp_TempList(right, Temp_TempList(r, NULL)), NULL));
            return r;
        }
        case T_div:
        {
            /*
            除法把%rdx(高64位)和%rax（低64位）作为被除数
            除数由指令给出 s0
            把商存在%rax里  余数存在%rdx里

            eax <- quotient edx <- remainder 
            movl left, %rax
    	    cqto #use rax's sign bit to fill rdx
    	    idivl right #now, q in rax and remainder in rdx

            left是dst
            */
           emit(AS_Move("movq `s0, `d0", Temp_TempList(F_RAX(), NULL), Temp_TempList(left, NULL)));
            emit(AS_Oper("cqto", Temp_TempList(F_RAX(), Temp_TempList(F_RDX(), NULL)), Temp_TempList(F_RAX(), NULL), NULL));
            emit(AS_Oper("idivq `s0", Temp_TempList(F_RAX(), Temp_TempList(F_RDX(), NULL)), Temp_TempList(right, Temp_TempList(F_RAX(), Temp_TempList(F_RDX(), NULL))), NULL));
            emit(AS_Move("movq `s0, `d0", Temp_TempList(r, NULL),Temp_TempList(F_RAX(), NULL)));
            
            return r;
        }
        default:
            assert(0);
        }
    }
    case T_MEM:
    {
        Temp_temp r = Temp_newtemp();
        T_exp mem = e->u.MEM;
        if (mem->kind == T_CONST)
        {
            char a[100];
            sprintf(a, "\tmovq %d, `d0", mem->u.CONST);
            emit(AS_Oper(String(a), Temp_TempList(r, NULL), NULL, NULL));
            return r;
        }
        else
        {
            Temp_temp s = munchExp(mem);
            emit(AS_Oper("\tmovq (`s0), `d0", Temp_TempList(r, NULL), Temp_TempList(s, NULL), NULL));
            return r;
        }
    }
    case T_TEMP:
    {
        return e->u.TEMP;
    }
    case T_ESEQ:
    {
        munchStm(e->u.ESEQ.stm);
        return munchExp(e->u.ESEQ.exp);
    }
    case T_NAME:
    {
        Temp_temp r = Temp_newtemp();
        char a[100];
        sprintf(a, "\tleaq .%s(%%rip), `d0", Temp_labelstring(e->u.NAME));
        emit(AS_Oper(String(a), Temp_TempList(r, NULL), NULL, NULL));
        return r;
    }
    case T_CONST:
    {
        Temp_temp r = Temp_newtemp();
        char a[100];
        sprintf(a, "\tmovq $%d, `d0", e->u.CONST);
        emit(AS_Oper(String(a), Temp_TempList(r, NULL), NULL, NULL));
        return r;
    }
    case T_CALL:
    {
        Temp_temp r = F_RV();
        int stack_size = 0;
        char a[100];
        // 处理参数
        Temp_tempList l = munchArgs(0, e->u.CALL.args, &stack_size);
        sprintf(a, "\tcall %s", Temp_labelstring(e->u.CALL.fun->u.NAME));
        emit(AS_Oper(String(a), Temp_TempList(F_RV(), F_callerSavedReg()), l, NULL));

        // 因为有参数要放在栈上传，所以栈增长了  相应的改变rsp
        if (stack_size > 0)
        {
            char b[100];
            sprintf(b, "\tadd $%d, `d0", stack_size);
            emit(AS_Oper(String(b), Temp_TempList(F_SP(), NULL), NULL, NULL));
        }
        return r;
    }
    }
}

void munchStm(T_stm s)
{

    switch (s->kind)
    {

    case T_MOVE:
    {
        T_exp dst = s->u.MOVE.dst, src = s->u.MOVE.src;
        Temp_temp t = munchExp(src);
        if (dst->kind == T_MEM)
        {
            T_exp mem = dst->u.MEM;
            if (mem->kind == T_BINOP && mem->u.BINOP.op == T_plus && mem->u.BINOP.right->kind == T_CONST)
            {

                Temp_temp b = munchExp(mem->u.BINOP.left);
                char a[100];
                sprintf(a, "\tmovq `s0, %d(`s1)", mem->u.BINOP.right->u.CONST);
                emit(AS_Oper(String(a), NULL, Temp_TempList(t, Temp_TempList(b, NULL)), NULL));
                break;
            }
            Temp_temp d = munchExp(mem);
            emit(AS_Oper("\tmovq `s0, (`s1)", NULL, Temp_TempList(t, Temp_TempList(d, NULL)), NULL));
            break;
        }
        else if (dst->kind == T_TEMP)
        {
            Temp_temp d = munchExp(dst);
            emit(AS_Move("\tmovq `s0, `d0", Temp_TempList(d, NULL), Temp_TempList(t, NULL)));
            break;
        }
        break;
    }
    case T_JUMP:
    {
        T_exp e = s->u.JUMP.exp;
        if (e->kind == T_NAME)
        {
            emit(AS_Oper("\tjmp .`j0", NULL, NULL, AS_Targets(s->u.JUMP.jumps)));
        }
        else
        {
            assert(0);
        }
        break;
    }
    case T_CJUMP:
    {
        Temp_temp left = munchExp(s->u.CJUMP.left);
        Temp_temp right = munchExp(s->u.CJUMP.right);
        emit(AS_Oper("\tcmp `s1, `s0", NULL, Temp_TempList(left, Temp_TempList(right, NULL)), NULL));
        char *op;
        switch (s->u.CJUMP.op)
        {
        case T_eq:
        {
            op = "je";
            break;
        }
        case T_ne:
        {
            op = "jne";
            break;
        }
        case T_lt:
        {
            op = "jl";
            break;
        }
        case T_gt:
        {
            op = "jg";
            break;
        }
        case T_le:
        {
            op = "jle";
            break;
        }
        case T_ge:
        {
            op = "jge";
            break;
        }
        }
        char a[100];

        sprintf(a, "\t%s .`j0", op);
        emit(AS_Oper(String(a), NULL, NULL, AS_Targets(Temp_LabelList(s->u.CJUMP.true, NULL))));
        break;
    }
    case T_EXP:
    {
        munchExp(s->u.EXP);
        break;
    }
    case T_SEQ:
    {
        munchStm(s->u.SEQ.left);
        munchStm(s->u.SEQ.right);
        break;
    }
    case T_LABEL:
    {
        char a[100];
        sprintf(a, ".%s", Temp_labelstring(s->u.LABEL));
        emit(AS_Label(String(a), s->u.LABEL));
        break;
    }

    default:
        assert(0);
    }
}

AS_instrList F_codegen(F_frame f, T_stmList stmList)
{
    AS_instrList list;

    for (T_stmList sl = stmList; sl; sl = sl->tail)
    {
        munchStm(sl->head);
    }

    list = iList;
    iList = last = NULL;

    return F_procEntryExit2(list);
}
