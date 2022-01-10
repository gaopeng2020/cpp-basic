#include "seqQueue.h"

//初始化顺序队列
seqQueue* init_seqQueue(int len)
{
	seqQueue* queue = (seqQueue*)malloc(sizeof(seqQueue) + sizeof(void*) * len);
	queue->size = len;
	for (int i = 0; i < len; i++)
	{
		queue->array[i] = NULL;
	}
	return queue;
}

//如队列（添加数据）
void push_seqQueue(seqQueue* queue, void* data)
{
	if (queue == NULL || data == NULL)
	{
		return;
	}
	//从数组的尾部添加数据
	for (int i = 0; i < queue->size; i++)
	{
		if (queue->array[i] == NULL)
		{
			queue->array[i] = data;
			break;
		}
	}
	//queue->size++;
}

//出队列（删除数据）
void pop_seqQueue(seqQueue* queue)
{
	if (queue == NULL)
	{
		return;
	}
	//从队列的头部删除,需要将后续数据前移一位
	for (int i = 0; i < queue->size - 1; i++)
	{
		queue->array[i] = NULL;
		queue->array[i] = queue->array[i + 1];
	}
}

//返回队尾元素
void* back_seqQueue(seqQueue* queue)
{
	if (queue == NULL)
	{
		return NULL;
	}
	return queue->array[queue->size];
}

//返回队头元素
void* front_seqQueue(seqQueue* queue)
{
	if (queue == NULL)
	{
		return NULL;
	}
	return queue->array[0];
}

//获取顺序队列的长度
int getSize_seqQueue(seqQueue* queue)
{
	if (queue == NULL)
	{
		return -1;
	}
	return queue->size;
}

//清空队列中的数据
void clear_seqQueue(seqQueue* queue)
{
	if (queue == NULL)
	{
		return;
	}
	for (int i = 0; i < queue->size; i++)
	{
		queue->array[i] = NULL;
	}
}

//释放顺序队列在堆中分配的内存空间
void free_seqQueue(seqQueue* queue)
{
	if (queue == NULL)
	{
		return;
	}
	free(queue);
}

