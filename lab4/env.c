#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "env.h"

/*Lab4: Your implementation of lab4*/

E_enventry E_VarEntry(Ty_ty ty)
{
	E_enventry e = checked_malloc(sizeof(*e));
	e->kind = E_varEntry;
	e->u.var.ty=ty;
	return e;
}
E_enventry E_ROVarEntry(Ty_ty ty)
{
	E_enventry entry = checked_malloc(sizeof(*entry));
    entry->kind = E_varEntry;
	entry->u.var.ty = ty;
	entry->readonly = 1;
	return entry;
}
E_enventry E_FunEntry(Ty_tyList formals, Ty_ty result)
{
	E_enventry e = checked_malloc(sizeof(*e));
	e->kind = E_funEntry;
	e->u.fun.formals = formals;
	e->u.fun.result = result;
	return e;
}

S_table E_base_tenv(void)
{
	// int -> Ty_int ; string -> Ty_string
	S_table t = S_empty();
	S_enter(t,S_Symbol("int"),Ty_Int());
	S_enter(t,S_Symbol("string"),Ty_String());
	return t;
}

S_table E_base_venv(void)
{
	// 预定义的函数
	S_table v = S_empty();
	return v;
}
