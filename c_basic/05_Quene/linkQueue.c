#include "linkQueue.h"

//��ʼ��˳�����
linkQueue* init_linkQueue()
{
	linkQueue* queue = (linkQueue*)malloc(sizeof(linkQueue));
	queue->size = 0;
	queue->head = (linkQueueNode*)malloc(sizeof(linkQueueNode));
	queue->head->data = NULL;
	queue->head->next = NULL;

	return queue;
}

//����У�������ݣ�
void push_linkQueue(linkQueue* queue, void* data)
{
	if (queue == NULL || data == NULL)
	{
		return;
	}
	//�ڶ�β���������ݣ�ͷ�ڵ���ڶ�ͷ��,�����½ڵ��β��ʼ��ָ��NULL

	linkQueueNode* newNode = (linkQueueNode*)malloc(sizeof(linkQueueNode));
	newNode->data = data;
	newNode->next = NULL;

	//��ȡβ�ڵ�,β�ڵ�ָ���½ڵ�
	linkQueueNode* backNode = queue->head;
	while (backNode->next != NULL)
	{
		backNode = backNode->next;
	}
	backNode->next = newNode;

	queue->size++;
}

//�����У�ɾ�����ݣ�
void pop_linkQueue(linkQueue* queue)
{
	if (queue == NULL)
	{
		return;
	}
	//�Զ��е�ͷ��ɾ��
	linkQueueNode* frontNode = queue->head->next;
	queue->head->next = frontNode->next;
	queue->size--;
	if (frontNode != NULL)
	{
		free(frontNode);
	}
}

//���ض�βԪ��
void* back_linkQueue(linkQueue* queue)
{
	if (queue == NULL)
	{
		return NULL;
	}
	linkQueueNode* backNode = queue->head->next;
	while (backNode != NULL && backNode->next != NULL)
	{
		backNode = backNode->next;
	}
	if (backNode == NULL)
	{
		return NULL;
	}
	return backNode->data;
}

//���ض�ͷԪ��
void* front_linkQueue(linkQueue* queue)
{
	if (queue == NULL)
	{
		return NULL;
	}
	if (queue->head->next != NULL)
	{
		return queue->head->next->data;
	}
	return NULL;
}

//��ȡ˳����еĳ���
int getSize_linkQueue(linkQueue* queue)
{
	if (queue == NULL)
	{
		return -1;
	}
	return queue->size;
}

//�ͷ�˳������ڶ��з�����ڴ�ռ�
void free_linkQueue(linkQueue* queue)
{
	if (queue == NULL)
	{
		return;
	}
	linkQueueNode* currentNode = queue->head;
	while (currentNode!= NULL && currentNode->next != NULL)
	{
		linkQueueNode* nextNode = currentNode->next;
		free(currentNode);
		currentNode = nextNode;
	}
	if (currentNode != NULL)
	{
		free(currentNode);
	}
	queue->size = 0;
	free(queue);
}
