#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "absyn.h"
#include "assem.h"
#include "frame.h"
#include "graph.h"
#include "flowgraph.h"
#include "liveness.h"
#include "table.h"

Live_moveList Live_MoveList(G_node src, G_node dst, Live_moveList tail)
{
	Live_moveList lm = (Live_moveList)checked_malloc(sizeof(*lm));
	lm->src = src;
	lm->dst = dst;
	lm->tail = tail;
	return lm;
}

Temp_temp Live_gtemp(G_node n)
{
	return (Temp_temp)G_nodeInfo(n);
}

static void enterLiveMap(G_table t, G_node flowNode, Temp_tempList temps)
{
	G_enter(t, flowNode, temps);
}

static void enterMoveMap(G_table t, G_node src, G_node dst)
{
	G_enter(t, src, Live_MoveList(src, dst, (Live_moveList)G_look(t, src)));
	G_enter(t, dst, Live_MoveList(src, dst,(Live_moveList)G_look(t, dst)));
}


G_table buildLiveMap(G_graph flow)
{
	G_table in_set_table = G_empty();
	G_table out_set_table = G_empty();
	

	// temp_to_move 是一个表格 记录了 各个图中的点 和他的此昂管传送 src->dst
	G_table temp_to_moves = G_empty();
	// 如果某次循环后def-use-in-out表没有变化就可以终止了
	bool has_change = TRUE;

	// ml 是一个链表 记录了所有传送 src->dst
	Live_moveList ml = NULL;
	// 获得def-use-in-out表
	while (has_change)
	{
		has_change = FALSE;
		// 获得图的结点表
		G_nodeList flownodes = G_nodes(flow);
		for (; flownodes; flownodes = flownodes->tail)
		{
			Temp_tempList old_in_set = (Temp_tempList)G_look(in_set_table,flownodes->head);
			Temp_tempList old_out_set = (Temp_tempList)G_look(out_set_table,flownodes->head);
			Temp_tempList use_set = FG_use(flownodes->head);
			Temp_tempList def_set = FG_def(flownodes->head);

			Temp_tempList new_out_set = NULL;

			for (G_nodeList nodes = G_succ(flownodes->head); nodes; nodes = nodes->tail)
			{
				new_out_set = Temp_union(new_out_set, (Temp_tempList)G_look(in_set_table,nodes->head));
			}

			Temp_tempList new_in_set = Temp_union(use_set, Temp_difference(new_out_set, def_set));

			if (!Temp_equalTempList(old_in_set, new_in_set))
			{
				has_change = TRUE;
				enterLiveMap(in_set_table, flownodes->head, new_in_set);
			}

			if (!Temp_equalTempList(old_out_set, new_out_set))
			{
				has_change = TRUE;
				enterLiveMap(out_set_table, flownodes->head, new_out_set);
			}
		}
	}
	return out_set_table;
}

struct Live_graph Live_liveness(G_graph flow)
{
	G_nodeList nodes = G_nodes(flow);
	struct Live_graph lg;

	G_graph g = G_Graph();
	Live_moveList ml = NULL;
	G_table temp_to_moves = G_empty();

	G_table liveout = buildLiveMap(flow);


	// temp_to_node 表就是记录图中的点和变量的映射关系的表
	TAB_table temp_to_node = TAB_empty();

	// 一个记录了所有要加入图的变量的链表 
	Temp_tempList added_temps = NULL;

	// nodes 是控制流图里面的结点 也就是语句
	for (G_nodeList nl = nodes; nl; nl = nl->tail)
	{
		G_node node = nl->head;
		Temp_tempList def = FG_def(node);
		Temp_tempList use = FG_use(node);
		Temp_tempList to_add = Temp_union(def, use);
		for (Temp_tempList tl = to_add; tl; tl = tl->tail)
		{
			Temp_temp t = tl->head;
			// 没有重复才加入
			if (!Temp_inTempList(t,added_temps))
			{
				TAB_enter(temp_to_node, t, G_Node(g, t));
				added_temps = Temp_TempList(t, added_temps);
			}
		}
	}
	// 添加冲突边
	for (G_nodeList nl = nodes; nl; nl = nl->tail)
	{
		G_node node = nl->head;
		Temp_tempList def = FG_def(node);
		Temp_tempList out = (Temp_tempList)G_look(liveout,node);
		Temp_tempList conflict = out;

		//如果是传送指令 则def和out-use冲突   否则def和out冲突
		if (FG_isMove(node))
		{
			Temp_tempList use = FG_use(node);
			G_node src = (G_node)TAB_look(temp_to_node, use->head);
			G_node dst = (G_node)TAB_look(temp_to_node, def->head);
			ml = Live_MoveList(src, dst, ml);
			enterMoveMap(temp_to_moves, src, dst);

			conflict = Temp_difference(out, use);
		}

		// 把def和有冲突的点加上冲突边
		for (Temp_tempList tl = def; tl; tl = tl->tail)
		{
			Temp_temp td = tl->head;
			for (Temp_tempList tll = conflict; tll; tll = tll->tail)
			{
				Temp_temp tc = tll->head;
				if (td == tc)
					continue;
				G_node td_node = (G_node)TAB_look(temp_to_node, td);
				G_node tc_node = (G_node)TAB_look(temp_to_node, tc);
				G_addEdge(td_node, tc_node);
				G_addEdge(tc_node, td_node);
			}
		}
	}

	lg.graph = g;
	lg.moves = ml;
	lg.temp_to_moves = temp_to_moves;

	return lg;
}

//--------------set operation------------------------

// 取交集
Live_moveList Live_moveIntersect(Live_moveList a, Live_moveList b) {
	Live_moveList res = NULL;
	for (; a; a=a->tail) {
		if (Live_inMoveList(b, a->src,a->dst)) {
			res = Live_MoveList(a->src, a->dst,res);
		}
	}
	return res;
}

// 取并集
Live_moveList Live_moveUnion(Live_moveList a, Live_moveList b) {
	Live_moveList res = NULL;
	for(Live_moveList moves1 = a; moves1; moves1 = moves1->tail) {
		res = Live_MoveList(moves1->src, moves1->dst, res);
	}

	for(Live_moveList moves2 = b; moves2; moves2 = moves2->tail) {
		
		if(!Live_inMoveList(res,moves2->src,moves2->dst)) {
			res = Live_MoveList(moves2->src, moves2->dst, res);
		}
	}
	return res;
}


// ml是否包含指令a， 传入啊的src a的dst
bool Live_inMoveList(Live_moveList ml, G_node src,G_node dst) {
	for (; ml; ml=ml->tail) {
		if (ml->src == src && ml->dst == dst) {
			return TRUE;
		}
	}
	return FALSE;
}

// 差集
Live_moveList Live_moveDifference(Live_moveList a, Live_moveList b)
{
	Live_moveList res = NULL;
	for(Live_moveList moves1 = a; moves1; moves1 = moves1->tail) {
		
		
		if(!Live_inMoveList(b,moves1->src,moves1->dst)) {
			res = Live_MoveList(moves1->src, moves1->dst, res);
		}
	}
	return res;
}