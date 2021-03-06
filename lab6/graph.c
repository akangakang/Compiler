/*
 * graph.c - Functions to manipulate and create control flow and
 *           interference graphs.
 */

#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "absyn.h"
#include "assem.h"
#include "frame.h"
#include "graph.h"
#include "errormsg.h"
#include "table.h"

struct G_graph_
{
  int nodecount;
  G_nodeList mynodes, mylast;
};

struct G_node_
{
  G_graph mygraph;
  int mykey;
  G_nodeList succs;
  G_nodeList preds;
  void *info;
};

G_graph G_Graph(void)
{
  G_graph g = (G_graph)checked_malloc(sizeof *g);
  g->nodecount = 0;
  g->mynodes = NULL;
  g->mylast = NULL;
  return g;
}

G_nodeList G_NodeList(G_node head, G_nodeList tail)
{
  G_nodeList n = (G_nodeList)checked_malloc(sizeof *n);
  n->head = head;
  n->tail = tail;
  return n;
}

/* generic creation of G_node */
G_node G_Node(G_graph g, void *info)
{
  G_node n = (G_node)checked_malloc(sizeof *n);
  G_nodeList p = G_NodeList(n, NULL);
  assert(g);
  n->mygraph = g;
  n->mykey = g->nodecount++;

  if (g->mylast == NULL)
    g->mynodes = g->mylast = p;
  else
    g->mylast = g->mylast->tail = p;

  n->succs = NULL;
  n->preds = NULL;
  n->info = info;
  return n;
}

G_nodeList G_nodes(G_graph g)
{
  assert(g);
  return g->mynodes;
}

G_nodeList G_rnodes(G_graph g)
{
  assert(g);
  G_nodeList nl = NULL;
  for (G_nodeList nodes = g->mynodes; nodes; nodes = nodes->tail)
  {
    nl = G_NodeList(nodes->head, nl);
  }
  return nl;
}

/* return true if a is in l list */
bool G_inNodeList(G_node a, G_nodeList l)
{
  G_nodeList p;
  for (p = l; p != NULL; p = p->tail)
    if (p->head == a)
      return TRUE;
  return FALSE;
}

void G_addEdge(G_node from, G_node to)
{
  assert(from);
  assert(to);
  assert(from->mygraph == to->mygraph);
  if (G_goesTo(from, to))
    return;
  to->preds = G_NodeList(from, to->preds);
  from->succs = G_NodeList(to, from->succs);
}

static G_nodeList delete (G_node a, G_nodeList l)
{
  assert(a && l);
  if (a == l->head)
    return l->tail;
  else
    return G_NodeList(l->head, delete (a, l->tail));
}

void G_rmEdge(G_node from, G_node to)
{
  assert(from && to);
  to->preds = delete (from, to->preds);
  from->succs = delete (to, from->succs);
}

/**
  * Print a human-readable dump for debugging.
  */
void G_show(FILE *out, G_nodeList p, void showInfo(FILE *, void *))
{
  for (; p != NULL; p = p->tail)
  {
    G_node n = p->head;
    G_nodeList q;
    assert(n);
    if (showInfo)
      showInfo(out, n->info);
    fprintf(out, " (%d): ", n->mykey);
    for (q = G_succ(n); q != NULL; q = q->tail)
      fprintf(out, "%d ", q->head->mykey);
    fprintf(out, "\n");
  }
}

G_nodeList G_succ(G_node n)
{
  assert(n);
  return n->succs;
}

G_nodeList G_pred(G_node n)
{
  assert(n);
  return n->preds;
}

bool G_goesTo(G_node from, G_node n)
{
  return G_inNodeList(n, G_succ(from));
}

/* return length of predecessor list for node n */
static int inDegree(G_node n)
{
  int deg = 0;
  G_nodeList p;
  for (p = G_pred(n); p != NULL; p = p->tail)
    deg++;
  return deg;
}

/* return length of successor list for node n */
static int outDegree(G_node n)
{
  int deg = 0;
  G_nodeList p;
  for (p = G_succ(n); p != NULL; p = p->tail)
    deg++;
  return deg;
}

int G_degree(G_node n) { return inDegree(n) + outDegree(n); }

/* put list b at the back of list a and return the concatenated list */
static G_nodeList cat(G_nodeList a, G_nodeList b)
{
  if (a == NULL)
    return b;
  else
    return G_NodeList(a->head, cat(a->tail, b));
}

/* create the adjacency list for node n by combining the successor and 
 * predecessor lists of node n */
G_nodeList G_adj(G_node n) { return cat(G_succ(n), G_pred(n)); }
bool G_isAdj(G_node u, G_node v)
{
  assert(u);
  assert(v);
  assert(u->mygraph == v->mygraph);
  return (G_inNodeList(u, v->preds) || G_inNodeList(u, v->succs));
}
void *G_nodeInfo(G_node n) { return n->info; }

/* G_node table functions */

G_table G_empty(void)
{
  return TAB_empty();
}

void G_enter(G_table t, G_node node, void *value)
{
  TAB_enter(t, node, value);
}

void *G_look(G_table t, G_node node)
{
  return TAB_look(t, node);
}

//---------------set operation------------------


G_nodeList G_nodeRemove(G_nodeList nl, G_node n)
{
  G_nodeList prev = NULL;
  G_nodeList origin = nl;
  for (; nl; nl = nl->tail)
  {
    if (nl->head == n)
    {
      if (prev)
      {
        prev->tail = nl->tail;
        return origin;
      }
      else
      {
        return nl->tail;
      }
    }
    prev = nl;
  }
  return origin;
}

G_nodeList G_nodeAppend(G_nodeList nl, G_node n)
{
  if (G_inNodeList(n, nl))
  {
    return nl;
  }
  else
  {
    return G_NodeList(n, nl);
  }
}

// 把a b合并 去重
G_nodeList G_union(G_nodeList a, G_nodeList b)
{
  // printf("G_union-1\n");
  G_nodeList res = NULL;

  //  printf("G_union-3\n");
  // 把a的结点放进最终结果
  for (G_nodeList nodes1 = a; nodes1; nodes1 = nodes1->tail)
  {
    res = G_NodeList(nodes1->head, res);
  }

  //  printf("G_union-4\n");
  // 把a和b串起来 去掉重复的
  for (G_nodeList nodes2 = b; nodes2; nodes2 = nodes2->tail)
  {
    // debug ：反了
    if (!G_inNodeList(nodes2->head, res))
    {
      res = G_NodeList(nodes2->head, res);
    }
  }
  //  printf("G_union-5\n");
  return res;
}

// 返回a b中不同的结点
G_nodeList G_difference(G_nodeList a, G_nodeList b)
{
  G_nodeList res = NULL;
  for (G_nodeList nodes1 = a; nodes1; nodes1 = nodes1->tail)
  {

    if (!G_inNodeList(nodes1->head, b))
    {
      res = G_NodeList(nodes1->head, res);
    }
  }
  return res;
}