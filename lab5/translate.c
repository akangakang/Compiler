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

//LAB5: you can modify anything you want.
static Tr_level outermost = NULL;
static F_fragList frags;
struct Tr_access_
{
	Tr_level level;
	F_access access;
};

// struct Tr_accessList_
// {
// 	Tr_access head;
// 	Tr_accessList tail;
// };

struct Tr_level_
{
	F_frame frame;
	Tr_level parent;
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

static Tr_accessList makeTrAccessList(F_accessList list, Tr_level level) {
    if (list == NULL) {
        return NULL;
    }

    Tr_access tr_access = checked_malloc(sizeof(*tr_access));
    tr_access->level = level;
    tr_access->access = list->head;

    return Tr_AccessList(tr_access, makeTrAccessList(list->tail, level));

}
struct Tr_expList_
{
	Tr_exp head;
	Tr_expList tail;
};

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

//------------------------constructure-------------------------//

Tr_expList Tr_ExpList(Tr_exp head, Tr_expList tail)
{
	Tr_expList exps = checked_malloc(sizeof(*exps));
	exps->head = head;
	exps->tail = tail;
	return exps;
}

Tr_access Tr_Access(Tr_level level, F_access access)
{
	Tr_access trAccess = checked_malloc(sizeof(*trAccess));
	trAccess->level = level;
	trAccess->access = access;
	return trAccess;
}

Tr_accessList Tr_AccessList(Tr_access head, Tr_accessList tail)
{
	Tr_accessList trAccesses = checked_malloc(sizeof(*trAccesses));
	trAccesses->head = head;
	trAccesses->tail = tail;
	return trAccesses;
}

Tr_level Tr_outermost(void)
{
	if (outermost)
	{
		return outermost;
	}
	else
	{
		outermost = checked_malloc(sizeof(*outermost));
		outermost->parent = NULL;
		outermost->frame = F_newFrame(Temp_newlabel(), NULL);
		return outermost;
	}
}

// 构造一个新的Tr_level
Tr_level Tr_newLevel(Tr_level parent, Temp_label name, U_boolList formals)
{
	Tr_level level = checked_malloc(sizeof(*level));
	level->parent = parent;
	level->frame = F_newFrame(name, U_BoolList(TRUE, formals));
	return level;
}

// 把Tr_level 里面的静态链去掉 变成
Tr_accessList Tr_formals(Tr_level level)
{
	F_accessList f_access_list = F_formals(level->frame)->tail; // skip static link
	return makeTrAccessList(f_access_list, level);
}

static Tr_exp Tr_Ex(T_exp ex)
{
	Tr_exp trEx = checked_malloc(sizeof(*trEx));
	trEx->kind = Tr_ex;
	trEx->u.ex = ex;
	return trEx;
}

static Tr_exp Tr_Nx(T_stm nx)
{
	Tr_exp trNx = checked_malloc(sizeof(*trNx));
	trNx->kind = Tr_nx;
	trNx->u.nx = nx;
	return trNx;
}

static Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm)
{
	Tr_exp trCx = checked_malloc(sizeof(*trCx));
	trCx->kind = Tr_cx;
	trCx->u.cx.trues = trues;
	trCx->u.cx.falses = falses;
	trCx->u.cx.stm = stm;
	return trCx;
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

static struct Cx unCx(Tr_exp e)
{
	switch (e->kind)
	{
	case Tr_cx:
		return e->u.cx;

	case Tr_ex:
	{

		struct Cx trCx;
		// 如果Tr_ex的值不为0则跳转到true
		trCx.stm = T_Cjump(T_ne, unEx(e), T_Const(0), NULL, NULL);
		trCx.trues = PatchList(&(trCx.stm->u.CJUMP.true), NULL);
		trCx.falses = PatchList(&(trCx.stm->u.CJUMP.false), NULL);
		return trCx;
	}

	case Tr_nx:
	{
		assert(0);
	}
	}

	assert(0);
}

static T_stm unNx(Tr_exp e)
{
	switch (e->kind)
	{
	case Tr_nx:
		return e->u.nx;

	case Tr_ex:
		return T_Exp(e->u.ex); // 计算e但忽略结果

	case Tr_cx:
	{
		Temp_label empty = Temp_newlabel();
		doPatch(e->u.cx.trues, empty);
		doPatch(e->u.cx.falses, empty);
		return T_Seq(e->u.cx.stm, T_Label(empty));
	}
	}

	assert(0);
}

static T_exp staticLink(Tr_level level, Tr_level target)
{
	T_exp ex = T_Temp(F_FP());
	Tr_level nowLevel = level;
	while (nowLevel && nowLevel != target)
	{
		ex = F_Exp(F_formals(nowLevel->frame)->head, ex);
		nowLevel = nowLevel->parent;
	}
	return ex;
}

//-----------------------------------------------------------------------//
Tr_exp Tr_nil()
{
	return Tr_Ex(T_Const(0));
}

Tr_exp Tr_int(int i)
{
	return Tr_Ex(T_Const(i));
}

//--------------------------------var------------------------------------//
Tr_exp Tr_simpleVar(Tr_access access, Tr_level level)
{
	T_exp fp = staticLink(level, access->level);
	T_exp ex = F_Exp(access->access, fp);
	return Tr_Ex(ex);
}

Tr_exp Tr_subscriptVar(Tr_exp addr, Tr_exp off)
{
	T_exp ex = T_Mem(
		T_Binop(
			T_plus,
			unEx(addr),
			T_Binop(T_mul, unEx(off), T_Const(F_wordSize))));

	return Tr_Ex(ex);
}

Tr_exp Tr_fieldVar(Tr_exp addr, int n)
{
	T_exp ex = T_Mem(
		T_Binop(T_plus, unEx(addr), T_Const(n * F_wordSize)));

	return Tr_Ex(ex);
}

//------------------------------array-record----------------------------//
//要知道有几个fields
static T_stm createFields(Temp_temp r, int n, Tr_expList fields)
{
	if (fields)
	{
		return T_Seq(
			T_Move(
				T_Mem(
					T_Binop(T_plus, T_Temp(r), T_Const(n * F_wordSize))),
				unEx(fields->head)),
			createFields(r, n - 1, fields->tail));
	}
	else
	{
		return T_Exp(T_Const(0));
	}
}

Tr_exp Tr_record(int n, Tr_expList fields)
{
	Temp_temp r = Temp_newtemp();
	T_exp ex = T_Eseq(
		T_Move(
			T_Temp(r),
			F_externalCall("malloc", T_ExpList(T_Const(n * F_wordSize), NULL))),
		T_Eseq(
			createFields(r, n - 1, fields),
			T_Temp(r)));

	return Tr_Ex(ex);
}

Tr_exp Tr_array(Tr_exp size, Tr_exp init)
{
	Temp_temp r;
	Temp_temp n = Temp_newtemp(), i = Temp_newtemp();

	T_exp ex = T_Eseq(
		T_Move(
			T_Temp(n),
			unEx(size)),
		T_Eseq(
			T_Move(
				T_Temp(i),
				unEx(init)),
			T_Eseq(
				T_Move(
					T_Temp(r),
					F_externalCall("initArray",
								   T_ExpList(T_Binop(T_mul, T_Temp(n), T_Const(F_wordSize)),
											 T_ExpList(T_Temp(i), NULL)))),
				T_Temp(r))));

	return Tr_Ex(ex);
}

//----------------------------------0p-----------------------------------//
Tr_exp Tr_opA(A_oper oper, Tr_exp left, Tr_exp right)
{
	T_exp l = unEx(left), r = unEx(right), ex;
	switch (oper)
	{
	case A_plusOp:
	{
		ex = T_Binop(T_plus, l, r);
		break;
	}

	case A_minusOp:
	{
		ex = T_Binop(T_minus, l, r);
		break;
	}

	case A_timesOp:
	{
		ex = T_Binop(T_mul, l, r);
		break;
	}

	case A_divideOp:
	{
		ex = T_Binop(T_div, l, r);
		break;
	}
	}
	return Tr_Ex(ex);
}

Tr_exp Tr_opL(A_oper oper, Tr_exp left, Tr_exp right, bool isStrCmp)
{
	T_exp l, r;
	struct Cx cx;

	if (isStrCmp)
	{
		l = F_externalCall("stringEqual", T_ExpList(unEx(left), T_ExpList(unEx(right), NULL)));
		r = T_Const(0);
	}
	else
	{
		l = unEx(left);
		r = unEx(right);
	}

	switch (oper)
	{
	case A_eqOp:
	{
		cx.stm = T_Cjump(T_eq, l, r, NULL, NULL);
		break;
	}

	case A_neqOp:
	{
		cx.stm = T_Cjump(T_ne, l, r, NULL, NULL);
		break;
	}

	case A_ltOp:
	{
		cx.stm = T_Cjump(T_lt, l, r, NULL, NULL);
		break;
	}

	case A_gtOp:
	{
		cx.stm = T_Cjump(T_gt, l, r, NULL, NULL);
		break;
	}

	case A_leOp:
	{
		cx.stm = T_Cjump(T_le, l, r, NULL, NULL);
		break;
	}

	case A_geOp:
	{
		cx.stm = T_Cjump(T_ge, l, r, NULL, NULL);
		break;
	}
	}

	cx.trues = PatchList(&(cx.stm->u.CJUMP.true), NULL);
	cx.falses = PatchList(&(cx.stm->u.CJUMP.false), NULL);
	return Tr_Cx(cx.trues, cx.falses, cx.stm);
}

//----------------------------------if------------------------------------//
Tr_exp Tr_ifThen(Tr_exp test, Tr_exp then)
{
	struct Cx cx = unCx(test);
	Temp_label t = Temp_newlabel(), f = Temp_newlabel();
	doPatch(cx.trues, t);
	doPatch(cx.falses, f);

	T_stm nx = T_Seq(
		cx.stm,
		T_Seq(
			T_Label(t),
			T_Seq(
				unNx(then),
				T_Label(f))));
	return Tr_Nx(nx);
}

Tr_exp Tr_ifThenElse(Tr_exp test, Tr_exp then, Tr_exp elsee)
{

	struct Cx cx = unCx(test);
	Temp_temp r;
	Temp_label t = Temp_newlabel(), f = Temp_newlabel(), done = Temp_newlabel();
	doPatch(cx.trues, t);
	doPatch(cx.falses, f);

	T_exp ex = T_Eseq(
		cx.stm,
		T_Eseq(
			T_Label(t),
			T_Eseq(
				T_Move(T_Temp(r), unEx(then)),
				T_Eseq(
					T_Jump(T_Name(done), Temp_LabelList(done, NULL)),
					T_Eseq(
						T_Label(f),
						T_Eseq(
							T_Move(T_Temp(r), unEx(elsee)),
							T_Eseq(
								T_Jump(T_Name(done), Temp_LabelList(done, NULL)),
								T_Eseq(
									T_Label(done),
									T_Temp(r)))))))));
	return Tr_Ex(ex);
}

//-------------------------------while-----for-----------------------------//
Tr_exp Tr_while(Tr_exp test, Tr_exp body, Temp_label done)
{
	struct Cx cx = unCx(test);
	Temp_label t = Temp_newlabel(), b = Temp_newlabel();
	doPatch(cx.trues, b);
	doPatch(cx.falses, done);

	T_stm nx = T_Seq(
		T_Label(t),
		T_Seq(
			cx.stm,
			T_Seq(
				T_Label(b),
				T_Seq(
					unNx(body),
					T_Seq(
						T_Jump(T_Name(t), Temp_LabelList(t, NULL)),
						T_Label(done))))));

	return Tr_Nx(nx);
}

Tr_exp Tr_for(Tr_access access, Tr_level level, Tr_exp lo, Tr_exp hi, Tr_exp body, Temp_label done)
{
	Tr_exp i = Tr_simpleVar(access, level);
	Temp_temp limit = Temp_newtemp();
	Temp_label b = Temp_newlabel(), inc = Temp_newlabel();

	T_stm nx = T_Seq(
		T_Move(unEx(i), unEx(lo)),
		T_Seq(
			T_Move(T_Temp(limit), unEx(hi)),
			T_Seq(
				T_Cjump(T_gt, unEx(i), T_Temp(limit), done, b),
				T_Seq(
					T_Label(b),
					T_Seq(
						unNx(body),
						T_Seq(
							T_Cjump(T_eq, unEx(i), T_Temp(limit), done, inc),
							T_Seq(
								T_Label(inc),
								T_Seq(
									T_Move(unEx(i), T_Binop(T_plus, unEx(i), T_Const(1))),
									T_Seq(
										T_Jump(T_Name(b), Temp_LabelList(b, NULL)),
										T_Label(done))))))))));

	return Tr_Nx(nx);
}

Tr_exp Tr_break(Temp_label done)
{
	T_stm nx = T_Jump(T_Name(done), Temp_LabelList(done, NULL));
	return Tr_Nx(nx);
}

//----------------------------------call----------------------------------//
T_expList makeCallParams(Tr_expList params)
{
	T_expList exps = NULL;
	Tr_expList trExps = params;
	while (trExps)
	{
		exps = T_ExpList(unEx(trExps->head), exps);
		trExps = trExps->tail;
	}
	return exps;
}
Tr_exp Tr_call(Tr_level callee, Temp_label label, Tr_expList params, Tr_level caller)
{
	T_exp ex = T_Call(T_Name(label),
					  T_ExpList(staticLink(caller, callee->parent), makeCallParams(params)));
	return Tr_Ex(ex);
}

//----------------------------dec----------------------------------------//
Tr_access Tr_allocLocal(Tr_level level, bool escape)
{
	return Tr_Access(level, F_allocLocal(level->frame, escape));
}

//----------------------------------frag-----------------------------------//
F_fragList Tr_getResult(void)
{
	return frags;
}

/* Add data frag  */
Tr_exp Tr_string(string str)
{
	Temp_label label = Temp_newlabel();
	F_frag head = F_StringFrag(label, str);
	frags = F_FragList(head, frags);
	return Tr_Ex(T_Name(label));
}

void Tr_func(Tr_exp body, Tr_level level)
{
	T_stm stm = Tr_procEntryExit(level, body, Tr_formals(level));
	F_frag head = F_ProcFrag(stm, level->frame);
	frags = F_FragList(head, frags);
}

// 把函数体的结果放进返回值
T_stm Tr_procEntryExit(Tr_level level, Tr_exp body, Tr_accessList formals)
{
	return T_Move(T_Temp(F_RV()), unEx(body));
}

//-----------------------------------------------------------//
Tr_exp Tr_seq(Tr_exp e1, Tr_exp e2)
{
	T_exp ex = T_Eseq(unNx(e1), unEx(e2));
	return Tr_Ex(ex);
}

Tr_exp Tr_assign(Tr_exp var, Tr_exp exp)
{
	T_stm nx = T_Move(unEx(var), unEx(exp));
	return Tr_Nx(nx);
}


//-------------------------------------------------------------//
