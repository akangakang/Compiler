#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "absyn.h"
#include "assem.h"
#include "frame.h"
#include "graph.h"
#include "flowgraph.h"
#include "errormsg.h"
#include "table.h"

// 语句n def了什么变量
Temp_tempList FG_def(G_node n)
{
	AS_instr instr = (AS_instr)G_nodeInfo(n);
	switch(instr->kind) {
		case I_OPER:
			return instr->u.OPER.dst;
		case I_LABEL:
			return NULL;
		case I_MOVE:
			return instr->u.MOVE.dst;
		default:
			assert(0);
	}
	// return NULL;
}

// 语句n use了什么变量
Temp_tempList FG_use(G_node n)
{
	AS_instr instr = G_nodeInfo(n);
	switch(instr->kind) {
		case I_OPER:
			return instr->u.OPER.src;
		case I_LABEL:
			return NULL;
		case I_MOVE:
			return instr->u.MOVE.src;
		default:
			assert(0);
	}
}

// 语句n 是不是 传送指令
bool FG_isMove(G_node n)
{
	AS_instr inst = (AS_instr)G_nodeInfo(n);
	return inst->kind == I_MOVE;
}

// 得到控制流图
G_graph FG_AssemFlowGraph(AS_instrList il)
{
	G_graph g = G_Graph();
	TAB_table t = TAB_empty();

	G_node prev = NULL;

	for (AS_instrList i = il; i; i = i->tail)
	{
		AS_instr inst = i->head;

		// 为这个语句创建一个图结点
		G_node node = G_Node(g, (void *)inst);
		if (prev)
		{
			G_addEdge(prev, node);
		}
		if (inst->kind == I_OPER && strncmp("\tjmp", inst->u.OPER.assem, 4) == 0)
		{
			// 如果这个语句是跳转  那他的下一个语句的prev
			prev = NULL;
		}
		else
		{
			prev = node;
		}
		if (inst->kind == I_LABEL)
		{
			TAB_enter(t, inst->u.LABEL.label, node);
		}
	}

	// 把跳转的边加上
	G_nodeList nl = G_nodes(g);
	for (; nl; nl = nl->tail)
	{
		G_node node = nl->head;
		AS_instr inst = (AS_instr)G_nodeInfo(node);

		if (inst->kind == I_OPER && inst->u.OPER.jumps)
		{
			Temp_labelList targets = inst->u.OPER.jumps->labels;
			for (; targets; targets = targets->tail)
			{
				G_node t_node = (G_node)TAB_look(t, targets->head);
				if (t_node)
				{
					G_addEdge(node, t_node);
				}
			}
		}
	}

	return g;
}
