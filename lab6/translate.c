#include <stdio.h>
#include "util.h"
#include "table.h"
#include "symbol.h"
#include "absyn.h"
#include "temp.h"
#include "tree.h"
#include "printtree.h"
#include "frame.h"
#include "translate.h"
#include "string.h"

//LAB5: you can modify anything you want.

struct Tr_access_
{
	Tr_level level;
	F_access access;
};

struct Tr_level_
{
	F_frame frame;
	Tr_level parent;
	// Tr_accessList formals;
};

typedef struct patchList_ *patchList;
struct patchList_
{
	Temp_label *head;
	patchList tail;
};

struct Cx
{
	patchList trues;
	patchList falses;
	T_stm stm;
};

struct Tr_exp_
{
	enum
	{
		Tr_ex,
		Tr_nx,
		Tr_cx
	} kind;
	union
	{
		T_exp ex;
		T_stm nx;
		struct Cx cx;
	} u;
};

//global variable

// 记录所有的frag
static F_fragList fragList = NULL;
static Tr_level outermost = NULL;

static patchList PatchList(Temp_label *head, patchList tail)
{
	patchList list;

	list = (patchList)checked_malloc(sizeof(struct patchList_));
	list->head = head;
	list->tail = tail;
	return list;
}

void doPatch(patchList tList, Temp_label label)
{
	for (; tList; tList = tList->tail)
		*(tList->head) = label;
}

patchList joinPatch(patchList first, patchList second)
{
	if (!first)
		return second;
	for (; first->tail; first = first->tail)
		;
	first->tail = second;
	return first;
}

static Tr_exp Tr_Ex(T_exp ex)
{
	Tr_exp res = checked_malloc(sizeof(*res));
	res->kind = Tr_ex;
	res->u.ex = ex;
	return res;
}

static Tr_exp Tr_Nx(T_stm nx)
{
	Tr_exp res = checked_malloc(sizeof(*res));
	res->kind = Tr_nx;
	res->u.nx = nx;
	return res;
}

static Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm)
{
	Tr_exp res = checked_malloc(sizeof(*res));
	res->kind = Tr_cx;
	res->u.cx.trues = trues;
	res->u.cx.falses = falses;
	res->u.cx.stm = stm;
	return res;
}

//------------------------Tr_exp unconstructure-------------------------//
static T_exp unEx(Tr_exp e)
{
	switch (e->kind)
	{
	case Tr_ex:
		return e->u.ex;
	case Tr_cx:
	{
		Temp_temp r = Temp_newtemp();
		Temp_label t = Temp_newlabel(), f = Temp_newlabel();
		doPatch(e->u.cx.trues, t);
		doPatch(e->u.cx.falses, f);
		return T_Eseq(
			T_Move(
				T_Temp(r),
				T_Const(1)),
			T_Eseq(
				e->u.cx.stm,
				T_Eseq(
					T_Label(f),
					T_Eseq(
						T_Move(
							T_Temp(r),
							T_Const(0)),
						T_Eseq(
							T_Label(t),
							T_Temp(r))))));
	}
	case Tr_nx:
		return T_Eseq(e->u.nx, T_Const(0));
	}
	assert(0);
}

static T_stm unNx(Tr_exp e)
{
	switch (e->kind)
	{
	case Tr_ex:
		return T_Exp(e->u.ex);
	case Tr_cx:
	{
		Temp_label l = Temp_newlabel();
		doPatch(e->u.cx.trues, l);
		doPatch(e->u.cx.falses, l);
		return T_Seq(e->u.cx.stm, T_Label(l));
	}
	case Tr_nx:
		return e->u.nx;
	}
	assert(0);
}

static struct Cx unCx(Tr_exp e)
{
	switch (e->kind)
	{
	case Tr_ex:
	{
		struct Cx cx;
		// 如果Tr_ex的值不为0则跳转到true
		cx.stm = T_Cjump(T_ne, e->u.ex, T_Const(0), NULL, NULL);
		patchList trues = PatchList(&cx.stm->u.CJUMP.true, NULL);
		patchList falses = PatchList(&cx.stm->u.CJUMP.false, NULL);
		cx.trues = trues;
		cx.falses = falses;
		return cx;
	}
	case Tr_cx:
		return e->u.cx;
	case Tr_nx:
		assert(0);
	}
	assert(0);
}


//---------------------------access---------------------------------------//
static Tr_access Tr_Access(Tr_level level, F_access access)
{
	Tr_access traccess = checked_malloc(sizeof(*traccess));
	traccess->level = level;
	traccess->access = access;
	return traccess;
}


