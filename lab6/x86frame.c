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

//varibales
const int F_wordSize = 8;
const int F_formalRegNum = 6;
const int F_regNum = 16;

F_frag F_StringFrag(Temp_label label, string str)
{
	F_frag f = checked_malloc(sizeof(*f));
	f->kind = F_stringFrag;
	f->u.stringg.label = label;
	f->u.stringg.str = str;
	return f;
	
}

F_frag F_ProcFrag(T_stm body, F_frame frame)
{
	F_frag f = checked_malloc(sizeof(*f));
	f->kind = F_procFrag;
	f->u.proc.body = body;
	f->u.proc.frame = frame;
	return f;
}

F_fragList F_FragList(F_frag head, F_fragList tail)
{
	F_fragList fl = checked_malloc(sizeof(*fl));
	fl->head = head;
	fl->tail = tail;
	return fl;
}

static F_access InFrame(int offset)
{
	F_access a = checked_malloc(sizeof(*a));

	a->kind = inFrame;
	a->u.offset = offset;

	return a;
}

static F_access InReg(Temp_temp reg)
{
	F_access a = checked_malloc(sizeof(*a));

	a->kind = inReg;
	a->u.reg = reg;

	return a;
}

F_accessList F_AccessList(F_access head, F_accessList tail)
{
	F_accessList a = checked_malloc(sizeof(*a));

	a->head = head;
	a->tail = tail;
	return a;
}

Temp_label F_name(F_frame f)
{
	return f->name;
}

F_accessList F_formals(F_frame f)
{
	return f->formals;
}

F_access F_allocLocal(F_frame f, bool escape)
{
	if (escape)
	{
		f->size += F_wordSize;
		return InFrame(-1 * f->size);
	}
	else
	{
		return InReg(Temp_newtemp());
	}
}

// 把一个F_access 转化成一个tree
T_exp F_Exp(F_access acc, T_exp framePtr)
{
	switch (acc->kind)
	{
	case inFrame:
		return T_Mem(T_Binop(T_plus, framePtr, T_Const(acc->u.offset)));

	case inReg:
		return T_Temp(acc->u.reg);
	}
	assert(0);
}

static T_stm makeShiftStm(F_frame f)
{
	// 参数寄存器
	Temp_tempList tl = F_paramReg();
	T_stm stm = NULL;
	F_accessList fmls = f->formals;
	for (int i = 0; fmls; fmls = fmls->tail, i++)
	{
		T_stm s = NULL;
		T_exp dst = F_Exp(fmls->head, T_Temp(F_FP()));
		T_exp src = NULL;

		if (i < F_formalRegNum)
		{
			src = T_Temp(tl->head);
		}
		else
		{
			// spare 1 word for return address and 1 word for rbp
			src = T_Mem(T_Binop(T_plus, T_Temp(F_FP()), T_Const((i - F_formalRegNum + 2) * F_wordSize)));
		}
		s = T_Move(dst, src);
		if (stm == NULL)
		{
			stm = s;
		}
		else
		{
			stm = T_Seq(stm, s);
		}
		if (tl)
			tl = tl->tail;
	}

	return stm;
}

F_frame F_newFrame(Temp_label name, U_boolList formals)
{
	F_frame f = checked_malloc(sizeof(*f));
	f->name = name;
	f->size = 0;

	U_boolList escapes = formals;
	F_accessList head = NULL;
	F_accessList tail = NULL;
	for (; escapes; escapes = escapes->tail)
	{
		F_access a = F_allocLocal(f, escapes->head);
		if (head == NULL)
		{
			head = tail = F_AccessList(a, NULL);
		}
		else
		{
			tail->tail = F_AccessList(a, NULL);
			tail = tail->tail;
		}
	}
	f->formals = head;

	f->shift = makeShiftStm(f);
	return f;
}

T_exp F_externalCall(string s, T_expList args)
{
	return T_Call(T_Name(Temp_namedlabel(s)), args);
}

//----------------------------------------寄存器相关-------------------------------------//

static Temp_temp rax = NULL;
Temp_temp F_RAX(void)
{
	if (rax == NULL)
	{
		rax = Temp_newtemp();
	}
	return rax;
}

static Temp_temp rbx = NULL;
Temp_temp F_RBX(void)
{
	if (rbx == NULL)
	{
		rbx = Temp_newtemp();
	}
	return rbx;
}

static Temp_temp rcx = NULL;
Temp_temp F_RCX(void)
{
	if (rcx == NULL)
	{
		rcx = Temp_newtemp();
	}
	return rcx;
}

static Temp_temp rdx = NULL;
Temp_temp F_RDX(void)
{
	if (rdx == NULL)
	{
		rdx = Temp_newtemp();
	}
	return rdx;
}

