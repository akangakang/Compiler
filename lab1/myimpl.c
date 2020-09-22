#include "prog1.h"
#include <stdio.h>
typedef struct table* Table_;
// typedef struct IntAndTable* IntAndTable_;
struct table
{
	string id;
	int value;
	Table_ tail;
};

struct IntAndTable{
	int i;
	Table_ t;
};
struct IntAndTable IntAndTable_c(int i,struct table* t);
Table_ interpStm(A_stm s,Table_ t);
void interpExpList(A_expList exps,Table_ t);
struct IntAndTable interpExp(A_exp e,Table_ t);
Table_ update(Table_ t,string id, int val);
int lookup(Table_ t,string key);
int maxargs(A_stm stm);
int parse_stm(A_stm stm);
int parse_exp(A_exp exp);
int parse_expList(A_expList exps);

struct IntAndTable IntAndTable_c(int i,struct table* t)
{
	// IntAndTable_ it=malloc(sizeof(*it));
	struct IntAndTable it;
	it.i=i;
	it.t=t;
	return it;
}

Table_ Table (string id,int value,Table_ tail)
{
	Table_ t=checked_malloc(sizeof(*t));
	// printf("!");
	// printf("%d \n",value);
	t->id=id;
	t->value=value;
	t->tail=tail;
	return t;
}

Table_ interpStm(A_stm s,Table_ t)
{

	if(s->kind==A_compoundStm)
	{
		// if(t!=NULL)
		// {
		// 	printf("com t: %d \n",t->value);
		// }
		Table_ t1=interpStm(s->u.compound.stm1,t);
		// printf("# %d \n",t1->value);
		Table_ t2=interpStm(s->u.compound.stm2,t1);
		
		return t2;
	}

	if(s->kind==A_assignStm)
	{
		// if(t!=NULL)
		// {
		// 	printf("assign t: %d \n",t->value);
		// }
		// printf("assign %s \n",s->u.assign.id);
		struct IntAndTable it = interpExp(s->u.assign.exp,t);
		Table_ t1=update(it.t,s->u.assign.id,it.i);
		// printf("new table after assign %d \n",t1->value);
		return t1;
	}

	if(s->kind==A_printStm)
	{
		// if(t!=NULL)
		// {
		// 	printf("print t: %d \n",t->value);
		// }
		interpExpList(s->u.print.exps,t);
		printf("\n");
		return t;
	}

}

void interpExpList(A_expList exps,Table_ t)
{
	if(exps->kind==A_pairExpList)
	{
	// 	if(t!=NULL)
	// 	{
	// 		printf("pairExpList t: %d \n",t->value);
	// 	}
		struct IntAndTable it=interpExp(exps->u.pair.head,t);
		printf("%d ",it.i);
		interpExpList(exps->u.pair.tail,it.t);
	}

	if(exps->kind==A_lastExpList)
	{
		// if(t!=NULL)
		// {
		// 	printf("lastExpList t: %d \n",t->value);
		// }
		struct IntAndTable it=interpExp(exps->u.last,t);
		printf("%d ",it.i);
	}

}
struct IntAndTable interpExp(A_exp e,Table_ t){
	if(e->kind==A_idExp)
	{
		// printf("!! %d \n",t->value );
		// if(t!=NULL)
		// {
			
		// 	printf("idExp t: %d \n",t->value);
		// }
		struct IntAndTable it;
		it.i=lookup(t,e->u.id);
		it.t=t;
		// printf("id: %s value: %d: \n",e->u.id,it.i);
		return it;
	}

	if(e->kind==A_numExp)
	{

		// printf("num : %d \n",e->u.num);
		struct IntAndTable it;
		it.i=e->u.num;
		it.t=t;
		// if(t!=NULL)
		// {
			
		// 	printf("numExp it.t: %d \n",it.t->value);
		// }
		return it;
	}

