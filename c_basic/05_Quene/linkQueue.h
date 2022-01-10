#ifndef _LINKQUEUE_H
#define _LINKQUEUE_H
#include <stdlib.h>
#include <stdio.h>

typedef struct LINKQUEUENODE
{
	void* data;
	struct LINKQUEUENODE* next;
}linkQueueNode;

typedef struct LINKQUEUE
{
	int size;
	linkQueueNode* head;
}linkQueue;

//初始化顺序队列
linkQueue* init_linkQueue();

//压队列（添加数据）
void push_linkQueue(linkQueue* queue, void* data);

//出队列（删除数据）
void pop_linkQueue(linkQueue* queue);

//返回队尾元素
void* back_linkQueue(linkQueue* queue);

//返回队头元素
void* front_linkQueue(linkQueue* queue);

//获取顺序队列的长度
int getSize_linkQueue(linkQueue* queue);

//释放顺序队列在堆中分配的内存空间
void free_linkQueue(linkQueue* queue);

#endif // !_LINKQUEUE_H