static Temp_temp rsi = NULL;
Temp_temp F_RSI(void)
{
	if (rsi == NULL)
	{
		rsi = Temp_newtemp();
	}
	return rsi;
}

static Temp_temp rdi = NULL;
Temp_temp F_RDI(void)
{
	if (rdi == NULL)
	{
		rdi = Temp_newtemp();
	}
	return rdi;
}

static Temp_temp rsp = NULL;
Temp_temp F_RSP(void)
{
	if (rsp == NULL)
	{
		rsp = Temp_newtemp();
	}
	return rsp;
}

static Temp_temp rbp = NULL;
Temp_temp F_RBP(void)
{
	if (rbp == NULL)
	{
		rbp = Temp_newtemp();
	}
	return rbp;
}

static Temp_temp r8 = NULL;
Temp_temp F_R8(void)
{
	if (r8 == NULL)
	{
		r8 = Temp_newtemp();
	}
	return r8;
}

static Temp_temp r9 = NULL;
Temp_temp F_R9(void)
{
	if (r9 == NULL)
	{
		r9 = Temp_newtemp();
	}
	return r9;
}

static Temp_temp r10 = NULL;
Temp_temp F_R10(void)
{
	if (r10 == NULL)
	{
		r10 = Temp_newtemp();
	}
	return r10;
}

static Temp_temp r11 = NULL;
Temp_temp F_R11(void)
{
	if (r11 == NULL)
	{
		r11 = Temp_newtemp();
	}
	return r11;
}

static Temp_temp r12 = NULL;
Temp_temp F_R12(void)
{
	if (r12 == NULL)
	{
		r12 = Temp_newtemp();
	}
	return r12;
}

static Temp_temp r13 = NULL;
Temp_temp F_R13(void)
{
	if (r13 == NULL)
	{
		r13 = Temp_newtemp();
	}
	return r13;
}

static Temp_temp r14 = NULL;
Temp_temp F_R14(void)
{
	if (r14 == NULL)
	{
		r14 = Temp_newtemp();
	}
	return r14;
}

static Temp_temp r15 = NULL;
Temp_temp F_R15(void)
{
	if (r15 == NULL)
	{
		r15 = Temp_newtemp();
	}
	return r15;
}
Temp_temp F_FP(void)
{
	//TODO should always return the same one.
	// return Temp_newtemp();
	return F_RBP();
}

Temp_temp F_SP(void)
{
	return F_RSP();
}
Temp_temp F_RV(void)
{
	//TODO should always return the same one.
	// return Temp_newtemp();
	return F_RAX();
}

static Temp_tempList allReg = NULL;
Temp_tempList F_registers()
{
	if (!allReg)
		allReg = Temp_TempList(F_RAX(),
							   Temp_TempList(F_RBX(),
											 Temp_TempList(F_RCX(),
														   Temp_TempList(F_RDX(),
																		 Temp_TempList(F_RSI(),
																					   Temp_TempList(F_RDI(),
																									 Temp_TempList(F_RBP(),
																												   Temp_TempList(F_RSP(),
																																 Temp_TempList(F_R8(),
																																			   Temp_TempList(F_R9(),
																																							 Temp_TempList(F_R10(),
																																										   Temp_TempList(F_R11(),
																																														 Temp_TempList(F_R12(),
																																																	   Temp_TempList(F_R13(),
																																																					 Temp_TempList(F_R14(),
																																																								   Temp_TempList(F_R15(),
																																																												 NULL))))))))))))))));
	return allReg;
}

static Temp_tempList calleeSavedReg = NULL;
Temp_tempList F_calleeSavedReg()
{
	if (!calleeSavedReg)
		calleeSavedReg = Temp_TempList(F_RBX(),
									   Temp_TempList(F_RBP(),
													 Temp_TempList(F_R12(),
																   Temp_TempList(F_R13(),
																				 Temp_TempList(F_R14(),
																							   Temp_TempList(F_R15(),
																											 NULL))))));
	return calleeSavedReg;
}

static Temp_tempList callerSavedReg = NULL;
Temp_tempList F_callerSavedReg()
{
	if (!callerSavedReg)
		callerSavedReg = Temp_TempList(F_RAX(),
									   Temp_TempList(F_RCX(),
													 Temp_TempList(F_RDX(),
																   Temp_TempList(F_RDI(),
																				 Temp_TempList(F_RSI(),
																							   Temp_TempList(F_R8(),
																											 Temp_TempList(F_R9(),
																														   Temp_TempList(F_R10(),
																																		 Temp_TempList(F_R11(),
																																					   NULL)))))))));
	return callerSavedReg;
}

static Temp_tempList paramReg = NULL;
Temp_tempList F_paramReg()
{
	if (!paramReg)
		paramReg = Temp_TempList(F_RDI(),
								 Temp_TempList(F_RSI(),
											   Temp_TempList(F_RDX(),
															 Temp_TempList(F_RCX(),
																		   Temp_TempList(F_R8(),
																						 Temp_TempList(F_R9(),
																									   NULL))))));
	return paramReg;
}

