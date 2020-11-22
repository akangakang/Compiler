#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "absyn.h"
#include "types.h"
#include "helper.h"
#include "env.h"
#include "semant.h"

/*Lab4: Your implementation of lab4*/

typedef void *Tr_exp;
struct expty
{
	Tr_exp exp;
	Ty_ty ty;
};

//In Lab4, the first argument exp should always be **NULL**.
struct expty expTy(Tr_exp exp, Ty_ty ty)
{
	struct expty e;

	e.exp = exp;
	e.ty = ty;

	return e;
}

Ty_ty actual_ty(Ty_ty ty)
{
	if (ty->kind == Ty_name)
	{
		return ty->u.name.ty;
	}
	else
	{
		return ty;
	}
}

Ty_tyList makeFormalTyList(S_table tenv, A_fieldList fieldList)
{
	A_fieldList f;
	Ty_tyList th = NULL;
	Ty_tyList tl = NULL;

	for (f = fieldList; f; f = f->tail)
	{
		Ty_ty t = S_look(tenv, f->head->typ);
		if (t == NULL)
		{
			EM_error(f->head->pos, "undefined type %s", f->head->typ);
			t = Ty_Int();
		}
		if (th == NULL)
		{
			th = tl = Ty_TyList(t, NULL);
		}
		else
		{
			tl->tail = Ty_TyList(t, NULL);
			tl = tl->tail;
		}
	}
	return th;
}

