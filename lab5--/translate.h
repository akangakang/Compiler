#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "util.h"
#include "absyn.h"
#include "temp.h"
#include "frame.h"

/* Lab5: your code below */

typedef struct Tr_exp_ *Tr_exp;
typedef struct Tr_expList_ *Tr_expList;
typedef struct Tr_access_ *Tr_access;

typedef struct Tr_accessList_ *Tr_accessList;
struct Tr_accessList_
{
	Tr_access head;
	Tr_accessList tail;
};
typedef struct Tr_level_ *Tr_level;

Tr_accessList Tr_AccessList(Tr_access head, Tr_accessList tail);

Tr_level Tr_outermost(void);

Tr_level Tr_newLevel(Tr_level parent, Temp_label name, U_boolList formals);

Tr_accessList Tr_formals(Tr_level level);

Tr_access Tr_allocLocal(Tr_level level, bool escape);

Tr_expList Tr_ExpList(Tr_exp head, Tr_expList tail);
Tr_access Tr_Access(Tr_level level, F_access access);
Tr_accessList Tr_AccessList(Tr_access head, Tr_accessList tail);


Tr_exp Tr_nil();
Tr_exp Tr_int(int i);
Tr_exp Tr_string(string s);
Tr_exp Tr_call(Tr_level callee, Temp_label label, Tr_expList params, Tr_level caller);
Tr_exp Tr_opA(A_oper oper, Tr_exp left, Tr_exp right);
Tr_exp Tr_opL(A_oper oper, Tr_exp left, Tr_exp right, bool isStrCmp);
Tr_exp Tr_record(int n, Tr_expList fields);
Tr_exp Tr_seq(Tr_exp e1, Tr_exp e2);
Tr_exp Tr_assign(Tr_exp var, Tr_exp exp);
Tr_exp Tr_ifThen(Tr_exp test, Tr_exp then);
Tr_exp Tr_ifThenElse(Tr_exp test, Tr_exp then, Tr_exp elsee);
Tr_exp Tr_while(Tr_exp test, Tr_exp body, Temp_label done);
Tr_exp Tr_for(Tr_access access, Tr_level level, Tr_exp lo, Tr_exp hi, Tr_exp body, Temp_label done);
Tr_exp Tr_break(Temp_label done);
Tr_exp Tr_array(Tr_exp size, Tr_exp init);
Tr_exp Tr_simpleVar(Tr_access access, Tr_level level);
Tr_exp Tr_fieldVar(Tr_exp addr, int n);
Tr_exp Tr_subscriptVar(Tr_exp addr, Tr_exp off);
void Tr_func(Tr_exp body, Tr_level level);

T_stm Tr_procEntryExit(Tr_level level, Tr_exp body, Tr_accessList formals);
F_fragList Tr_getResult(void);
#endif