Tr_accessList Tr_AccessList(Tr_access head, Tr_accessList tail)
{
	Tr_accessList a = checked_malloc(sizeof(*a));
	a->head = head;
	a->tail = tail;
	return a;
}

Tr_expList Tr_ExpList(Tr_exp head, Tr_expList tail)
{
	Tr_expList e = checked_malloc(sizeof(*e));
	e->head = head;
	e->tail = tail;
	return e;
}

Tr_level Tr_outermost(void)
{
	if (!outermost)
	{
		outermost = checked_malloc(sizeof(*outermost));
		outermost->frame = F_newFrame(Temp_newlabel(), NULL);
		outermost->parent = NULL;
	}
	return outermost;
}

// 构造一个新的Tr_level  定义一个新的函数
Tr_level Tr_newLevel(Tr_level parent, Temp_label name, U_boolList formals)
{
	Tr_level l = checked_malloc(sizeof(*l));
	// 加上静态链
	l->frame = F_newFrame(name, U_BoolList(1, formals)); // Add static link
	l->parent = parent;
	return l;
}

Tr_accessList Tr_formals(Tr_level level)
{
	Tr_accessList alist = NULL, rlist = NULL;
	
	F_accessList accessList = F_formals(level->frame)->tail;

	// 跳过静态链
	for (; accessList; accessList = accessList->tail)
	{
		alist = Tr_AccessList(Tr_Access(level, accessList->head), alist);
	}

	// 反过来
	for (; alist; alist = alist->tail)
	{
		rlist = Tr_AccessList(alist->head, rlist);
	}

	return rlist;
}

Tr_access Tr_allocLocal(Tr_level level, bool escape)
{
	Tr_access a = checked_malloc(sizeof(*a));
	a->level = level;
	a->access = F_allocLocal(level->frame, escape);
	return a;
}

//--------------------------------var------------------------------------//
Tr_exp Tr_simpleVar(Tr_access access, Tr_level level)
{
	T_exp addr = T_Temp(F_FP());
	//following static link

	// 找到这个变量定义的帧栈
	for (; level != access->level; level = level->parent)
	{
		if (level == outermost)
		{
			break;
		}

		// 得到第一个形参 也就是静态链
		addr = F_Exp(F_formals(level->frame)->head, addr);
	}
	return Tr_Ex(F_Exp(access->access, addr));
}

Tr_exp Tr_fieldVar(Tr_exp base_addr, int index)
{
	return Tr_Ex(T_Mem(T_Binop(T_plus, unEx(base_addr), T_Binop(T_mul, T_Const(index), T_Const(F_wordSize)))));
}

Tr_exp Tr_subscriptVar(Tr_exp base_addr, Tr_exp index)
{
	return Tr_Ex(T_Mem(T_Binop(T_plus, unEx(base_addr), T_Binop(T_mul, unEx(index), T_Const(F_wordSize)))));
}

Tr_exp Tr_nilExp()
{
	return Tr_Ex(T_Const(0));
}

Tr_exp Tr_intExp(int value)
{
	return Tr_Ex(T_Const(value));
}

Tr_exp Tr_stringExp(string s)
{
	// 把stringfrag放到一起
	for (F_fragList fl = fragList; fl; fl = fl->tail)
	{
		F_frag f = fl->head;

		if (f->kind == F_stringFrag)
		{
			string str = f->u.stringg.str;
			int len1 = *(int *)str;
			int len2 = strlen(s);
			int len = len1 < len2 ? len1 : len2;

			// 如果找到了字符串片段 且字符串相同 则直接返回
			// 前四个字节是放字符串长度的
			if (strncmp(s, str + 4, len) == 0)
			{
				return Tr_Ex(T_Name(f->u.stringg.label));
			}
		}
	}

	// 如果没有找到相同的字符串 则要新建一个label
	Temp_label l = Temp_newlabel();
	int len = strlen(s);
	char *buf = checked_malloc(len + sizeof(int));
	(*(int *)buf) = len;
	strncpy(buf + 4, s, len);
	F_frag f = F_StringFrag(l, buf);
	fragList = F_FragList(f, fragList);
	return Tr_Ex(T_Name(l));
}

