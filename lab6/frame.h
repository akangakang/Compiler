
/*Lab5: This header file is not complete. Please finish it with more definition.*/

#ifndef FRAME_H
#define FRAME_H

#include "tree.h"
#include "assem.h"

extern const int F_wordSize;
extern const int F_formalRegNum;
extern const int F_regNum;
typedef struct F_frame_ *F_frame;

typedef struct F_access_ *F_access;
typedef struct F_accessList_ *F_accessList;
struct F_frame_
{
	T_stm shift;
	int size;		// 记录这个栈的大小
	F_accessList formals;
	Temp_label name;
};

struct F_accessList_
{
	F_access head;
	F_accessList tail;
};
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


F_accessList F_AccessList(F_access head, F_accessList tail);

F_frame F_newFrame(Temp_label name, U_boolList formals);
Temp_label F_name(F_frame f);
F_accessList F_formals(F_frame f);
F_access F_allocLocal(F_frame f, bool escape);

T_exp F_externalCall(string s, T_expList args);


T_exp F_Exp(F_access acc, T_exp framePtr);


/* declaration for fragments */
typedef struct F_frag_ *F_frag;
struct F_frag_
{
	enum
	{
		F_stringFrag,
		F_procFrag
	} kind;
	union
	{
		struct
		{
			Temp_label label;
			string str;
		} stringg;
		struct
		{
			T_stm body;
			F_frame frame;
		} proc;
	} u;
};

F_frag F_StringFrag(Temp_label label, string str);
F_frag F_ProcFrag(T_stm body, F_frame frame);

typedef struct F_fragList_ *F_fragList;
struct F_fragList_
{
	F_frag head;
	F_fragList tail;
};

F_fragList F_FragList(F_frag head, F_fragList tail);


Temp_map F_tempMap;
Temp_tempList F_registers(void);
Temp_tempList F_calleeSavedReg(void);
Temp_tempList F_callerSavedReg(void);
Temp_tempList F_paramReg(void);
Temp_map F_regTempMap();
Temp_temp F_FP(void);
Temp_temp F_SP(void);
Temp_temp F_RV(void);
Temp_temp F_RAX(void);
Temp_temp F_RDX(void);



T_stm F_procEntryExit1(F_frame frame, T_stm stm);
AS_instrList F_procEntryExit2(AS_instrList body);
AS_proc F_procEntryExit3(F_frame frame, AS_instrList body);

Temp_labelList F_preDefineFuncs();

#endif