struct expty transExp(S_table venv, S_table tenv, A_exp a)
{
	switch (a->kind)
	{
	case A_varExp:
		return transVar(venv, tenv, a->u.var);

	case A_nilExp:
		return expTy(NULL, Ty_Nil());

	case A_intExp:
		return expTy(NULL, Ty_Int());

	case A_stringExp:
		return expTy(NULL, Ty_String());

	case A_callExp:
	{
		E_enventry func = S_look(venv, get_callexp_func(a));
		/*remember: E_enventry is a pointer */

		// check : if there is "func" in S_table
		if (! func)
		{
			EM_error(a->pos, "undefined function %s", S_name(get_callexp_func(a)));

			//why : renturn int?
			return expTy(NULL, Ty_Int());
		}

		// check : if it's function
		if (func->kind != E_funEntry)
		{
			EM_error(a->pos, "%s is not a function", S_name(get_callexp_func(a)));

			//why : renturn int?
			return expTy(NULL, Ty_Int());
		}

		// check : if formals and actuals match on type
		Ty_tyList formals = get_func_tylist(func);
		A_expList actuals = get_callexp_args(a);

		while (formals != NULL && actuals != NULL)
		{
			// get the type of actual
			struct expty e = transExp(venv, tenv, actuals->head);
			if (e.ty != formals->head)
			{
				EM_error(a->pos, "A_callExp : para type mismatch");
				return expTy(NULL, actual_ty(get_func_res(func)));
			}
			formals = formals->tail;
			actuals = actuals->tail;
		}

		// check : the number of para
		if (formals != NULL )
		{
			EM_error(a->pos, "too less params in function %s",S_name(get_callexp_func(a)));
			return expTy(NULL, actual_ty(get_func_res(func)));
		}

		if (actuals != NULL)
		{
			EM_error(a->pos, "too many params in function %s",S_name(get_callexp_func(a)));
			return expTy(NULL, actual_ty(get_func_res(func)));
		}

		return expTy(NULL, actual_ty(get_func_res(func)));
	}

	case A_opExp:
	{
		A_oper oper = get_opexp_oper(a);
		// EM_error(get_opexp_leftpos(a),"op %d",oper);
		struct expty left = transExp(venv, tenv, get_opexp_left(a));
		struct expty right = transExp(venv, tenv, get_opexp_right(a));

		// check : +-*/ left and right should be int
		if (oper == A_plusOp || oper == A_minusOp || oper == A_timesOp || oper == A_divideOp)
		{
			if (left.ty->kind != Ty_int)
			{
				EM_error(get_opexp_leftpos(a), "integer required");
			}
			if (right.ty->kind != Ty_int)
			{
				EM_error(get_opexp_rightpos(a), "integer required");
			}
			return expTy(NULL, Ty_Int());
		}

		// check : other op left and right should be same
		if (oper == A_eqOp || oper == A_neqOp || oper == A_ltOp || oper == A_leOp || oper == A_gtOp || oper == A_geOp)
		{
			// EM_error(get_opexp_leftpos(a), "s1ame type required ");
			if (left.ty->kind != right.ty->kind)
			{
				EM_error(get_opexp_leftpos(a), "same type required ");
			}
			return expTy(NULL, Ty_Int());
		}

		break;
	}

	case A_recordExp:
	{
		Ty_ty ty = S_look(tenv, get_recordexp_typ(a));
		
		// check :  if it's in tevn
		if (ty == NULL)
		{
			EM_error(a->pos, "undefined type %s", S_name(get_recordexp_typ(a)));
			return expTy(NULL, Ty_Record(NULL));
		}
ty=actual_ty(ty);
		// check : if it's record
		if (ty->kind != Ty_record)
		{
			EM_error(a->pos, "same type required %s", S_name(get_recordexp_typ(a)));
			return expTy(NULL, Ty_Record(NULL));
		}

		// check : filed
		Ty_fieldList expected = ty->u.record;
		A_efieldList actuals = get_recordexp_fields(a);

		while (actuals != NULL && expected != NULL)
		{
			// check : name
			if (expected->head->name != actuals->head->name)
			{
				EM_error(a->pos, "A_recordExp : expected %s but get %s", S_name(expected->head->name), S_name(actuals->head->name));
			}
			struct expty exp = transExp(venv, tenv, actuals->head->exp);
			// check : type
			if (expected->head->ty != exp.ty)
			{
				EM_error(a->pos, "A_recordExp : type not match");
			}

			actuals = actuals->tail;
			expected = expected->tail;
		}

		// check : number
		if (expected != NULL || actuals != NULL)
		{
			EM_error(a->pos, "A_recordExp : field number of %s does not match", S_name(a->u.record.typ));
		}
		return expTy(NULL, ty);
	}

	case A_seqExp:
	{

		// 返回最后一个表达式的返回类型
		A_expList seq = get_seqexp_seq(a);
		struct expty e;
		if (seq != NULL)
		{
			while (seq)
			{
				
				e=transExp(venv, tenv, seq->head);
				seq = seq->tail;
			}

			return e;
		}

		break;
		// TODO : if seq == NULL ?
	}

	case A_assignExp:
	{
		// check : if var type = exp type
		printf("assign");
		struct expty ee = transExp(venv, tenv, get_assexp_exp(a));
		struct expty ev = transVar(venv, tenv, get_assexp_var(a));
		if (ee.ty != ev.ty)
		{
			EM_error(a->pos, "A_assignExp : unmatched assign exp");
		}

		if (get_assexp_var(a)->kind == A_simpleVar)
		{
			E_enventry x = S_look(venv, get_simplevar_sym(get_assexp_var(a)));
			if (x && x->readonly)
			{
				EM_error(a->pos, "loop variable can't be assigned");
			}
		}
		
		return expTy(NULL, Ty_Void());

		// loop variables?
	}

	case A_ifExp:
	{
		// EM_error(a->u.iff.test->pos, "type of test expression shoulf be int");
		struct expty test = transExp(venv, tenv, a->u.iff.test);

		if (actual_ty(test.ty)->kind != Ty_int) {
				EM_error(a->u.iff.test->pos, "type of test expression shoulf be int");
			}
		struct expty then = transExp(venv, tenv, get_ifexp_then(a));

		// EM_error(a->u.iff.test->pos, "%s",);
		// 如果有else
		// 之所以写成A_nilExp 是因为tigher.y 里面
		// IF exp THEN exp  {$$ = A_IfExp(EM_tokPos, $2, $4, A_NilExp(EM_tokPos))
		if (get_ifexp_else(a)->kind != A_nilExp)
		{
			struct expty elsee = transExp(venv, tenv, get_ifexp_else(a));
			if (actual_ty(then.ty) != actual_ty(elsee.ty))
			{
				EM_error(a->u.iff.then->pos, "then exp and else exp type mismatch");
				return expTy(NULL, then.ty);
			}
		}
		else
		{
			if (then.ty->kind != Ty_void)
			{
				EM_error(a->u.iff.then->pos, "if-then exp's body must produce no value");
				return expTy(NULL, then.ty);
			}
		}
		return expTy(NULL, then.ty);
	}

	case A_whileExp:
	{
		struct expty test = transExp(venv, tenv, get_whileexp_test(a));
		struct expty body = transExp(venv, tenv, get_whileexp_body(a));

		// check :  test should be int
		if (get_expty_kind(test) != Ty_int)
		{
			EM_error(a->pos, "type of test expression should be int");
			return expTy(NULL, Ty_Void());
		}

		// check : body should be void
		if (get_expty_kind(body) != Ty_void)
		{
			EM_error(a->pos, "while body must produce no value");
			return expTy(NULL, Ty_Void());
		}

		return expTy(NULL, Ty_Void());
	}

	case A_forExp:
	{
		struct expty lo = transExp(venv, tenv, get_forexp_lo(a));
		struct expty hi = transExp(venv, tenv, get_forexp_hi(a));

		// check : lo, hi ->int
		if (get_expty_kind(lo) != Ty_int)
		{
			EM_error(get_forexp_lo(a)->pos, "for exp's range type is not integer");
		
		}
		if (get_expty_kind(hi) != Ty_int)
		{
			EM_error(get_forexp_hi(a)->pos, "for exp's range type is not integer");
			// not e: can't return 
		}

		// loop variable
		S_beginScope(venv);
		S_enter(venv, get_forexp_var(a), E_ROVarEntry(Ty_Int()));

		struct expty body = transExp(venv, tenv, get_forexp_body(a));
		
		if (get_expty_kind(body) != Ty_void)
		{
			EM_error(get_forexp_body(a)->pos, "type of body expression should be void");
		}
		S_endScope(venv);

		return expTy(NULL, Ty_Void());
	}

	case A_breakExp:
		return expTy(NULL, Ty_Void());
	case A_letExp:
	{
		struct expty e;
		A_decList d;
		S_beginScope(venv);
		S_beginScope(tenv);
		for (d = get_letexp_decs(a); d; d = d->tail)
		{
			transDec(venv, tenv, d->head);
		}
		e = transExp(venv, tenv, get_letexp_body(a));
		S_endScope(tenv);
		S_endScope(venv);
		return e;
	}
	case A_arrayExp:
	{
		Ty_ty t = S_look(tenv, get_arrayexp_typ(a));
		struct expty e = transExp(venv, tenv, get_arrayexp_init(a));
		if (e.ty != actual_ty(t)->u.array)
		{
			EM_error(a->pos, "type mismatch");
		}
		return expTy(NULL, actual_ty(t));
	}
	}
}

