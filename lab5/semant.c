#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "absyn.h"
#include "types.h"
#include "env.h"
#include "semant.h"
#include "helper.h"
#include "translate.h"

/*Lab5: Your implementation of lab5.*/

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
U_boolList makeFormalBoolList(A_fieldList fields)
{
	if (fields)
	{
		return U_BoolList(fields->head->escape, makeFormalBoolList(fields->tail));
	}
	else
	{
		return NULL;
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

Ty_fieldList makeFieldList(S_table tenv, A_fieldList fields)
{
	Ty_ty type = S_look(tenv, fields->head->typ);
	if (!type)
	{
		EM_error(fields->head->pos, "undefined type %s", S_name(fields->head->typ));
		type = Ty_Int();
	}
	Ty_field field = Ty_Field(fields->head->name, type);

	if (fields->tail == NULL)
	{
		return Ty_FieldList(field, NULL);
	}
	else
	{
		return Ty_FieldList(field, makeFieldList(tenv, fields->tail));
	}
}
struct expty transExp(S_table venv, S_table tenv, A_exp a, Tr_level l, Temp_label breakLabel)
{
	switch (a->kind)
	{
	case A_varExp:
	
		return transVar(venv, tenv, a->u.var, l, breakLabel);

	case A_nilExp:
		return expTy(Tr_nil(), Ty_Nil());

	case A_intExp:
		return expTy(Tr_int(a->u.intt), Ty_Int());

	case A_stringExp:
		return expTy(Tr_string(a->u.stringg), Ty_String());

	case A_callExp:
	{
		E_enventry func = S_look(venv, get_callexp_func(a));
		/*remember: E_enventry is a pointer */

		// check : if there is "func" in S_table
		if (!func)
		{
			EM_error(a->pos, "undefined function %s", S_name(get_callexp_func(a)));

			//why : renturn int?
			return expTy(Tr_nil(), Ty_Int());
		}

		// check : if it's function
		if (func->kind != E_funEntry)
		{
			EM_error(a->pos, "%s is not a function", S_name(get_callexp_func(a)));

			//why : renturn int?
			return expTy(Tr_nil(), Ty_Int());
		}

		// check : if formals and actuals match on type
		Ty_tyList formals = get_func_tylist(func);
		A_expList actuals = get_callexp_args(a);
		Tr_expList params = NULL;
		while (formals != NULL && actuals != NULL)
		{
			// get the type of actual
			struct expty e = transExp(venv, tenv, actuals->head, l, breakLabel);
			if (e.ty != formals->head)
			{
				EM_error(a->pos, "A_callExp : para type mismatch");

				return expTy(Tr_nil(), actual_ty(get_func_res(func)));
			}
			params = Tr_ExpList(e.exp, params);
			formals = formals->tail;
			actuals = actuals->tail;
		}

		// check : the number of para
		if (formals != NULL)
		{
			EM_error(a->pos, "too less params in function %s", S_name(get_callexp_func(a)));
			return expTy(Tr_call(func->u.fun.level, func->u.fun.label, params, l), actual_ty(get_func_res(func)));
		}

		if (actuals != NULL)
		{
			EM_error(a->pos, "too many params in function %s", S_name(get_callexp_func(a)));
			return expTy(Tr_call(func->u.fun.level, func->u.fun.label, params, l), actual_ty(get_func_res(func)));
		}

		return expTy(Tr_call(func->u.fun.level, func->u.fun.label, params, l), actual_ty(get_func_res(func)));
	}

	case A_opExp:
	{
		A_oper oper = get_opexp_oper(a);
		// EM_error(get_opexp_leftpos(a),"op %d",oper);
		struct expty left = transExp(venv, tenv, get_opexp_left(a), l, breakLabel);
		struct expty right = transExp(venv, tenv, get_opexp_right(a), l, breakLabel);

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
			return expTy(Tr_opA(oper, left.exp, right.exp), Ty_Int());
		}

		// check : other op left and right should be same
		if (oper == A_eqOp || oper == A_neqOp || oper == A_ltOp || oper == A_leOp || oper == A_gtOp || oper == A_geOp)
		{
			// EM_error(get_opexp_leftpos(a), "s1ame type required ");
			if (left.ty->kind != right.ty->kind)
			{
				EM_error(get_opexp_leftpos(a), "same type required ");
			}
			return expTy(Tr_opL(oper, left.exp, right.exp, get_expty_kind(left) == Ty_string), Ty_Int());
		}
		assert(0);
		break;
	}

	case A_recordExp:
	{
		Ty_ty ty = S_look(tenv, get_recordexp_typ(a));

		// check :  if it's in tevn
		if (ty == NULL)
		{
			EM_error(a->pos, "undefined type %s", S_name(get_recordexp_typ(a)));
			return expTy(Tr_nil(), Ty_Record(NULL));
		}
		ty = actual_ty(ty);
		// check : if it's record
		if (ty->kind != Ty_record)
		{
			EM_error(a->pos, "same type required %s", S_name(get_recordexp_typ(a)));
			return expTy(Tr_nil(), Ty_Record(NULL));
		}

		// check : filed
		Ty_fieldList expected = ty->u.record;
		A_efieldList actuals = get_recordexp_fields(a);
		int num = 0;
		Tr_expList trExps = NULL;

		while (actuals != NULL && expected != NULL)
		{

			// check : name
			if (expected->head->name != actuals->head->name)
			{
				EM_error(a->pos, "A_recordExp : expected %s but get %s", S_name(expected->head->name), S_name(actuals->head->name));
			}
			struct expty exp = transExp(venv, tenv, actuals->head->exp, l, breakLabel);
			// check : type
			if (expected->head->ty != exp.ty)
			{
				EM_error(a->pos, "A_recordExp : type not match");
			}
			trExps = Tr_ExpList(exp.exp, trExps);
			num++;
			actuals = actuals->tail;
			expected = expected->tail;
		}

		// check : number
		if (expected != NULL || actuals != NULL)
		{
			EM_error(a->pos, "A_recordExp : field number of %s does not match", S_name(a->u.record.typ));
		}
		return expTy(Tr_record(num, trExps), ty);
	}

	case A_seqExp:
	{

		// 返回最后一个表达式的返回类型
		struct expty last;
		Tr_exp exp = Tr_nil();
		Ty_ty ty = Ty_Void();

		A_expList exps;
		for (exps = get_seqexp_seq(a); exps; exps = exps->tail)
		{
			last = transExp(venv, tenv, exps->head, l, breakLabel);
			exp = Tr_seq(exp, last.exp);
			ty = last.ty;
		}

		last = expTy(exp, ty);

		return last;
		// TODO : if seq == NULL ?
	}

	case A_assignExp:
	{
		// check : if var type = exp type
		EM_error(0, "A_assignExp");
		struct expty ee = transExp(venv, tenv, get_assexp_exp(a), l, breakLabel);
		struct expty ev = transVar(venv, tenv, get_assexp_var(a), l, breakLabel);
		if (ee.ty != ev.ty)
		{
			EM_error(a->pos, "A_assignExp : unmatched assign exp");
		}
// EM_error(a->pos, "A_assignExp : unmatched assign exp");
		if (get_assexp_var(a)->kind == A_simpleVar)
		{
			
			E_enventry x = S_look(venv, get_simplevar_sym(get_assexp_var(a)));
			if (x && x->readonly)
			{
				EM_error(a->pos, "loop variable can't be assigned");
			}
		}

		return expTy(Tr_assign(ev.exp, ee.exp), Ty_Void());

		// loop variables?
	}

	case A_ifExp:
	{
		// EM_error(a->u.iff.test->pos, "type of test expression shoulf be int");
		struct expty test = transExp(venv, tenv, a->u.iff.test, l, breakLabel);

		if (actual_ty(test.ty)->kind != Ty_int)
		{
			EM_error(a->u.iff.test->pos, "type of test expression shoulf be int");
		}
		struct expty then = transExp(venv, tenv, get_ifexp_then(a), l, breakLabel);

		// EM_error(a->u.iff.test->pos, "%s",);
		// 如果有else
		// 之所以写成A_nilExp 是因为tigher.y 里面
		// IF exp THEN exp  {$$ = A_IfExp(EM_tokPos, $2, $4, A_NilExp(EM_tokPos))
		if (get_ifexp_else(a))
		{
			struct expty elsee = transExp(venv, tenv, get_ifexp_else(a), l, breakLabel);
			if (actual_ty(then.ty) != actual_ty(elsee.ty))
			{
				EM_error(a->u.iff.then->pos, "then exp and else exp type mismatch");
				return expTy(Tr_nil(), then.ty);
			}

			return expTy(Tr_ifThenElse(test.exp, then.exp, elsee.exp), then.ty);
		}
		else
		{
			if (then.ty->kind != Ty_void)
			{
				EM_error(a->u.iff.then->pos, "if-then exp's body must produce no value");
				return expTy(Tr_nil(), then.ty);
			}

			return expTy(Tr_ifThen(test.exp, then.exp), Ty_Void());
		}
	}

	case A_whileExp:
	{
		struct expty test = transExp(venv, tenv, get_whileexp_test(a), l, breakLabel);
		struct expty body = transExp(venv, tenv, get_whileexp_body(a), l, breakLabel);

		// break
		Temp_label done = Temp_newlabel();

		// check :  test should be int
		if (get_expty_kind(test) != Ty_int)
		{
			EM_error(a->pos, "type of test expression should be int");
			// return expTy(NULL, Ty_Void());
		}

		// check : body should be void
		if (get_expty_kind(body) != Ty_void)
		{
			EM_error(a->pos, "while body must produce no value");
			// return expTy(NULL, Ty_Void());
		}
		return expTy(Tr_while(test.exp, body.exp, done), Ty_Void());
	}

	case A_forExp:
	{
		// break
		Temp_label done = Temp_newlabel();

		struct expty lo = transExp(venv, tenv, get_forexp_lo(a), l, breakLabel);
		struct expty hi = transExp(venv, tenv, get_forexp_hi(a), l, breakLabel);

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
		Tr_access access = Tr_allocLocal(l, a->u.forr.escape);
		S_enter(venv, get_forexp_var(a), E_ROVarEntry(access, Ty_Int()));

		struct expty body = transExp(venv, tenv, get_forexp_body(a), l, done);

		if (get_expty_kind(body) != Ty_void)
		{
			EM_error(get_forexp_body(a)->pos, "type of body expression should be void");
		}
		S_endScope(venv);

		return expTy(Tr_for(access, l, lo.exp, hi.exp, body.exp, done), Ty_Void());
	}

	case A_breakExp:
	{
		if (breakLabel)
		{
			return expTy(Tr_break(breakLabel), Ty_Void());
		}
		else
		{
			return expTy(Tr_nil(), Ty_Void());
		}
	}

	case A_letExp:
	{
		EM_error(0, "A_letExp");
		struct expty e;
		Tr_expList eseq = NULL;
		A_decList d;
		S_beginScope(venv);
		S_beginScope(tenv);
		for (d = get_letexp_decs(a); d; d = d->tail)
		{
			transDec(venv, tenv, d->head, l, breakLabel);
		}
		e = transExp(venv, tenv, get_letexp_body(a), l, breakLabel);
		 eseq = Tr_ExpList(e.exp, eseq);
		EM_error(0, "A_letExp : finish");
		S_endScope(tenv);
		S_endScope(venv);
		return expTy(Tr_seq(eseq,NULL), e.ty);
	}
	case A_arrayExp:
	{
		EM_error(0, "A_arrayExp");
		Ty_ty t = S_look(tenv, get_arrayexp_typ(a));
		struct expty e = transExp(venv, tenv, get_arrayexp_init(a), l, breakLabel);
		if (e.ty != actual_ty(t)->u.array)
		{
			EM_error(a->pos, "type mismatch");
			return expTy(Tr_nil(), Ty_Int());
		}
		return expTy(Tr_nil(), actual_ty(t));
	}
	}
}
struct expty transVar(S_table venv, S_table tenv, A_var v, Tr_level l, Temp_label breakLabel)
{
	switch (v->kind)
	{
	case A_simpleVar:
	{
		S_symbol id = get_simplevar_sym(v);
		
		E_enventry x = S_look(venv, v->u.simple);
		
		if (x && x->kind == E_varEntry)
		{
			
			return expTy(Tr_simpleVar(x->u.var.access, l), actual_ty(get_varentry_type(x)));
		}
		else
		{
			EM_error(v->pos, "A_simpleVar : undefined variable %s", S_name(get_simplevar_sym(v)));
			return expTy(Tr_nil(), Ty_Int());
		}
	}

	case A_fieldVar:
	{
		struct expty e = transVar(venv, tenv, get_fieldvar_var(v), l, breakLabel);

		// check : if it's record
		if (get_expty_kind(e) != Ty_record)
		{
			EM_error(get_fieldvar_var(v)->pos, "A_fieldVar : not a record type");
			return expTy(Tr_nil(), Ty_Int());
		}
		Ty_fieldList record = e.ty->u.record;

		// check : if record has this field
		while (record != NULL)
		{
			if (record->head->name == get_fieldvar_sym(v))
			{
				return expTy(Tr_nil(), actual_ty(record->head->ty));
			}
			record = record->tail;
		}
		EM_error(get_fieldvar_var(v)->pos, "A_fieldVar : field %s doesn't exist", S_name(get_fieldvar_sym(v)));
		return expTy(Tr_nil(), Ty_Int());
	}

	case A_subscriptVar:
	{
		// check : if exp is int
		EM_error(0, "A_subscriptVar");
		struct expty exp = transExp(venv, tenv, get_subvar_exp(v), l, breakLabel);
		if (get_expty_kind(exp) != Ty_int)
		{
			EM_error(get_subvar_exp(v)->pos, "interger subscript required");
			return expTy(Tr_nil(), Ty_Int());
		}

		struct expty var = transVar(venv, tenv, get_subvar_var(v), l, breakLabel);
		if (get_expty_kind(var) != Ty_array)
		{
			EM_error(get_subvar_var(v)->pos, "array type required");
			return expTy(Tr_nil(), Ty_Int());
		}
		return expTy(Tr_subscriptVar(var.exp, exp.exp), actual_ty(get_array(var)));
	}
	}
}

F_fragList SEM_transProg(A_exp exp)
{
	//TODO LAB5: do not forget to add the main frame
	Tr_level mainFrame = Tr_outermost();
	struct expty e = transExp(E_base_venv(), E_base_tenv(), exp, mainFrame, NULL);
	Tr_func(e.exp, mainFrame);

	return Tr_getResult();
}

// Ty_ty transTy(S_table tenv, A_ty a)
// {
// 	switch (a->kind)
// 	{
// 	case A_nameTy:
// 	{
// 		Ty_ty t = S_look(tenv, get_ty_name(a));
// 		if (t == NULL)
// 		{
// 			EM_error(a->pos, "undefined type %s", S_name(get_ty_name(a)));
// 			return Ty_Name(a->u.name, t);
// 		}
// 	}

// 	case A_recordTy:
// 	{
// 		// A_fieldList f;
// 		// Ty_fieldList tf = NULL;
// 		// Ty_ty t;
// 		// for (f = get_ty_record(a); f; f = f->tail)
// 		// {
// 		// 	t = S_look(tenv, f->head->typ);
// 		// 	if (t == NULL)
// 		// 	{
// 		// 		EM_error(f->head->pos, "undefined type %s", S_name(f->head->typ));
// 		// 		t = Ty_Int();
// 		// 	}
// 		// 	tf = Ty_FieldList(Ty_Field(f->head->name, t), tf);
// 		// }
// 		// return Ty_Record(tf);
// 		return Ty_Record(makeFieldList(tenv, a->u.record));
// 	}
// 	case A_arrayTy:
// 	{
// 		Ty_ty t = S_look(tenv, get_ty_array(a));
// 		if (t != NULL)
// 		{
// 			return Ty_Array(t);
// 		}
// 		else
// 		{
// 			EM_error(a->pos, "undefined type %s", S_name(get_ty_array(a)));
// 			return Ty_Array(Ty_Int());
// 		}
// 	}
// 	}
// }

Ty_ty transTy(S_table tenv, A_ty a) {
    switch (a->kind) {
        case A_nameTy: {
            Ty_ty type = S_look(tenv, a->u.name);
            if (!type) {
                EM_error(a->pos, "undefined type %s", S_name(a->u.name));
                return Ty_Int();
            }

            return Ty_Name(a->u.name, type);
        }
        case A_recordTy:
            return Ty_Record(makeFieldList(tenv, a->u.record));
        case A_arrayTy: {
			EM_error(0, "A_arrayTy");
            Ty_ty type = S_look(tenv, a->u.array);
            if (!type) {
                EM_error(a->pos, "undefined type %s", S_name(a->u.array));
                return Ty_Int();
            }

            return Ty_Array(type);
        }
    }

    return NULL;
}
void transDec(S_table venv, S_table tenv, A_dec d, Tr_level l, Temp_label breakLable)
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

			// Check name duplicated in funcList
			for (A_fundecList afterFuncList = function->tail; afterFuncList;
				 afterFuncList = afterFuncList->tail)
			{
				if (head->name == afterFuncList->head->name)
				{
					EM_error(d->pos, "two functions have the same name");
					break;
				}
			}

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
			U_boolList formalBools = makeFormalBoolList(head->params);
			E_enventry e = E_FunEntry(Tr_newLevel(l, Temp_newlabel(), formalBools), Temp_newlabel(), formalTys, result);
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
			E_enventry funEntry = S_look(venv, head->name);

			Tr_accessList accesses = Tr_formals(funEntry->u.fun.level);
			S_beginScope(venv);
			while (formalNames)
			{
				S_enter(venv, formalNames->head->name, E_VarEntry(accesses->head, formalTys->head));
				formalNames = formalNames->tail;
				formalTys = formalTys->tail;
				accesses = accesses->tail;
			}

			struct expty exp = transExp(venv, tenv, head->body, funEntry->u.fun.level, breakLable);
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
		EM_error(0, "A_varDec");
		// 先赋值
		// EM_error(0, "A_varDec:%s",get_vardec_init(d)->kind);
		struct expty init = transExp(venv, tenv, get_vardec_init(d), l, breakLable);
		
		Tr_access access = Tr_allocLocal(l, d->u.var.escape);
		if (strcmp(S_name(get_vardec_typ(d)), ""))
		{
			
			Ty_ty type = S_look(tenv, get_vardec_typ(d));
			if (!type)
			{
				EM_error(d->pos, "type not exist %s", S_name(get_vardec_typ(d)));
				type = Ty_Int();
			}

			if (actual_ty(type) != actual_ty(init.ty))
			{
				EM_error(d->pos, "type mismatch");
			}
			
			S_enter(venv, get_vardec_var(d), E_VarEntry(access, type));
		}
		else if (get_expty_kind(init) == Ty_nil)
		{
			EM_error(d->pos, "init should not be nil without type specified");
			S_enter(venv, get_vardec_var(d), E_VarEntry(access, init.ty));
		}
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

// #include <stdio.h>
// #include <assert.h>
// #include <string.h>
// #include "util.h"
// #include "errormsg.h"
// #include "symbol.h"
// #include "absyn.h"
// #include "types.h"
// #include "env.h"
// #include "semant.h"
// #include "helper.h"
// #include "translate.h"

// /*Lab4: Your implementation of lab4*/
// /* Func prototype */
// bool isSameType(Ty_ty left, Ty_ty right);
// Ty_ty actual_ty(Ty_ty ty);
// Ty_fieldList getTyFieldList(A_pos pos, S_table tenv, A_fieldList fields);
// Ty_fieldList actual_tys(Ty_fieldList fields);
// U_boolList makeFormalBoolList(A_fieldList fields);

// struct expty 
// {
// 	Tr_exp exp; 
// 	Ty_ty ty;
// };

// //In Lab4, the first argument exp should always be **NULL**.
// struct expty expTy(Tr_exp exp, Ty_ty ty)
// {
// 	struct expty e;

// 	e.exp = exp;
// 	e.ty = ty;

// 	return e;
// };

// F_fragList SEM_transProg(A_exp exp){
// 	//TODO LAB5: do not forget to add the main frame
// 	Tr_level mainFrame = Tr_outermost();
// 	struct expty e = transExp(E_base_venv(), E_base_tenv(), exp, mainFrame, NULL);
// 	Tr_func(e.exp, mainFrame);

// 	return Tr_getResult();
// }

// /* transExp, all Exp kind can be found in absyn.h */
// struct expty transExp(S_table venv, S_table tenv, A_exp a, Tr_level l, Temp_label breakLabel) {
// 	switch(a->kind) {
// 		case A_varExp:
// 			return transVar(venv, tenv, a->u.var, l, breakLabel);

// 		case A_nilExp:
// 			return expTy(Tr_nil(), Ty_Nil());

// 		case A_intExp:
// 			return expTy(Tr_int(a->u.intt), Ty_Int());

// 		case A_stringExp:
// 			return expTy(Tr_string(a->u.stringg), Ty_String());

// 		case A_callExp: {
// 			/* Check func id */
// 			E_enventry x = S_look(venv, get_callexp_func(a));
// 			if (x && x->kind == E_funEntry) {
// 				/* Check args */
// 				A_expList args;
// 				Ty_tyList argTypes;
// 				struct expty exp;
// 				Tr_expList params = NULL;
// 				for (args = get_callexp_args(a), argTypes = get_func_tylist(x); 
// 					args && argTypes; args = args->tail, argTypes = argTypes->tail) {
// 					exp = transExp(venv, tenv, args->head, l, breakLabel);
// 					if(!isSameType(argTypes->head, exp.ty)) {
// 						EM_error(args->head->pos, "para type mismatch");
// 					}
// 					params = Tr_ExpList(exp.exp, params);
// 				}

// 				if (args) {
// 					EM_error(a->pos, "too many params in function %s", S_name(get_callexp_func(a)));
// 				}

// 				if (argTypes) {
// 					EM_error(a->pos, "too few params in function %s", S_name(get_callexp_func(a)));
// 				}

// 				return expTy(Tr_call(x->u.fun.level, x->u.fun.label, params, l), get_func_res(x));
// 			} else {
// 				EM_error(a->pos, "undefined function %s", S_name(get_callexp_func(a)));
// 				return expTy(Tr_nil(), Ty_Int());
// 			}
// 		}

// 		case A_opExp: {
// 			/* left op right */
// 			struct expty left = transExp(venv, tenv, get_opexp_left(a), l, breakLabel);
// 			struct expty right = transExp(venv, tenv, get_opexp_right(a), l, breakLabel);
// 			A_oper oper = get_opexp_oper(a);
// 			if (oper == A_plusOp || oper == A_minusOp || 
// 				oper == A_timesOp || oper == A_divideOp) {
// 				if (get_expty_kind(left) != Ty_int) {
// 					EM_error(get_opexp_leftpos(a), "integer required");
// 				}

// 				if (get_expty_kind(right) != Ty_int) {
// 					EM_error(get_opexp_rightpos(a), "integer required");
// 				}
// 				return expTy(Tr_opA(oper, left.exp, right.exp), Ty_Int());

// 			} else if (oper == A_eqOp || oper == A_neqOp) {
// 				if (!isSameType(left.ty, right.ty)) {
// 					EM_error(a->pos, "same type required");
// 				}
// 				return expTy(Tr_opL(oper, left.exp, right.exp, get_expty_kind(left) == Ty_string), Ty_Int());

// 			} else {
// 				if (!((get_expty_kind(left) == Ty_int && get_expty_kind(right) == Ty_int) ||
//                      (get_expty_kind(left) == Ty_string && get_expty_kind(right) == Ty_string))) {
//             		EM_error(a->pos, "same type required");
// 				}
// 				return expTy(Tr_opL(oper, left.exp, right.exp, get_expty_kind(left) == Ty_string), Ty_Int());
// 			}
// 		}

// 		case A_recordExp: {
// 			/* Check whether type exists */
// 			S_symbol typ = get_recordexp_typ(a);
// 			Ty_ty ty = S_look(tenv, typ);
// 			if (!ty) {
// 				EM_error(a->pos, "undefined type %s", S_name(typ));
// 				return expTy(Tr_nil(), Ty_Int());
// 			}

// 			/* Check whether type is a record */
// 			ty = actual_ty(ty);
// 			if (ty->kind != Ty_record) {
//                 EM_error(a->pos, "not a record type");
//                 return expTy(Tr_nil(), Ty_Int());
//             }

//             /* Check whether the fields in efieldList match the record type */
//             A_efieldList efields;
//             Ty_fieldList fields;
//             struct expty exp;
//             int n = 0;
//             Tr_expList trExps = NULL; 

//             for (efields = get_recordexp_fields(a), fields = ty->u.record; efields && fields; 
//             	efields = efields->tail, fields = fields->tail) {
//                 exp = transExp(venv, tenv, efields->head->exp, l, breakLabel);

//                 if (efields->head->name != fields->head->name || 
//                 	!isSameType(exp.ty, fields->head->ty)) {
//                     EM_error(efields->head->exp->pos, "type mismatch");
//                 }

//                 trExps = Tr_ExpList(exp.exp, trExps);
//                 n++;
//             }

//             if (efields || fields) {
//                 EM_error(a->pos, "type mismatch");
//             }

// 			return expTy(Tr_record(n, trExps), ty);
// 		}

// 		case A_seqExp: {
// 			/* The result is the type of last exp, if the exp has return */
// 			struct expty last;
// 			Tr_exp exp = Tr_nil();
// 			Ty_ty ty = Ty_Void();

// 			A_expList exps;
// 			for (exps = get_seqexp_seq(a); exps; exps = exps->tail) {
// 				last = transExp(venv, tenv, exps->head, l, breakLabel);
// 				exp = Tr_seq(exp, last.exp);
// 				ty = last.ty;
// 			}

// 			last = expTy(exp, ty);

// 			return last;
// 		}

// 		case A_assignExp: {
// 			/* return void */
// 			Ty_ty ty = Ty_Void();

// 			/* var := exp */
// 			A_var var = get_assexp_var(a);
// 			A_exp exp = get_assexp_exp(a);
// 			struct expty left = transVar(venv, tenv, var, l, breakLabel);
// 			struct expty right = transExp(venv, tenv, exp, l, breakLabel);

// 			/* Loop var cannot be assigned */
// 			if (var->kind == A_simpleVar) {
// 				E_enventry x = S_look(venv, get_simplevar_sym(var));
// 				if (x->readonly == 1) {
//                 	EM_error(a->pos, "loop variable can't be assigned");
//                 }
//             }

//             /* Check var type and exp type */
//             if (!isSameType(left.ty, right.ty)) {
//                 EM_error(a->pos, "unmatched assign exp");
// 			}

// 			return expTy(Tr_assign(left.exp, right.exp), ty);
// 		}

// 		case A_ifExp: {
// 			/* if exp1 then exp2 (else exp3) */
// 			A_exp test = get_ifexp_test(a);
// 			A_exp then = get_ifexp_then(a);
// 			A_exp elsee = get_ifexp_else(a);

// 			/* exp1 should be int */
// 			struct expty exp1 = transExp(venv, tenv, test, l, breakLabel);
//            	if (get_expty_kind(exp1) != Ty_int) {
//             	EM_error(test->pos, "if required integer");
//             }

//             struct expty exp2 = transExp(venv, tenv, then, l, breakLabel);
//             /* exp3 exists, check the type of exp2 and exp3 */
//             if (elsee) {
//                 struct expty exp3 = transExp(venv, tenv, elsee, l, breakLabel);
//                 if (!isSameType(exp2.ty, exp3.ty)) {
//                     EM_error(a->pos, "then exp and else exp type mismatch");
//                 }

//                 return expTy(Tr_ifThenElse(exp1.exp, exp2.exp, exp3.exp), exp2.ty);
//             } else {
//                 if (get_expty_kind(exp2) != Ty_void) {
//                     EM_error(a->pos, "if-then exp's body must produce no value");
//                 }

//                 return expTy(Tr_ifThen(exp1.exp, exp2.exp), Ty_Void());
// 			}
// 		}

// 		case A_whileExp: {
// 			/* for break */
// 			Temp_label done = Temp_newlabel();

// 			/* while exp1 do exp2 */
// 			A_exp test = get_whileexp_test(a);
// 			A_exp body = get_whileexp_body(a);

// 			/* exp1 should be int */
// 			struct expty exp1 = transExp(venv, tenv, test, l, breakLabel);
// 			if (get_expty_kind(exp1) != Ty_int) {
//             	EM_error(test->pos, "while test required int");
//             }

//             /* exp2 should be void */
//             struct expty exp2 = transExp(venv, tenv, body, l, done);
//             if (get_expty_kind(exp2) != Ty_void) {
//             	EM_error(body->pos, "while body must produce no value");
//             }

//             return expTy(Tr_while(exp1.exp, exp2.exp, done), Ty_Void());
// 		}

// 		case A_forExp: {
// 			/* for break */
// 			Temp_label done = Temp_newlabel();

// 			/* for var := exp1 to exp2 do exp3 */
// 			S_symbol var = get_forexp_var(a);
// 			A_exp lo = get_forexp_lo(a);
// 			A_exp hi = get_forexp_hi(a);
// 			A_exp body = get_forexp_body(a);

// 			/* exp1, exp2 should be int */
// 			struct expty exp1 = transExp(venv, tenv, lo, l, breakLabel);
// 			if (get_expty_kind(exp1) != Ty_int) {
//             	EM_error(lo->pos, "for lo required int");
//             }

//             struct expty exp2 = transExp(venv, tenv, hi, l, breakLabel);
// 			if (get_expty_kind(exp2) != Ty_int) {
//             	EM_error(hi->pos, "for exp's range type is not integer");
//             }

//             /* Enter var into env */
//             S_beginScope(venv);
//             Tr_access access = Tr_allocLocal(l, a->u.forr.escape);
//             S_enter(venv, var, E_ROVarEntry(access, Ty_Int()));
//             /* exp3 should be void */
//             struct expty exp3 = transExp(venv, tenv, body, l, done);
//             if (get_expty_kind(exp3) != Ty_void) {
//                 EM_error(body->pos, "for body required void");
//             }
//             S_endScope(venv);

// 			return expTy(Tr_for(access, l, exp1.exp, exp2.exp, exp3.exp, done), Ty_Void());
// 		}

// 		case A_breakExp: {
//         	if (breakLabel) {
//         		return expTy(Tr_break(breakLabel), Ty_Void());	
//             } else {
//             	return expTy(Tr_nil(), Ty_Void());
//             }
// 		}

// 		case A_letExp: {
// 			Tr_exp trSeq = Tr_nil();

// 			/* let decs in expseq end */
//             A_decList decs;
//             A_exp body = get_letexp_body(a);

//             /* Check dec */
//             S_beginScope(venv);
//             S_beginScope(tenv);
//             for (decs = get_letexp_decs(a); decs; decs = decs->tail) {
//                 trSeq = Tr_seq(trSeq, transDec(venv, tenv, decs->head, l, breakLabel));
//             }
//             struct expty exp = transExp(venv, tenv, body, l, breakLabel);
//             exp.exp = Tr_seq(trSeq, exp.exp);
//             S_endScope(tenv);
//             S_endScope(venv);

// 			/* The result is the type of last exp, if the exp has return */
// 			return exp;
// 		}

// 		case A_arrayExp: {
// 			/* var exp1 of exp2 */
// 			S_symbol typ = get_arrayexp_typ(a);
// 			A_exp size = get_arrayexp_size(a);
// 			A_exp init = get_arrayexp_init(a);

// 			/* Check whether type exists */
// 			Ty_ty ty = S_look(tenv, typ);
// 			if (!ty) {
// 				EM_error(a->pos, "undefined type %s", typ);
// 				return expTy(Tr_nil(), Ty_Int());
// 			}

// 			/* Check whether type is a array */
// 			ty = actual_ty(ty);
// 			if (ty->kind != Ty_array) {
//                 EM_error(a->pos, "array type required");
//                 return expTy(Tr_nil(), Ty_Int());
//             }

//             /* exp1 should be int */
//             struct expty exp1 = transExp(venv, tenv, size, l, breakLabel);
//             if (get_expty_kind(exp1) != Ty_int) {
//                 EM_error(size->pos, "array size required int");
//             }

//             /* exp2 should be the type of array */
//             struct expty exp2 = transExp(venv, tenv, init, l, breakLabel);
//             if (!isSameType(exp2.ty, get_ty_array(ty))) {
//                 EM_error(size->pos, "array init type mismatch");
//             }

//             return expTy(Tr_array(exp1.exp, exp2.exp), ty);
// 		}
// 	}
// }

// struct expty transVar(S_table venv, S_table tenv, A_var v, Tr_level l, Temp_label breakLabel) {
// 	switch (v->kind) {
// 		case A_simpleVar: {
// 			/* id */
// 			/* Check id */
// 			S_symbol id = get_simplevar_sym(v);
// 			E_enventry x = S_look(venv, id);
// 			if (x && x->kind == E_varEntry) {
// 				return expTy(Tr_simpleVar(x->u.var.access, l), get_varentry_type(x));
// 			} else {
// 				EM_error(v->pos, "undefined variable %s", S_name(id));
// 				return expTy(Tr_nil(), Ty_Int());
// 			}
// 		}

// 		case A_fieldVar: {
// 			A_var lvalue = get_fieldvar_var(v);
// 			S_symbol id = get_fieldvar_sym(v);

// 			/* lvalue should be record */
// 			struct expty var = transVar(venv, tenv, lvalue, l, breakLabel);
// 			if (get_expty_kind(var) != Ty_record) {
// 				EM_error(lvalue->pos, "not a record type");
// 				return expTy(Tr_nil(), Ty_Int());
// 			} 

// 			/* Check whether id in fields */
// 			Ty_fieldList fields;
// 			int n = 0;
// 			for (fields = get_record_fieldlist(var); fields; fields = fields->tail) {
// 				if (fields->head->name == id) {
// 					return expTy(Tr_fieldVar(var.exp, n), fields->head->ty);
// 				}
// 				n++;
// 			}

// 			EM_error(lvalue->pos, "field %s doesn't exist", S_name(id));
// 			return expTy(Tr_nil(), Ty_Int());
// 		}

// 		case A_subscriptVar: {
// 			/* var[exp] */
// 			A_var lvalue = get_subvar_var(v);
// 			A_exp index = get_subvar_exp(v);

// 			/* lvalue should be array */
// 			struct expty var = transVar(venv, tenv, lvalue, l, breakLabel);
// 			if (get_expty_kind(var) != Ty_array) {
// 				EM_error(lvalue->pos, "array type required");
// 				return expTy(Tr_nil(), Ty_Int());
// 			} 

// 			/* exp should be int */
// 			struct expty exp = transExp(venv, tenv, index, l, breakLabel);
// 			if (get_expty_kind(exp) != Ty_int) {
// 				EM_error(index->pos, "int type required");
// 			} 

// 			return expTy(Tr_subscriptVar(var.exp, exp.exp), get_array(var));
// 		}
// 	}
// }

// Tr_exp transDec(S_table venv, S_table tenv, A_dec d, Tr_level l, Temp_label breakLabel) {
// 	switch (d->kind) {
// 		case A_functionDec: {
// 			/* funcList: name-sym (params-fieldList) : result-sym = body-exp......  */
// 			/* Put all func heads into venv */ 
// 			A_fundecList functions, afterFuncList;
// 			A_fundec func;
// 			S_symbol name, typ, result;
// 			A_fieldList params;
// 			A_field field;
// 			Ty_tyList formals, formalsTail;
// 			Ty_ty formal, resultTy;
// 			U_boolList formalBools;
// 			E_enventry funEntry;
// 			struct expty exp;
// 			for (functions = get_funcdec_list(d); functions; functions = functions->tail) {
// 				func = functions->head;
// 				name = func->name;
// 				/* Check name duplicated in funcList */
// 				for (afterFuncList = functions->tail; afterFuncList; 
// 					afterFuncList = afterFuncList->tail) {
// 					if (name == afterFuncList->head->name) {
// 						EM_error(d->pos, "two functions have the same name");
// 						break;
// 					}
// 				}

// 				/* Check fieldList */
// 				formals = NULL;
// 				formalsTail = formals;
// 				int count = 0;
// 				for (params = func->params; params; params = params->tail) {
// 					count++;
// 					field = params->head;
// 					typ = field->typ;
// 					formal = S_look(tenv, typ);
// 					if (!formal) {
// 						EM_error(d->pos, "undefined type %s", S_name(typ));
// 						formal = Ty_Int();
// 					}

// 					formal = actual_ty(formal);

// 					if (count == 1) {
// 						formals = Ty_TyList(NULL, NULL);
// 						formalsTail = formals;
// 					} else {
// 						formalsTail->tail = Ty_TyList(NULL, NULL);
// 						formalsTail = formalsTail->tail;
// 					}
// 					formalsTail->head = formal;
// 				}

// 				/* Check result */
// 				result = func->result;
// 				if (strcmp(S_name(result), "")) {
// 					resultTy = S_look(tenv, result);
// 					if (!resultTy) {
// 						EM_error(d->pos, "undefined type %s", S_name(result));
// 						resultTy = Ty_Int();
// 					}

//                 } else {
//                     resultTy = Ty_Void();
// 				}

// 				formalBools = makeFormalBoolList(func->params);
// 				S_enter(venv, name, E_FunEntry(Tr_newLevel(l, Temp_newlabel(), formalBools), 
// 					Temp_newlabel(), formals, resultTy));
// 			}

// 			/* Check all the bodies */ 
// 			Tr_accessList accesses = NULL;
// 			for (functions = get_funcdec_list(d); functions; functions = functions->tail) {
// 				func = functions->head;
// 				name = func->name;
// 				S_beginScope(venv);
//                 funEntry = S_look(venv, name);
//                 formals = get_func_tylist(funEntry);
//                 resultTy = get_func_res(funEntry);
//                 /* Put the params into venv */
//                 accesses = Tr_formals(funEntry->u.fun.level);
//                 for (params = func->params; params; params = params->tail, formals = formals->tail, accesses = accesses->tail) {
//                     field = params->head;
//                     S_enter(venv, field->name, E_VarEntry(accesses->head, formals->head));
//                 }
//                 /* Check body */
//                 exp = transExp(venv, tenv, func->body, funEntry->u.fun.level, breakLabel);

//                 /* body type should be the type of result */
//                 if (!isSameType(exp.ty, resultTy)) {
//               		if (resultTy->kind == Ty_void) {
//                    	    EM_error(func->body->pos, "procedure returns value");
//                 	} else {
//                     	EM_error(func->body->pos, "type mismatch");
// 					}
//                 }

// 				S_endScope(venv);
// 				Tr_func(exp.exp, funEntry->u.fun.level);
// 			}


// 			return Tr_nil();
// 		}

// 		case A_varDec: {
// 			/* sym : (type) = init */
// 			S_symbol var = get_vardec_var(d);
// 			S_symbol typ =  get_vardec_typ(d);
// 			A_exp init = get_vardec_init(d);

// 			struct expty exp = transExp(venv, tenv, init, l, breakLabel);
// 			Tr_access access = Tr_allocLocal(l, d->u.var.escape);
// 			/* long dec */
// 			if (strcmp(S_name(typ), "")) {
// 				/* Check whether type exists */
// 				Ty_ty ty = S_look(tenv, typ);
// 				if (!ty) {
// 					EM_error(d->pos, "undefined type %s", S_name(typ));
// 					ty = Ty_Int();
// 				}

// 				/* init type = typ */
// 				ty = actual_ty(ty);
// 				if (!isSameType(ty, exp.ty)) {
// 					EM_error(d->pos, "vardec type mismatch");
// 				}

// 				S_enter(venv, var, E_VarEntry(access, ty));
// 			} else {
// 				/* short dex */
// 				/* init should not be nil */
// 				if (get_expty_kind(exp) == Ty_nil) {
// 					EM_error(d->pos, "init should not be nil without type specified");
// 				}

// 				S_enter(venv, var, E_VarEntry(access, exp.ty));
// 			}

// 			return Tr_assign(Tr_simpleVar(access, l), exp.exp);
// 		}

// 		case A_typeDec: {
// 			/* name-sym = type-ty */
// 			A_nametyList nametys, afterNametys;
//             int maxCircle, nowCircle;
//             A_namety namety;
//             S_symbol name;
//             Ty_ty idTy, typeTy;

//             /* Put the ty_Name into tenv */
//             for (nametys = get_typedec_list(d); nametys; nametys = nametys->tail) {
//             	namety = nametys->head;
//             	name = namety->name;
//             	/* Check name duplicated */
//             	for (afterNametys = nametys->tail; afterNametys; 
//             		afterNametys = afterNametys->tail) {
//             		if (name == afterNametys->head->name) {
//             			EM_error(d->pos, "two types have the same name");
//             			break;
//             		}
//             	}

//                 S_enter(tenv, name, Ty_Name(name, NULL));
//             }

//             /* Check wrong cycle and put the real type into tenv */
//             maxCircle = 0;
//             for (nametys = get_typedec_list(d); nametys; nametys = nametys->tail) {
//             	namety = nametys->head;
//             	name = namety->name;
//                 switch(namety->ty->kind) {
//                     case A_nameTy: {
//                     	idTy = S_look(tenv, name);
//                     	if (!idTy) {
// 							EM_error(d->pos, "undefined type %s", S_name(name));
// 							idTy = Ty_Int();
// 						}
//                         idTy->u.name.sym = namety->ty->u.name;
//                         maxCircle++;
//                         break;
//                     }

//                     case A_recordTy: {
//                         idTy = S_look(tenv, name);
//                         if (!idTy) {
// 							EM_error(d->pos, "undefined type %s", S_name(name));
// 							idTy = Ty_Int();
// 						}
//                         idTy->kind = Ty_record;
//                         idTy->u.record = getTyFieldList(d->pos, tenv, namety->ty->u.record);
//                         break;
//                     }

//                     case A_arrayTy: {
//                         idTy = S_look(tenv, name);
//                         if (!idTy) {
// 							EM_error(d->pos, "undefined type %s", S_name(name));
// 							idTy = Ty_Int();
// 						}
//                         idTy->kind = Ty_array;
//                         idTy->u.array = S_look(tenv, namety->ty->u.array);
//                         if (!idTy->u.array) {
// 							EM_error(d->pos, "undefined type %s", S_name(namety->ty->u.array));
// 							idTy->u.array = Ty_Int();
// 						}
//                         break;
//                     }
//                 }
//             }

//             while (maxCircle > 0) {
//                 nowCircle = 0;
//                 for (nametys = get_typedec_list(d); nametys; nametys = nametys->tail) {
//                 	namety = nametys->head;
//             		name = namety->name;
//                     if (namety->ty->kind == A_nameTy) {
//                         idTy = S_look(tenv, name);
//                         if (!idTy) {
// 							EM_error(d->pos, "undefined type %s", S_name(name));
// 							idTy = Ty_Int();
// 						}
//                         if (!idTy->u.name.ty) {
//                             typeTy = S_look(tenv ,idTy->u.name.sym);
//                             if (!typeTy) {
// 								EM_error(d->pos, "undefined type %s", S_name(idTy->u.name.sym));
// 								typeTy = Ty_Int();
// 							}

//                             if (typeTy->kind == Ty_name) {
//                                 if (typeTy->u.name.ty) {
//                                     idTy->u.name.ty = typeTy->u.name.ty;
//                                 } else {
//                                     nowCircle++;
//                                 }
//                             } else {
//                                 idTy->u.name.ty = typeTy;
//                             }
//                         }
//                     }
//                 }

//                 if (nowCircle == maxCircle) {
//                     EM_error(d->pos, "illegal type cycle");
//                     break;
//                 }

//                 maxCircle = nowCircle;
//             }

//             /* Check record type and array type */
//             for (nametys = get_typedec_list(d); nametys; nametys = nametys->tail) {
//             	namety = nametys->head;
//             	name = namety->name;
//                 switch(namety->ty->kind) {
//                 	case A_nameTy:
//                     	break;
                	
//                 	case A_recordTy: {
//                         idTy = S_look(tenv, name);
//                         if (!idTy) {
// 							EM_error(d->pos, "undefined type %s", S_name(name));
// 							idTy = Ty_Int();
// 						}
//                         idTy->u.record = actual_tys(idTy->u.record);
//                         break;
//                     }

//                 	case A_arrayTy: {
//                         idTy = S_look(tenv, name);
//                         if (!idTy) {
// 							EM_error(d->pos, "undefined type %s", S_name(name));
// 							idTy = Ty_Int();
// 						}
//                         idTy->u.array = actual_ty(idTy->u.array);
//                         break;
//                     }
//                 }
// 			}
// 			return Tr_nil();
// 		}
// 	}
// }

// /* Help func */

// bool isSameType(Ty_ty left, Ty_ty right)
// {
// 	/* Warning: type a = {int,int} type b = {int,int} */
//     /* They are not the same type */
// 	/* array and record should check its address */
// 	/* A none-nil record equals ty_nil */
//     if (left->kind == Ty_array) {
//         if (left == right) {
//         	return TRUE;
//         }
//     } else if (left->kind == Ty_record) {
//         if (left == right || right->kind == Ty_nil) {
//         	return TRUE;
//         }
//     } else if (right->kind == Ty_record) {
//         if (left == right || left->kind == Ty_nil) {
//         	return TRUE;
//         }
//     } else {
//         if (left->kind == right->kind) {
//         	return TRUE;
//         }
//     }

//     return FALSE;
// }

// Ty_ty actual_ty(Ty_ty ty) {
//     if (ty->kind == Ty_name) {
//         return ty->u.name.ty;
//     } else {
//         return ty;
//     }
// }

// Ty_fieldList getTyFieldList(A_pos pos, S_table tenv, A_fieldList fields) {
//     if (fields) {
//     	Ty_ty ty = S_look(tenv, fields->head->typ);
// 		if (!ty) {
// 			EM_error(pos, "undefined type treelist %s", S_name(fields->head->typ));
// 			ty = Ty_Int();
// 		}
//         return Ty_FieldList(Ty_Field(fields->head->name, ty), getTyFieldList(pos, tenv, fields->tail));
//     } else {
//         return NULL;
//     }
// }

// Ty_fieldList actual_tys(Ty_fieldList fields) {
//     if (fields) {
//         return Ty_FieldList(Ty_Field(fields->head->name, actual_ty(fields->head->ty)), actual_tys(fields->tail));
//     } else {
//         return NULL;
//     }
// }

// U_boolList makeFormalBoolList(A_fieldList fields) {
// 	if (fields) {
// 		return U_BoolList(fields->head->escape, makeFormalBoolList(fields->tail));
// 	} else {
// 		return NULL;
// 	}
// }