Tr_exp Tr_callExp(Tr_level level, Tr_level funLevel, Temp_label label, Tr_expList args)
{
	T_expList arglist = NULL;
	T_expList tail = NULL;

	// 处理参数
	for (; args; args = args->tail)
	{

		T_exp t = unEx(args->head);
		if (arglist == NULL)
		{
			arglist = tail = T_ExpList(t, NULL);
		}
		else
		{
			tail->tail = T_ExpList(t, NULL);
			tail = tail->tail;
		}
	}

	// EM_error(0, "Tr_callExp :111  \n");
	// debug ： 预先定义的函数！！！
	if (Temp_labelIn(F_preDefineFuncs(), label))
	{
		// EM_error(0, "Tr_callExp :end0  \n");
		return Tr_Ex(F_externalCall(Temp_labelstring(label), arglist));
	}

	// 找到公公祖先
	T_exp fp = T_Temp(F_FP());
	Tr_level l = level;
	while (l && l != funLevel->parent)
	{
		// EM_error(0, ".\n");
		F_access staticLink = F_formals(l->frame)->head;
		// EM_error(0, ".1\n");
		fp = F_Exp(staticLink, fp);
		// EM_error(0, ".2\n");
		l = l->parent;
		// EM_error(0, "3\n");
	}
	// EM_error(0, "Tr_callExp :end\n");
	return Tr_Ex(T_Call(T_Name(label), T_ExpList(fp, arglist)));
}

Tr_exp Tr_arithExp(A_oper oper, Tr_exp left, Tr_exp right)
{
	switch (oper)
	{
	case A_plusOp:
		return Tr_Ex(T_Binop(T_plus, unEx(left), unEx(right)));
	case A_minusOp:
		return Tr_Ex(T_Binop(T_minus, unEx(left), unEx(right)));
	case A_timesOp:
		return Tr_Ex(T_Binop(T_mul, unEx(left), unEx(right)));
	case A_divideOp:
		return Tr_Ex(T_Binop(T_div, unEx(left), unEx(right)));
	}
	assert(0);
}

Tr_exp Tr_relExp(A_oper oper, Tr_exp left, Tr_exp right)
{
	T_relOp op;
	switch (oper)
	{
	case A_eqOp:
		op = T_eq;
		break;
	case A_neqOp:
		op = T_ne;
		break;
	case A_ltOp:
		op = T_lt;
		break;
	case A_leOp:
		op = T_le;
		break;
	case A_gtOp:
		op = T_gt;
		break;
	case A_geOp:
		op = T_ge;
		break;
	default:
		assert(0);
	}
	T_stm s = T_Cjump(op, unEx(left), unEx(right), NULL, NULL);
	patchList trues = PatchList(&s->u.CJUMP.true, NULL);
	patchList falses = PatchList(&s->u.CJUMP.false, NULL);
	return Tr_Cx(trues, falses, s); // Will be unEx, when used
}

Tr_exp Tr_relStrExp(A_oper oper, Tr_exp left, Tr_exp right)
{
	T_exp res = F_externalCall("stringEqual", T_ExpList(unEx(left), T_ExpList(unEx(right), NULL)));
	switch (oper)
	{
	case A_eqOp:
	{
		return Tr_Ex(res); // Just return the result, 1 for equal, 0 for not equal
	}
	case A_neqOp:
	{
		T_stm s = T_Cjump(T_eq, res, T_Const(0), NULL, NULL);
		patchList trues = PatchList(&s->u.CJUMP.true, NULL);
		patchList falses = PatchList(&s->u.CJUMP.false, NULL);
		return Tr_Cx(trues, falses, s); // Will be unEx, when used
	}
	default:
		assert(0);
	}
	return Tr_Ex(res);
}

Tr_exp Tr_recordExp(int size, Tr_expList fields)
{
	//!!!!!
	Temp_temp r = Temp_newtemp();
	T_stm alloc = T_Move(T_Temp(r), F_externalCall("allocRecord", T_ExpList(T_Const(size * F_wordSize), NULL)));
	// debug : 发现这里初始化有问题
	T_stm init = T_Move(T_Mem(T_Binop(T_plus, T_Temp(r), T_Const(0))), unEx(fields->head));
	int i = 1;
	for (fields = fields->tail; fields; fields = fields->tail, i++)
	{
		init = T_Seq(init, T_Move(T_Mem(T_Binop(T_plus, T_Temp(r), T_Binop(T_mul, T_Const(i), T_Const(F_wordSize)))), unEx(fields->head)));
	}
	return Tr_Ex(T_Eseq(T_Seq(alloc, init), T_Temp(r)));
}

Tr_exp Tr_seqExp(Tr_expList seqs)
{
	// !!!
	T_exp res = unEx(seqs->head);
	for (seqs = seqs->tail; seqs; seqs = seqs->tail)
	{
		res = T_Eseq(unNx(Tr_Ex(res)), unEx(seqs->head));
	}
	return Tr_Ex(res);
}

