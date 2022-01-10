#include "graph.h"

int findAMGVertexPosition(adjacencyMatrixGraph* amg, vertexType vertex)
{
	if (amg == NULL)
		return -1;

	for (int i = 0; i < amg->edgeNum; i++)
		if (vertex == amg->vertexs[i])
			return i;
	return -1;
}

void createAdjacencyMatrixGramph(adjacencyMatrixGraph* amg)
{
	vertexType u, v;
	int uIndex, vIndex;

	if (amg == NULL)
		return;

	printf("请输入顶点数\n");
	scanf("%d", &amg->vertexNum);
	printf("请输入边数\n");
	scanf("%d", &amg->edgeNum);

	printf("请依次输入所有的顶点\n");
	for (int i = 0; i < amg->vertexNum; i++)
		scanf("%d", &amg->vertexs[i]);
	//初始化邻接矩阵,将所有值置为0
	for (int i = 0; i < amg->vertexNum; i++)
		for (int j = 0; j < amg->vertexNum; j++)
			amg->edges[i][j] = 0;

	printf("请依次输入每条边所依附的两个顶点u,v\n");
	while (amg->edgeNum--)
	{
		scanf("u=%c,v=%c\n", &u, &v);
		//查找顶点u的存储下标
		uIndex = findAMGVertexPosition(amg, u);
		//查找顶点v的存储下标
		vIndex = findAMGVertexPosition(amg, v);

		//邻接矩阵储置1,即有边所依附的两个顶点形成的矩阵edges[uIndex][vIndex] = 1;;
		if (uIndex != -1 && vIndex != -1) {
			amg->edges[uIndex][vIndex] = 1;
			printf("请继续输入剩余的%d条边所依附的两个顶点\n", amg->edgeNum);
		}
		else
		{
			printf("边所依附的顶点不存在，请重新输入\n");
			amg->edgeNum++;
		}
	}
}

void printAdjacencyMatricGramph(adjacencyMatrixGraph* amg)
{
	if (amg == NULL)
		return;
	printf("图的邻接矩阵为:\n");
	for (int i = 0; i < amg->vertexNum; i++)
	{
		for (int j = 0; j < amg->vertexNum; j++)
		{
			printf("%d\t", amg->edges[i][j]);
		}
		printf("\n");
	}
}

int findALGVertexPosition(adjacencyListGraph* alg, vertexType vertex)
{
	if (alg == NULL)
		return -1;

	for (int i = 0; i < alg->edgeNum; i++)
		if (vertex == alg->vertextNodes[i].data)
			return i;
	return -1;
}

void insertEdgeinALG(adjacencyListGraph* alg, vertexType u, vertexType v)
{
	if (alg == NULL)
		return -1;
	//查找顶点u的存储下标
	int uIndex = findALGVertexPosition(alg, u);
	//查找顶点v的存储下标
	int vIndex = findALGVertexPosition(alg, v);
	//判断输入的顶点是否存在
	if (uIndex != -1 && vIndex != -1) {
		//创建一个邻接点，采用链表的头插法，将顶点指向新建的邻接点，邻接点的next指向顶点原来的firstAdjNode
		adjacentNode* adjNode = (adjacentNode*)malloc(sizeof(adjacentNode));
		adjNode->index = vIndex;
		adjNode->next = alg->vertextNodes[u].firstAdjNode; //邻接点的next指向顶点原来的firstAdjNode；
		alg->vertextNodes[u].firstAdjNode = adjNode; //将顶点重新指向该邻接点，头插法
		printf("请继续输入剩余的%d条边所依附的两个顶点\n", alg->edgeNum);
	}
	else
	{
		printf("边所依附的顶点不存在，请重新输入\n");
		alg->edgeNum++;
	}

}

void printAdjacencyListGramph(adjacencyListGraph* alg)
{
	if (alg == NULL)
		return -1;
	printf("图的邻接表如下所示:\n");
	//遍历打印每个顶点的邻接表
	for (int i = 0; i < alg->vertexNum;i++) {
		printf("顶点:%c\t", alg->vertextNodes[i].data);
		adjacentNode* nextAdjNode = alg->vertextNodes[i].firstAdjNode;
		while (nextAdjNode)
		{
			printf("邻接点的下标依次是：%d\t",nextAdjNode->index);
			nextAdjNode = nextAdjNode->next;
		}
		printf("\n");
	}

}

void createAdjacencyListGramph(adjacencyListGraph* alg)
{
	vertexType u, v;
	int uIndex, vIndex;

	if (alg == NULL)
		return -1;
	printf("请输入顶点数\n");
	scanf("%d", &alg->vertexNum);
	printf("请输入边数\n");
	scanf("%d", &alg->edgeNum);

	printf("请依次输入所有的顶点\n");
	for (int i = 0; i < alg->vertexNum; i++)
		scanf("%d", &alg->vertextNodes[i]);
	//初始化邻接表,将节点的firstAdjNode指向NULL
	for (int i = 0; i < alg->vertexNum; i++)
		alg->vertextNodes[i].firstAdjNode = NULL;

	printf("请依次输入每条边所依附的两个顶点u,v\n");
	while (alg->edgeNum--)
	{
		scanf("u=%c,v=%c\n", &u, &v);
		//u作为顶点，v作为邻接点，在邻接链表中将其关联起来
		insertEdgeinALG(alg, u, v);
	}

}
