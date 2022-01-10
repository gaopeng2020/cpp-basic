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

	printf("�����붥����\n");
	scanf("%d", &amg->vertexNum);
	printf("���������\n");
	scanf("%d", &amg->edgeNum);

	printf("�������������еĶ���\n");
	for (int i = 0; i < amg->vertexNum; i++)
		scanf("%d", &amg->vertexs[i]);
	//��ʼ���ڽӾ���,������ֵ��Ϊ0
	for (int i = 0; i < amg->vertexNum; i++)
		for (int j = 0; j < amg->vertexNum; j++)
			amg->edges[i][j] = 0;

	printf("����������ÿ��������������������u,v\n");
	while (amg->edgeNum--)
	{
		scanf("u=%c,v=%c\n", &u, &v);
		//���Ҷ���u�Ĵ洢�±�
		uIndex = findAMGVertexPosition(amg, u);
		//���Ҷ���v�Ĵ洢�±�
		vIndex = findAMGVertexPosition(amg, v);

		//�ڽӾ�����1,���б������������������γɵľ���edges[uIndex][vIndex] = 1;;
		if (uIndex != -1 && vIndex != -1) {
			amg->edges[uIndex][vIndex] = 1;
			printf("���������ʣ���%d��������������������\n", amg->edgeNum);
		}
		else
		{
			printf("���������Ķ��㲻���ڣ�����������\n");
			amg->edgeNum++;
		}
	}
}

void printAdjacencyMatricGramph(adjacencyMatrixGraph* amg)
{
	if (amg == NULL)
		return;
	printf("ͼ���ڽӾ���Ϊ:\n");
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
	//���Ҷ���u�Ĵ洢�±�
	int uIndex = findALGVertexPosition(alg, u);
	//���Ҷ���v�Ĵ洢�±�
	int vIndex = findALGVertexPosition(alg, v);
	//�ж�����Ķ����Ƿ����
	if (uIndex != -1 && vIndex != -1) {
		//����һ���ڽӵ㣬���������ͷ�巨��������ָ���½����ڽӵ㣬�ڽӵ��nextָ�򶥵�ԭ����firstAdjNode
		adjacentNode* adjNode = (adjacentNode*)malloc(sizeof(adjacentNode));
		adjNode->index = vIndex;
		adjNode->next = alg->vertextNodes[u].firstAdjNode; //�ڽӵ��nextָ�򶥵�ԭ����firstAdjNode��
		alg->vertextNodes[u].firstAdjNode = adjNode; //����������ָ����ڽӵ㣬ͷ�巨
		printf("���������ʣ���%d��������������������\n", alg->edgeNum);
	}
	else
	{
		printf("���������Ķ��㲻���ڣ�����������\n");
		alg->edgeNum++;
	}

}

void printAdjacencyListGramph(adjacencyListGraph* alg)
{
	if (alg == NULL)
		return -1;
	printf("ͼ���ڽӱ�������ʾ:\n");
	//������ӡÿ��������ڽӱ�
	for (int i = 0; i < alg->vertexNum;i++) {
		printf("����:%c\t", alg->vertextNodes[i].data);
		adjacentNode* nextAdjNode = alg->vertextNodes[i].firstAdjNode;
		while (nextAdjNode)
		{
			printf("�ڽӵ���±������ǣ�%d\t",nextAdjNode->index);
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
	printf("�����붥����\n");
	scanf("%d", &alg->vertexNum);
	printf("���������\n");
	scanf("%d", &alg->edgeNum);

	printf("�������������еĶ���\n");
	for (int i = 0; i < alg->vertexNum; i++)
		scanf("%d", &alg->vertextNodes[i]);
	//��ʼ���ڽӱ�,���ڵ��firstAdjNodeָ��NULL
	for (int i = 0; i < alg->vertexNum; i++)
		alg->vertextNodes[i].firstAdjNode = NULL;

	printf("����������ÿ��������������������u,v\n");
	while (alg->edgeNum--)
	{
		scanf("u=%c,v=%c\n", &u, &v);
		//u��Ϊ���㣬v��Ϊ�ڽӵ㣬���ڽ������н����������
		insertEdgeinALG(alg, u, v);
	}

}
