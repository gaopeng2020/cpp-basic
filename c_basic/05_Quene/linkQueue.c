#include "linkQueue.h"

//初始化顺序队列
linkQueue* init_linkQueue()
{
	linkQueue* queue = (linkQueue*)malloc(sizeof(linkQueue));
	queue->size = 0;
	queue->head = (linkQueueNode*)malloc(sizeof(linkQueueNode));
	queue->head->data = NULL;
	queue->head->next = NULL;

	return queue;
}

//入队列（添加数据）
void push_linkQueue(linkQueue* queue, void* data)
{
	if (queue == NULL || data == NULL)
	{
		return;
	}
	//在队尾插入新数据（头节点放在对头）,所以新节点的尾部始终指向NULL

	linkQueueNode* newNode = (linkQueueNode*)malloc(sizeof(linkQueueNode));
	newNode->data = data;
	newNode->next = NULL;

	//获取尾节点,尾节点指向新节点
	linkQueueNode* backNode = queue->head;
	while (backNode->next != NULL)
	{
		backNode = backNode->next;
	}
	backNode->next = newNode;

	queue->size++;
}

//出队列（删除数据）
void pop_linkQueue(linkQueue* queue)
{
	if (queue == NULL)
	{
		return;
	}
	//对队列的头部删除
	linkQueueNode* frontNode = queue->head->next;
	queue->head->next = frontNode->next;
	queue->size--;
	if (frontNode != NULL)
	{
		free(frontNode);
	}
}

//返回队尾元素
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

//返回队头元素
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

//获取顺序队列的长度
int getSize_linkQueue(linkQueue* queue)
{
	if (queue == NULL)
	{
		return -1;
	}
	return queue->size;
}

//释放顺序队列在堆中分配的内存空间
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