static Temp_map map = NULL;
Temp_map F_regTempMap(void)
{
	if (!map)
	{
		map = Temp_empty();
		Temp_enter(map, F_RAX(), "\%rax");
		Temp_enter(map, F_RBX(), "\%rbx");
		Temp_enter(map, F_RCX(), "\%rcx");
		Temp_enter(map, F_RDX(), "\%rdx");
		Temp_enter(map, F_RBP(), "\%rbp");
		Temp_enter(map, F_RSP(), "\%rsp");
		Temp_enter(map, F_RDI(), "\%rdi");
		Temp_enter(map, F_RSI(), "\%rsi");
		Temp_enter(map, F_R8(), "\%r8");
		Temp_enter(map, F_R9(), "\%r9");
		Temp_enter(map, F_R10(), "\%r10");
		Temp_enter(map, F_R11(), "\%r11");
		Temp_enter(map, F_R12(), "\%r12");
		Temp_enter(map, F_R13(), "\%r13");
		Temp_enter(map, F_R14(), "\%r14");
		Temp_enter(map, F_R15(), "\%r15");
	}
	return map;
}

// 将每一个传入的寄存器参数存放到从函数内来看它的位置
T_stm F_procEntryExit1(F_frame frame, T_stm stm)
{
	T_stm shift = frame->shift;

	Temp_tempList temps = F_calleeSavedReg();
	T_stm save = NULL;
	T_stm restore = NULL;

	// 保护calleesave
	for (; temps; temps = temps->tail)
	{
		T_stm s = T_Move(T_Temp(Temp_newtemp()), T_Temp(temps->head));

		// 恢复的时候 反过来
		T_stm r = T_Move(s->u.MOVE.src, s->u.MOVE.dst);

		// 接在最後面
		if (save == NULL)
		{
			save = s;
		}
		else
		{
			save = T_Seq(save, s);
		}
		if (restore == NULL)
		{
			restore = r;
		}
		else
		{
			restore = T_Seq(restore, r);
		}
	}

	// 保存calleesave
	// shiftview
	// 回复calleesave
	return T_Seq(save, T_Seq(shift, T_Seq(stm, restore)));
}

static Temp_tempList returnSink = NULL;
// 告诉寄存器分配起 函数出口这些寄存器returnSink是活跃的
AS_instrList F_procEntryExit2(AS_instrList body)
{
	if (!returnSink)
		returnSink = Temp_TempList(F_SP(), Temp_TempList(F_FP(), F_calleeSavedReg()));
	return AS_splice(body, AS_InstrList(AS_Oper("", NULL, returnSink, NULL), NULL));
}

AS_proc F_procEntryExit3(F_frame frame, AS_instrList body)
{
	char prolog[1024];
	string procName = S_name(F_name(frame));
	sprintf(prolog, "\t.text\n");
	sprintf(prolog, "%s\t.globl %s\n", prolog, procName);
	sprintf(prolog, "%s\t.type %s, @function\n", prolog, procName);
	sprintf(prolog, "%s%s:\n", prolog, procName);

	// 每调用一次函数都要先保存上一个函数的帧指针
	// 把上一个函数的栈指针作为这个函数的帧指针
	// 把栈指针减去这个帧栈的大小

	sprintf(prolog, "%s\tpushq %%rbp\n", prolog);
	sprintf(prolog, "%s\tmovq %%rsp, %%rbp\n", prolog);
	sprintf(prolog, "%s\tsubq $%d, %%rsp\n", prolog, frame->size);

	char epilog[256];
	sprintf(epilog, "\tleave\n");
	sprintf(epilog, "%s\tret\n\n", epilog);

	return AS_Proc(String(prolog), body, String(epilog));
}

static Temp_labelList predefine = NULL;
Temp_labelList F_preDefineFuncs()
{
	if (!predefine)
	{
		predefine = Temp_LabelList(Temp_namedlabel("flush"),
								   Temp_LabelList(Temp_namedlabel("exit"),
												  Temp_LabelList(Temp_namedlabel("not"),
																 Temp_LabelList(Temp_namedlabel("chr"),
																				Temp_LabelList(Temp_namedlabel("getchar"),
																							   Temp_LabelList(Temp_namedlabel("print"),
																											  Temp_LabelList(Temp_namedlabel("ord"),
																															 Temp_LabelList(Temp_namedlabel("size"),
																																			Temp_LabelList(Temp_namedlabel("concat"),
																																						   Temp_LabelList(Temp_namedlabel("substring"),
																																										  Temp_LabelList(Temp_namedlabel("printi"),
																																														 NULL)))))))))));
	}
	return predefine;
}