struct expty transVar(S_table venv, S_table tenv, A_var v)
{
	switch (v->kind)
	{
	case A_simpleVar:
	{
		E_enventry x = S_look(venv, v->u.simple);
		if (x && x->kind == E_varEntry)
		{
			return expTy(NULL, actual_ty(get_varentry_type(x)));
		}
		else
		{
			EM_error(v->pos, "A_simpleVar : undefined variable %s", S_name(get_simplevar_sym(v)));
			return expTy(NULL, Ty_Int());
		}
	}
	case A_fieldVar:
	{
		struct expty e = transVar(venv, tenv, get_fieldvar_var(v));

		// check : if it's record
		if (get_expty_kind(e) != Ty_record)
		{
			EM_error(get_fieldvar_var(v)->pos, "A_fieldVar : not a record type");
			return expTy(NULL, Ty_Int());
		}
		Ty_fieldList record = e.ty->u.record;

		// check : if record has this field
		while (record != NULL)
		{
			if (record->head->name == get_fieldvar_sym(v))
			{
				return expTy(NULL, actual_ty(record->head->ty));
			}
			record = record->tail;
		}
		EM_error(get_fieldvar_var(v)->pos, "A_fieldVar : field %s doesn't exist", S_name(get_fieldvar_sym(v)));
		return expTy(NULL, Ty_Int());
	}

	case A_subscriptVar:
	{

		// check : if exp is int
		struct expty exp = transExp(venv, tenv, get_subvar_exp(v));
		if (get_expty_kind(exp) != Ty_int)
		{
			EM_error(get_subvar_exp(v)->pos, "interger subscript required");
			return expTy(NULL, Ty_Int());
		}

		struct expty var = transVar(venv, tenv, get_subvar_var(v));
		if (get_expty_kind(var) != Ty_array)
		{
			EM_error(get_subvar_var(v)->pos, "array type required");
			return expTy(NULL, Ty_Int());
		}
		return expTy(NULL, actual_ty(get_array(var)));
	}
	}
}
Ty_fieldList makeFieldList(S_table tenv, A_fieldList fields) {
	Ty_ty type = S_look(tenv, fields->head->typ);
	if (!type) {
		EM_error(fields->head->pos, "undefined type %s", S_name(fields->head->typ));
		type = Ty_Int();
	}
	Ty_field field = Ty_Field(fields->head->name, type);

	if (fields->tail == NULL) {
		return Ty_FieldList(field, NULL);
	} else {
		return Ty_FieldList(field, makeFieldList(tenv, fields->tail));
	}
}
Ty_ty transTy(S_table tenv, A_ty a)
{
	switch (a->kind)
	{
	case A_nameTy:
	{
		Ty_ty t = S_look(tenv, get_ty_name(a));
		if (t != NULL)
		{
			EM_error(a->pos, "undefined type %s", S_name(get_ty_name(a)));
			return Ty_Name(a->u.name,t);
		}
	}

	case A_recordTy:
	{
		// A_fieldList f;
		// Ty_fieldList tf = NULL;
		// Ty_ty t;
		// for (f = get_ty_record(a); f; f = f->tail)
		// {
		// 	t = S_look(tenv, f->head->typ);
		// 	if (t == NULL)
		// 	{
		// 		EM_error(f->head->pos, "undefined type %s", S_name(f->head->typ));
		// 		t = Ty_Int();
		// 	}
		// 	tf = Ty_FieldList(Ty_Field(f->head->name, t), tf);
		// }
		// return Ty_Record(tf);
		return Ty_Record(makeFieldList(tenv, a->u.record));
	}
	case A_arrayTy:
	{
		Ty_ty t = S_look(tenv, get_ty_array(a));
		if (t != NULL)
		{
			return Ty_Array(t);
		}
		else
		{
			EM_error(a->pos, "undefined type %s", S_name(get_ty_array(a)));
			return Ty_Array(Ty_Int());
		}
	}
	}
}