Tr_exp Tr_assignExp(Tr_exp var, Tr_exp exp)
{
	return Tr_Nx(T_Move(unEx(var), unEx(exp)));
}

Tr_exp Tr_ifExp(Tr_exp test, Tr_exp then, Tr_exp elsee)
{
	if (elsee != NULL)
	{
		Temp_temp r = Temp_newtemp();
		Temp_label t = Temp_newlabel(), f = Temp_newlabel(), j = Temp_newlabel();
		struct Cx cx = unCx(test);
		doPatch(cx.trues, t);
		doPatch(cx.falses, f);
		return Tr_Ex(T_Eseq(cx.stm,
							T_Eseq(T_Label(t),
								   T_Eseq(T_Move(T_Temp(r), unEx(then)),
										  T_Eseq(T_Jump(T_Name(j), Temp_LabelList(j, NULL)),
												 T_Eseq(T_Label(f),
														T_Eseq(T_Move(T_Temp(r), unEx(elsee)),
															   T_Eseq(T_Jump(T_Name(j), Temp_LabelList(j, NULL)),
																	  T_Eseq(T_Label(j), T_Temp(r))))))))));
	}
	else
	{
		Temp_label t = Temp_newlabel(), f = Temp_newlabel();
		struct Cx cx = unCx(test);
		doPatch(cx.trues, t);
		doPatch(cx.falses, f);
		return Tr_Nx(T_Seq(cx.stm,
						   T_Seq(T_Label(t),
								 T_Seq(unNx(then), T_Label(f)))));
	}
}

Tr_exp Tr_whileExp(Tr_exp test, Tr_exp body, Temp_label done)
{
	Temp_label testl = Temp_newlabel(), t = Temp_newlabel();
	struct Cx cx = unCx(test);
	doPatch(cx.trues, t);
	doPatch(cx.falses, done);
	return Tr_Nx(T_Seq(T_Label(testl),
					   T_Seq(cx.stm,
							 T_Seq(T_Label(t),
								   T_Seq(unNx(body),
										 T_Seq(T_Jump(T_Name(testl), Temp_LabelList(testl, NULL)), T_Label(done)))))));
}

Tr_exp Tr_breakExp(Temp_label done)
{
	return Tr_Nx(T_Jump(T_Name(done), Temp_LabelList(done, NULL)));
}

Tr_exp Tr_forExp(Tr_access access, Tr_level level, Tr_exp lo, Tr_exp hi, Tr_exp body, Temp_label done)
{
	Tr_exp i = Tr_simpleVar(access, level);
	Temp_temp max = Temp_newtemp();
	Temp_label b = Temp_newlabel(), inc = Temp_newlabel();

	return Tr_Nx(T_Seq(T_Move(unEx(i), unEx(lo)),
					   T_Seq(T_Move(T_Temp(max), unEx(hi)),
							 T_Seq(T_Cjump(T_gt, unEx(i), T_Temp(max), done, b),
								   T_Seq(T_Label(b),
										 T_Seq(unNx(body),
											   T_Seq(T_Cjump(T_eq, unEx(i), T_Temp(max), done, inc),
													 T_Seq(T_Label(inc),
														   T_Seq(T_Move(unEx(i), T_Binop(T_plus, unEx(i), T_Const(1))),
																 T_Seq(T_Jump(T_Name(b), Temp_LabelList(b, NULL)), T_Label(done)))))))))));
}

Tr_exp Tr_arrayExp(Tr_exp size, Tr_exp init)
{
	return Tr_Ex(F_externalCall(String("initArray"),
								T_ExpList(unEx(size), T_ExpList(unEx(init), NULL))));
}

Tr_exp Tr_noExp()
{
	return Tr_Nx(T_Exp(T_Const(0)));
}


// 每次定义完一个函数就要调用这个
void Tr_procEntryExit(Tr_level level, Tr_exp body, Tr_accessList formals)
{
	Temp_temp t = Temp_newtemp();

	// 把返回值放到rax里
	// 并加上calleesave的保存和恢复
	T_stm stm = F_procEntryExit1(level->frame, T_Seq(T_Move(T_Temp(t), unEx(body)), T_Move(T_Temp(F_RV()), T_Temp(t))));
	F_frag f = F_ProcFrag(stm, level->frame);
	fragList = F_FragList(f, fragList);
}

F_fragList Tr_getResult(void)
{
	return fragList;
}


