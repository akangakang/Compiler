#ifndef LIVENESS_H
#define LIVENESS_H


typedef struct Live_moveList_ *Live_moveList;
struct Live_moveList_ {
	G_node src, dst;
	Live_moveList tail;
};


Live_moveList Live_MoveList(G_node src, G_node dst, Live_moveList tail);

struct Live_graph {
	G_graph graph;
	Live_moveList moves;
	G_table temp_to_moves;
};
Temp_temp Live_gtemp(G_node n);

struct Live_graph Live_liveness(G_graph flow);

//-----------------set operation-------------------------
// ml是否包含指令a， 传入啊的src a的dst
bool Live_inMoveList(Live_moveList ml, G_node src,G_node dst);

// 取交集
Live_moveList Live_moveIntersect(Live_moveList a, Live_moveList b);

// 取并集
Live_moveList Live_moveUnion(Live_moveList a, Live_moveList b);

// 差集
Live_moveList Live_moveDifference(Live_moveList a, Live_moveList b);

#endif
