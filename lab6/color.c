#include <stdio.h>
#include <string.h>

#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "absyn.h"
#include "assem.h"
#include "frame.h"
#include "graph.h"
#include "liveness.h"
#include "color.h"
#include "table.h"

#define INFINITE 100000
static int K;

static Temp_tempList precolored;	// 机器寄存器
static G_nodeList simplifyWorklist; // 低度数的传送无关的结点表
static G_nodeList freezeWorklist;	// 低度数的传送有关的结点表
static G_nodeList spillWorklist;	// 高度数的结点
static G_nodeList spilledNodes;		// 在本轮中要被溢出的结点
static G_nodeList coalescedNodes;	// 已经合并的寄存器集合 当合并u<-v时，把v加入该集合
static G_nodeList coloredNodes;		// 已经成功着色的结点集合
static G_nodeList selectStack;		// 栈

static Live_moveList coalescedMoves;   // 已经合并的传送指令的集合
static Live_moveList constrainedMoves; // 源操作数和目标操作数冲突的 传送指令的集合
static Live_moveList frozenMoves;	   // 不再考虑合并的 传送指令的集合 冻住了的
static Live_moveList worklistMoves;	   // 有可能合并的传送指令的集合
static Live_moveList activeMoves;	   // 还没有做好合并准备的传送指令的集合

static G_table degree;	 // 包含每个结点当前度数的数组
static G_table color;	 // 算法为结点选择的颜色
static G_table alias;	 // 别名 alias（v）=u说明v被合并进了u
static G_table moveList; // 从一个结点到与该结点相关的 传送指令 表的映射

static G_graph interferenceGraph;

//-------------------------------------

//----------------wrap functions-------------

int getDegree(G_node n)
{
	return (int)G_look(degree, n);
}


void enterMoveListMap(G_node n, Live_moveList ml)
{
	G_enter(moveList, n, ml);
}

Live_moveList lookupMoveListMap(G_node n)
{
	return (Live_moveList)G_look(moveList, n);
}

void addEdge(G_node u, G_node v)
{
	if (!G_goesTo(u, v))
	{
		G_addEdge(u, v);
		G_addEdge(v, u);
		G_enter(degree, u, (void *)(getDegree(u) + 1));
		G_enter(degree, v, (void *)(getDegree(v) + 1));
	}
}

Live_moveList nodeMoves(G_node n)
{
	return Live_moveIntersect(lookupMoveListMap(n), Live_moveUnion(activeMoves, worklistMoves));
}

bool isMoveRelated(G_node n)
{
	return nodeMoves(n) != NULL;
}

G_nodeList adjacent(G_node n)
{
	return G_difference(G_succ(n), G_union(selectStack, coalescedNodes));
}

// 获得n合并进 的结点  n被和并进了谁
G_node getAlias(G_node n)
{
	for (G_nodeList nodes = coalescedNodes; nodes; nodes = nodes->tail)
	{
		if (n == nodes->head)
		{
			G_node a = G_look(alias, n);
			return getAlias(a);
		}
	}
	return n;
}

void assignColor(G_node n, Temp_temp t)
{
	G_enter(color, n, t);
}

Temp_temp getColor(G_node n)
{
	return (Temp_temp)G_look(color, n);
}

bool isPrecolored(G_node n)
{
	return Temp_inTempList(Live_gtemp(n), precolored);
}
//---------------------------------------------------

