#pragma once
#include <iostream>

/** 图的基本概念：
1. 图:通常用一个二元组表示：G<V，E>，V(edge)表示顶点集，E(vertex)表示边集。
	|V|表示顶点集中元素的个数，即顶点数，也称为图G的阶，例如n阶图，表示图中有n个顶点。
	|E|表示边集中元素的个数，即边数。
	一个图至少有一个顶点；

	子图：从图中选择若干个顶点、若干条边构成的图称为原图的子图。
	生成子图：从图中选择所有顶点，若干条边构成的图称为原图的生成子图。

	路径：接续的边的顶点构成的序列。
	路径长度：路径上边或弧的数目。.
	距离：从顶点到另一顶点的最短路径长度。

2. 无向图：图G中每条边都是没有方向的，每条边都是两个顶点组成的无序对，记为（Vi,Vj）；
3. 有向图：图G中每条边都是有方向的,每条边(称为弧)都是两个顶点组成的有序对，记为<Vi,Vj>，(Vi称为弧头，Vj称为弧尾)
4. 简单图：既不含平行边也不含自环的图。
	无向图中，关联一对顶点的无向边多于一条，则称这些边为平行边，平行边的条数称为重数。
	有向图中，关联一对顶点的有向边多于一条，并且这些边的始点和终点相同（方向一致），则称这些边为平行边。
5. 自环：是指一条边关联的两个顶点为同一个顶点，也就是说自己到自己有一条边。
6. 多重图：含有平行边或自环的图。
7. 稀疏图和稠密图: 有很少边或弧的图称为稀疏图，反之，称为稠密图。
	这是一个非常模糊的概念，一般来说，若图G满足|E| < |V| x log|V|，则称G为稀疏图。

8. 邻接:是指顶点和顶点之间的关系，如无向边（vi，vj，则称vi和vj互为邻接点, 有向边<vi，vj>，则称vi邻接到vj，vj邻接于vi。
9. 关联:是指边和顶点之间的关系。若存在（vi，vj）或<vi，vj>，则称该边或弧关联于vi和vj。
10 顶点的度: 是指与该顶点相关联的边的数目，记为TD（v）.
	握手定理：度数之和等于边数的两倍。

11. 连通图：如果图中任何两个顶点都是连通的，则称G为连通图。
	连通分量：无向图G的极大连通子图称为G的连通分量。
	极大连通子图意思是：该子图是G连通子图，如果再一个加入顶点，该子图不连通
	对于连通图，则其连通分量就是它自己，对于非连通图，则有2个以上连通分量。

12. 强连通图：有向图中，如果图中任何两个顶点vi到vj有路径，且vj到vi也有路径，则称G为强连通图。
	强连通分量：有向图G的极大强连通子图称为G的强连通分量。
	极大强连通子图意思是：该子图是G的强连通子图，如果再一个加入顶点，该子图不再是强连通的。

*/


// Adjacency Matrix 邻接矩阵, 用一个数组存放顶点信息即数据，一个二维数组存放边信息
/**
1. 无向图中，如果vi到vj有边，则邻接矩阵M[i][j] = M[j][i] = 1(true)，否则M[i][j] = 0(false)
2. 有向图中，如果vi到vj有边，则邻接矩阵M[i][j] = 1(true)，否则M[i][j] = 0(false)

3. 邻接矩阵的优点:
	快速判断两顶点之间是否右边。
	无向图/有向图中 Edge[i][j]=1表示有边，Edge[i][j]=0表示无边；
	网中，Edge[i][j]=∞表示无边，否则表示有边。时间复杂度为0（1）。
	方便计算各顶点的度。
	无向图中，邻接矩阵第i行元素之和就是顶点i的度；
	有向图中，第行元素之和就是顶点i的出度，第i列元素之和就是顶点i的入度。时间复杂度为0（n）。

4. 邻接矩阵的优缺点缺点:
	不便于增删顶点。增删顶点时，需要改变邻接矩阵的大小，效率较低。
	不便于访问所有邻接点。访问第i个顶点的所有邻接点，需要访问第i行的所有元素，时间复杂度为0（n）。访问所有顶点的邻接点，时间复杂度为o（n）。
	空间复杂度高。空间复杂度为0（n^2）。
*/

//====================邻接矩阵图相关定义======================
#define maxNum 100 //顶点数最大值
typedef char vertexType;  //顶点中存放数据的数据类型，根据需要定义

typedef int edgeType; //边数据类型
//定义邻接矩阵，用于存放一个图
typedef struct {
	vertexType vertexs[maxNum];
	edgeType edges[maxNum][maxNum];
	int vertexNum, edgeNum; //用于区分不同的图
}adjacencyMatrixGraph;

//====================邻接表数组图相关定义======================
//定义邻接点,用一个链表存放顶点所有的邻接点
typedef struct AdjacentNode { 
	int  index; //邻接点下标,即与该顶点邻接的顶点的数组下表
	struct AdjacentNode* next; //指向下一个邻接点
}adjacentNode;

//定义顶点数据结构，用于存放数据并指向邻接点链表中第一个邻接点
typedef struct VertexNode{
	vertexType data;
	adjacentNode* firstAdjNode;
}vertextNode;

//定义邻接表，用于存放一个图
typedef struct {
	//用一个数组存放所有的顶点；
	vertextNode vertextNodes[maxNum];
	int vertexNum, edgeNum;
}adjacencyListGraph;


//====================邻接矩阵相关接口函数======================
//查找顶点信息的下标
int findAMGVertexPosition(adjacencyMatrixGraph* amg, vertexType vertex);

//创建邻接矩阵图
void createAdjacencyMatrixGramph(adjacencyMatrixGraph* amg);

//void print Adjacency Matrix Gramph
void printAdjacencyMatricGramph(adjacencyMatrixGraph* amg);


//====================邻接表相关接口函数======================
//查找顶点的下标
int findALGVertexPosition(adjacencyListGraph* alg, vertexType vertex);

//向邻接表中插入一条边
void insertEdgeinALG(adjacencyListGraph* alg, vertexType u, vertexType v);

//void print Adjacency List Gramph
void printAdjacencyListGramph(adjacencyListGraph* alg);

//创建邻接矩阵图
void createAdjacencyListGramph(adjacencyListGraph* alg);