	if(e->kind == A_opExp)
	{
		struct IntAndTable it1=interpExp(e->u.op.left,t);
		struct IntAndTable it2=interpExp(e->u.op.right,t);
		if(e->u.op.oper==A_plus)
		{
			// printf("%d + %d \n",it1.i,it2.i);
			struct IntAndTable res;
			res.i=it1.i+it2.i;
			res.t=it2.t;
			// struct IntAndTable res=IntAndTable_c(it1.i+it2.i,it2.t);
			return res;
		}
		else if(e->u.op.oper==A_minus)
		{
			// printf("%d - %d \n",it1.i,it2.i);
			// struct IntAndTable res=IntAndTable_c(it1.i-it2.i,it2.t);
			struct IntAndTable res;
			res.i=it1.i-it2.i;
			res.t=t;
			return res;
		}
		else if(e->u.op.oper==A_times)
		{
			// printf("%d * %d \n",it1.i,it2.i);
			// struct IntAndTable res=IntAndTable_c(it1.i*it2.i,it2.t);
			struct IntAndTable res;
			res.i=it1.i*it2.i;
			res.t=t;
			return res;
		}
		else if(e->u.op.oper==A_div)
		{
			// printf("%d / %d \n",it1.i,it2.i);
			// struct IntAndTable res=IntAndTable_c(it1.i/it2.i,it2.t);
			struct IntAndTable res;
			res.i=it1.i/it2.i;
			res.t=t;
			return res;
		}

	}
	if(e->kind==A_eseqExp)
	{
		// if(t!=NULL)
		// {
			
		// 	printf("eseqExp t: %d \n",t->value);
		// }
		Table_ t1=interpStm(e->u.eseq.stm,t);
		// if(t1!=NULL)
		// {
			
		// 	printf("eseqExp t1: %d \n",t1->value);
		// }
		struct IntAndTable res=interpExp(e->u.eseq.exp,t1);
		return res;
		
	}


}

Table_ update(Table_ t,string id, int val)
{
	Table_ t1=Table(id,val,t);
	// printf("update %d \n",t1->value);
	return t1;

}

int lookup(Table_ t,string key)
{
	Table_ tmp=t;
	if(t==NULL) return 0;
	
	// printf("%d",t->value);
	while ( ! (*key == *(tmp->id)) && tmp->tail != NULL && tmp!=NULL)  
	{
		tmp=tmp->tail;
	}
 return tmp->value;

}

int maxargs(A_stm stm)
{
	//TODO: put your code here.
   
	return parse_stm(stm);
}

int parse_stm(A_stm stm)
{
	int sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
	if (stm->kind == A_printStm)
	{
		sum1 = parse_expList((stm->u).print.exps);
	}
	else if (stm->kind == A_compoundStm)
	{
		sum2 = parse_stm(stm->u.compound.stm1);
		sum3 = parse_stm(stm->u.compound.stm2);
	}
	else
	{
		sum4 = parse_exp(stm->u.assign.exp);
	}

	if(sum1>=sum2 && sum1 >=sum3 && sum1>=sum4) return sum1;
	if(sum2>=sum1 && sum2 >=sum3 && sum2>=sum4) return sum2;
	if(sum3>=sum2 && sum3 >=sum1 && sum3>=sum4) return sum3;
	if(sum4>=sum2 && sum4 >=sum3 && sum4>=sum1) return sum4;
}
int parse_exp(A_exp exp)
{
	if (exp->kind == A_eseqExp)
	{
		int sum1 = parse_stm(exp->u.eseq.stm);
		int sum2 = parse_exp(exp->u.eseq.exp);
		return sum1 > sum2 ? sum1 : sum2;
	}
	else
	{
		return 0;
	}
}
/*
 * calculate the arg of print
 */
int parse_expList(A_expList exps)
{
	if (exps->kind == A_lastExpList)
	{
		return 1;
	}
	else if (exps->kind == A_pairExpList)
	{
		int sum = 1;
		sum += parse_expList(exps->u.pair.tail);
		return sum;
	}
	else
	{
		return 0;
	}
}
void interp(A_stm stm)
{
	//TODO: put your code here.
    interpStm(stm,NULL);
}