void build(G_graph ig, Temp_tempList regs, Live_moveList moves, G_table temp_to_moves)
{

	K = F_regNum;
	simplifyWorklist = NULL;
	freezeWorklist = NULL;
	spillWorklist = NULL;
	spilledNodes = NULL;
	coalescedNodes = NULL;
	coloredNodes = NULL;
	selectStack = NULL;

	precolored = regs;
	coalescedMoves = NULL;
	constrainedMoves = NULL;
	frozenMoves = NULL;
	worklistMoves = moves;
	activeMoves = NULL;

	degree = G_empty();
	moveList = temp_to_moves;
	alias = G_empty();
	color = G_empty();

	interferenceGraph = ig;

	G_nodeList nodes = G_nodes(ig);
	for (; nodes; nodes = nodes->tail)
	{
		G_node n = nodes->head;
		if (!isPrecolored(n))
		{
			int d = (int)G_degree(nodes->head) / 2;
			G_enter(degree, nodes->head, (void *)d);
		}
		else
		{
			G_enter(color, nodes->head, Live_gtemp(nodes->head));
			G_enter(degree, nodes->head, (void *)INFINITE);
		}
	}
}

void makeWorkList()
{
	// int simpNum=0,spillNum=0,moveNum=0;
	G_nodeList nodes = G_nodes(interferenceGraph);
	for (; nodes; nodes = nodes->tail)
	{
		G_node n = nodes->head;
		if (isPrecolored(n))
			continue;
		if (getDegree(n) >= K)
		{
			// spillNum++;
			spillWorklist = G_nodeAppend(spillWorklist, n);
		}
		else if (isMoveRelated(n))
		{
			// moveNum++;
			freezeWorklist = G_nodeAppend(freezeWorklist, n);
		}
		else
		{
			// simpNum++;
			simplifyWorklist = G_nodeAppend(simplifyWorklist, n);
		}
		// 	printf("spill num %d\n",spillNum);
		// printf("simplify num %d\n",simpNum);
		// printf("move num %d\n",moveNum);
	}
}
//---------------------------------------------------
void enableMoves(G_nodeList nl)
{
	for (; nl; nl = nl->tail)
	{
		G_node n = nl->head;
		Live_moveList ml = nodeMoves(n);
		for (; ml; ml = ml->tail)
		{
			if (Live_inMoveList(activeMoves, ml->src, ml->dst))
			{
				activeMoves = Live_moveDifference(activeMoves, Live_MoveList(ml->src, ml->dst, NULL));
				worklistMoves = Live_moveUnion(worklistMoves, Live_MoveList(ml->src, ml->dst, NULL));
			}
		}
	}
}

void decrementDegree(G_node m)
{
	int d = getDegree(m);
	G_enter(degree, m, (void *)d - 1);

	if (d == K && !isPrecolored(m))
	{
		// 如果减1后变成了低度数结点
		enableMoves(G_nodeAppend(adjacent(m), m));
		spillWorklist = G_difference(spillWorklist, G_NodeList(m, NULL));
		if (isMoveRelated(m))
		{
			freezeWorklist = G_nodeAppend(freezeWorklist, m);
		}
		else
		{
			// 如果是传送无关的则加入
			simplifyWorklist = G_nodeAppend(simplifyWorklist, m);
		}
	}
}

void simplify()
{
	G_node n = simplifyWorklist->head;
	simplifyWorklist = simplifyWorklist->tail;
	selectStack = G_NodeList(n, selectStack);
	G_nodeList nl = adjacent(n);
	for (; nl; nl = nl->tail)
	{
		G_node m = nl->head;
		decrementDegree(m);
	}
}
//---------------------------------------------------

// 把传送无管的低度数结点放进simplifyWorklist
void addWorkList(G_node u)
{
	if (!isPrecolored(u) && !isMoveRelated(u) && getDegree(u) < K)
	{
		freezeWorklist = G_difference(freezeWorklist, G_NodeList(u, NULL));
		simplifyWorklist = G_union(simplifyWorklist, G_NodeList(u, NULL));
	}
}

bool OK(G_node t, G_node v)
{
	bool adj = G_isAdj(t, v);
	return getDegree(t) < K || isPrecolored(t) || adj;
}

// 高度数结点数是不是小于K个  实现保守合并 启发式的函数
bool conservative(G_nodeList nl)
{
	int k = 0;
	for (; nl; nl = nl->tail)
	{
		G_node node = nl->head;
		if (getDegree(node) >= K)
		{
			k = k + 1;
		}
	}
	return k < K;
}

