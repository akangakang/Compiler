#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "table.h"
#include "tree.h"
#include "frame.h"

/*Lab5: Your implementation here.*/

const int F_wordSize = 8;
//varibales
struct F_access_
{
	enum
	{
		inFrame,
		inReg
	} kind;
	union
	{
		int offset;	   //inFrame
		Temp_temp reg; //inReg
	} u;
};
struct F_frame_
{
	Temp_label name;
	F_accessList formals;
	int local_count;
};



//--------------------------construct-----------------------------//
static F_access F_Access_InFrame(int offset)
{
	F_access acc = checked_malloc(sizeof(*acc));
	acc->kind = inFrame;
	acc->u.offset = offset;
	return acc;
}

F_accessList F_AccessList(F_access head, F_accessList tail)
{
	F_accessList a = checked_malloc(sizeof(*a));
	a->head = head;
	a->tail = tail;
	return a;
}

static F_access F_Access_InReg(Temp_temp reg)
{
	F_access acc = checked_malloc(sizeof(*acc));
	acc->kind = inReg;
	acc->u.reg = reg;
	return acc;
}

F_frag F_StringFrag(Temp_label label, string str)
{
	F_frag string_frag = checked_malloc(sizeof(*string_frag));
	string_frag->kind = F_stringFrag;
	string_frag->u.stringg.label = label;
	string_frag->u.stringg.str = str;
	return string_frag;
}

F_frag F_ProcFrag(T_stm body, F_frame frame)
{
	F_frag proc_frag = checked_malloc(sizeof(*proc_frag));
	proc_frag->kind = F_procFrag;
	proc_frag->u.proc.body = body;
	proc_frag->u.proc.frame = frame;
	return proc_frag;
	return NULL;
}

F_fragList F_FragList(F_frag head, F_fragList tail)
{
	F_fragList f = checked_malloc(sizeof(*f));
	f->head = head;
	f->tail = tail;
	return f;
	
}

Temp_temp F_FP(void) {
	return Temp_newtemp();
}

Temp_temp F_RV(void)
{
	//TODO should always return the same one.
  return Temp_newtemp();
}

static F_access InFrame(int offset)
{
  F_access acc = checked_malloc(sizeof(*acc));
  acc->kind = inFrame;
  acc->u.offset = offset;
  return acc;
}

static F_access InReg(Temp_temp reg)
{
  F_access acc = checked_malloc(sizeof(*acc));
  acc->kind = inReg;
  acc->u.reg = reg;
  return acc;
}

T_exp F_Exp(F_access access, T_exp fp) {
	if (access->kind == inFrame) {
		return T_Mem(T_Binop(T_plus, fp, T_Const(access->u.offset)));
	} else {
		return T_Temp(access->u.reg);
	}
}
//-----------------------------------------------------------------//
// 在栈帧中分配一个心的局部变量
F_access F_allocLocal(F_frame f, bool escape)
{
	if (escape)
	{
		f->local_count++; //TODO
		return InFrame(F_wordSize * (-f->local_count));
	}
	else
	{
		return InReg(Temp_newtemp());
	}
}

// 返栈帧对应函数的名字
Temp_label F_name(F_frame f)
{
	return f->name;
}

// 返回栈帧对应形参的access（名字加位置）
F_accessList F_formals(F_frame f)
{
	return f->formals;
}

// 调用外部函数
T_exp F_externalCall(string s, T_expList args) {
	return T_Call(T_Name(Temp_namedlabel(s)), args);
}    

// 在栈帧中分配一个变量
F_access F_var(F_frame f, bool escape)
{
  if(escape){
		f->local_count++; //TODO
    return InFrame(F_wordSize * (- f->local_count));
  }else{
    return InReg(Temp_newtemp());
  }
}


static F_accessList makeFormalAccessList(F_frame f, U_boolList formals)
{
  F_accessList alist = NULL, rlist = NULL;
	//reserve space for return addreess
  int offset =  F_wordSize;

  // 简单点 先全都放在栈上
  for(; formals; formals = formals->tail, offset += F_wordSize){
    alist = F_AccessList(InFrame(offset), alist);
  }

  for(; alist; alist = alist->tail){
    rlist = F_AccessList(alist->head, rlist);
  }

  return rlist;
}

F_frame F_newFrame(Temp_label name, U_boolList formals)
{
  F_frame f = checked_malloc(sizeof(*f));
  f->name = name;
  f->formals = makeFormalAccessList(f, formals);
  f->local_count = 0;
  return f;
}

T_stm F_procEntryExit1(F_frame frame, T_stm stm)
{
  return stm;
}