void transDec(S_table venv, S_table tenv, A_dec d)
{
	switch (d->kind)
	{
	case A_functionDec:
	{
		A_fundecList function = get_funcdec_list(d);
		// int i=0;

		// 先把函数头都检查一遍
		while (function != NULL)
		{
			// i++;
			A_fundec head = function->head;

			// check : same name
			if (S_look(venv, head->name))
			{
				EM_error(d->pos, "two functions have the same name");
				function = function->tail;
				continue;
			}

			Ty_ty result;
			// check : result
			if (head->result)
			{
				result = S_look(tenv, head->result);
				if (!result)
				{
					EM_error(head->pos, "undefined result type %s", S_name(head->result));
					result = Ty_Void();
				}
			}
			else
			{
				result = Ty_Void();
			}

			Ty_tyList formalTys = makeFormalTyList(tenv, head->params);

			E_enventry e = E_FunEntry(formalTys, result);
			e->kind = E_funEntry;
			S_enter(venv, head->name, e);

			function = function->tail;
		}
	function = get_funcdec_list(d);

		// EM_error(function->head->pos, "number of fun %d", i);
		
	
		// input params check body
		while (function)
		{
			A_fundec head = function->head;
			Ty_tyList formalTys = makeFormalTyList(tenv, head->params);
			A_fieldList formalNames = head->params;

			S_beginScope(venv);
			while (formalNames)
			{
				S_enter(venv, formalNames->head->name, E_VarEntry(formalTys->head));
				formalNames = formalNames->tail;
				formalTys = formalTys->tail;
			}
			
			struct expty exp = transExp(venv, tenv, head->body);
			// EM_error(head->pos, "undefined function  %s", S_name(head->body->u.call.func));
			// EM_error(head->pos, "undefined function  %d", head->body->kind);
			E_enventry e = S_look(venv, head->name);

			// check return type
			if (actual_ty(e->u.fun.result)->kind == Ty_void)
			{ // if no value should be returned
				if (actual_ty(exp.ty)->kind != Ty_void)
				{ // but body returns something
					EM_error(function->head->pos, "procedure returns value");
				}
			}
			else if (actual_ty(e->u.fun.result)->kind != actual_ty(exp.ty)->kind)
			{ // when types do not match
				EM_error(function->head->pos, "procedure returns unexpected type");
			}
			S_endScope(venv);

			function = function->tail;
		}
		break;
	}

	case A_varDec:
	{
		// EM_error(d->pos, "type not exist %d", get_vardec_init(d)->kind);
		struct expty init = transExp(venv, tenv, get_vardec_init(d));
		if (get_vardec_typ(d) != NULL)
		{
			Ty_ty type = S_look(tenv, get_vardec_typ(d));
			if (!type)
			{
				EM_error(d->pos, "type not exist %s", S_name(get_vardec_typ(d)));
			}

			if (actual_ty(type) != actual_ty(init.ty))
			{
				EM_error(d->pos, "type mismatch");
			}
		}
		else if (get_expty_kind(init) == Ty_nil)
		{
			EM_error(d->pos, "init should not be nil without type specified");
		}
		S_enter(venv, get_vardec_var(d), E_VarEntry(init.ty));
		break;
	}

	case A_typeDec:
	{
		A_nametyList types = get_typedec_list(d);

		// check : same name
		while (types)
		{
			if (S_look(tenv, types->head->name) != NULL)
			{
				EM_error(d->pos, "two types have the same name");
			}
			else
			{
				S_enter(tenv, types->head->name, Ty_Name(types->head->name, NULL));
			}

			types = types->tail;
		}

		// resolve reference
		types = get_typedec_list(d);
		while (types)
		{
			Ty_ty type = S_look(tenv, types->head->name);
			type->u.name.ty = transTy(tenv, types->head->ty);
			types = types->tail;
		}

		types = d->u.type;
		while (types)
		{
			Ty_ty init = S_look(tenv, types->head->name);
			Ty_ty type = init;
			while ((type = type->u.name.ty)->kind == Ty_name)
			{
				// printf("checking name type %s\n", S_name(type->u.name.sym));
				if (type == init)
				{
					EM_error(d->pos, "illegal type cycle");
					init->u.name.ty = Ty_Int();
					break;
				}
			}

			types = types->tail;
		}

		break;
	}
	}
}

void SEM_transProg(A_exp exp)
{
	S_table tenv = E_base_tenv();
	S_table venv = E_base_venv();
	transExp(venv, tenv, exp);
}