// 把u v两个结点合并
void combine(G_node u, G_node v)
{
	if (G_inNodeList(v, freezeWorklist))
	{
		// 如果v被冻住了 取消
		freezeWorklist = G_difference(freezeWorklist, G_NodeList(v, NULL));
	}
	else
	{
		// 如果v溢出了 取消
		spillWorklist = G_difference(spillWorklist, G_NodeList(v, NULL));
	}
	coalescedNodes = G_nodeAppend(coalescedNodes, v);
	G_enter(alias, v, u);

	Live_moveList uml = lookupMoveListMap(u);
	Live_moveList vml = lookupMoveListMap(v);
	enterMoveListMap(u, Live_moveUnion(uml, vml));
	enableMoves(G_NodeList(v, NULL));
	G_nodeList nl = adjacent(v);
	for (; nl; nl = nl->tail)
	{
		G_node t = nl->head;
		addEdge(t, u);
		decrementDegree(t);
	}
	if (getDegree(u) >= K && G_inNodeList(u, freezeWorklist))
	{
		freezeWorklist = G_difference(freezeWorklist, G_NodeList(u, NULL));
		spillWorklist = G_union(spillWorklist, G_NodeList(u, NULL));
	}
}

// 合并阶段只考虑worklistMoves中的传送指令 一次就合并一个
void coalesce()
{
	assert(worklistMoves != NULL);

	// x---> y

	// Live_move m = worklistMoves->head;
	G_node xx = worklistMoves->src;
	G_node yy = worklistMoves->dst;
	G_node x = getAlias(xx);
	G_node y = getAlias(yy);

	G_node u, v;
	if (isPrecolored(y))
	{
		// 如果y是precolor 只能把别人并进他 不能被并进别人
		u = y;
		v = x;
	}
	else
	{
		u = x;
		v = y;
	}
	worklistMoves = worklistMoves->tail;
	bool adj = G_inNodeList(v, G_succ(u));
	if (u == v)
	{
		// 如果被并进x y的结点是一样的  说明x y已经合并了
		coalescedMoves = Live_moveUnion(coalescedMoves, Live_MoveList(x, y, NULL));
		addWorkList(u);
	}
	else if (isPrecolored(v) || adj)
	{
		constrainedMoves = Live_moveUnion(constrainedMoves, Live_MoveList(x, y, NULL));
		addWorkList(u);
		addWorkList(v);
	}
	else
	{
		G_nodeList nl = adjacent(v);
		bool flag = TRUE;
		for (; nl; nl = nl->tail)
		{
			G_node t = nl->head;
			if (!OK(t, u))
			{
				flag = FALSE;
				break;
			}
		}
		if (isPrecolored(u) && flag || !isPrecolored(u) && conservative(G_union(adjacent(u), adjacent(v))))
		{
			// 两种合并算法
			coalescedMoves = Live_moveUnion(coalescedMoves, Live_MoveList(x, y, NULL));
			combine(u, v);
			addWorkList(u);
		}
		else
		{
			activeMoves = Live_moveUnion(activeMoves, Live_MoveList(x, y, NULL));
		}
	}
}
//---------------------------------------------------

// 把n相关的传送指令都冻住
void freezeMoves(G_node u)
{
	Live_moveList ml = nodeMoves(u);
	for (; ml; ml = ml->tail)
	{

		G_node x = ml->src;
		G_node y = ml->dst;

		G_node v;
		if (getAlias(y) == getAlias(u))
		{
			v = getAlias(x);
		}
		else
		{
			v = getAlias(y);
		}

		activeMoves = Live_moveDifference(activeMoves, Live_MoveList(ml->src, ml->dst, NULL));
		frozenMoves = Live_moveUnion(frozenMoves, Live_MoveList(ml->src, ml->dst, NULL));

		if (!nodeMoves(v) && getDegree(v) < K)
		{
			freezeWorklist = G_difference(freezeWorklist, G_NodeList(v, NULL));
			simplifyWorklist = G_union(simplifyWorklist, G_NodeList(v, NULL));
		}
	}
}

// 冻结传送指令 冻结第一个结点
void freeze()
{
	G_node u = freezeWorklist->head;
	freezeWorklist = freezeWorklist->tail;
	// 放弃合并 把他加入传送无关
	simplifyWorklist = G_union(simplifyWorklist, G_NodeList(u, NULL));
	freezeMoves(u);
}

// 选度数最大的
void selectSpill()
{
	int max = -1;
	G_node m = NULL;
	for (G_nodeList nl = spillWorklist; nl; nl = nl->tail)
	{
		G_node n = nl->head;
		assert(!isPrecolored(n));

		if (getDegree(n) > max)
		{
			max = getDegree(n);
			m = n;
		}
	}
	
	spillWorklist = G_difference(spillWorklist, G_NodeList(m, NULL));
	simplifyWorklist = G_nodeAppend(simplifyWorklist, m);
	freezeMoves(m);
}
//---------------------------------------------------
void assignColors()
{

	while (selectStack)
	{
		G_node n = selectStack->head;
		selectStack = selectStack->tail;

		if (G_inNodeList(n, coloredNodes))
			continue;

		// 可以用的颜色
		Temp_tempList okColors = precolored;
		G_nodeList adj = G_succ(n);
		for (; adj; adj = adj->tail)
		{
			G_node w = getAlias(adj->head);
			if (G_inNodeList(w, coloredNodes) || isPrecolored(w))
			{
				Temp_tempList res = NULL;
				// 如果相邻的点已经图色，则okcolors去掉这个颜色
				while (okColors)
				{
					if (okColors->head != getColor(w))
					{
						res = Temp_TempList(okColors->head, res);
						okColors = okColors->tail;
					}
					else
					{
						// debug ： 死循环！
						okColors = okColors->tail;
					}
				}
				okColors = res;
			}
		}
		if (!okColors)
		{
			spilledNodes = G_nodeAppend(spilledNodes, n);
		}
		else
		{
			coloredNodes = G_nodeAppend(coloredNodes, n);
			assignColor(n, okColors->head);
		}
	}
	G_nodeList nl = coalescedNodes;

	// 把被并进他的结点都涂色了
	for (; nl; nl = nl->tail)
	{
		G_node n = nl->head;
		assignColor(n, getColor(getAlias(n)));
	}
}

struct COL_result COL_color(G_graph ig, Temp_map initial, Temp_tempList regs, Live_moveList moves, G_table temp_to_moves)
{
	struct COL_result ret;
	build(ig, regs, moves, temp_to_moves);
	makeWorkList();

	do
	{
		if (simplifyWorklist)
		{
			// printf("begin Simplify\n");
			simplify();
		}
		else if (worklistMoves)
		{
			// printf("begin coalesce\n");
			coalesce();
		}
		else if (freezeWorklist)
		{
			// printf("begin Freeze\n");
			freeze();
		}
		else if (spillWorklist)
		{
			// printf("begin SelectSpill\n");
			selectSpill();
		}
	} while (simplifyWorklist || worklistMoves || freezeWorklist || spillWorklist);
	assignColors();

	if (!spilledNodes)
	{
		// printf("bsgin ！spilledNodes1\n");
		Temp_map coloring = Temp_empty();
		G_nodeList nl = G_nodes(interferenceGraph);
		for (; nl; nl = nl->tail)
		{
			G_node n = nl->head;
			Temp_temp color = getColor(n);
			if (color)
			{
				Temp_enter(coloring, Live_gtemp(n), Temp_look(initial, color));
			}
		}
		ret.coloring = Temp_layerMap(coloring, initial);
	}

	Temp_tempList spills = NULL;
	for (G_nodeList nl = spilledNodes; nl; nl = nl->tail)
	{
		G_node n = nl->head;
		spills = Temp_TempList(Live_gtemp(n), spills);
	}
	ret.spills = spills;

	return ret;
}
