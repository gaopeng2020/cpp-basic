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

//��ʼ��˳�����
linkQueue* init_linkQueue();

//ѹ���У�������ݣ�
void push_linkQueue(linkQueue* queue, void* data);

//�����У�ɾ�����ݣ�
void pop_linkQueue(linkQueue* queue);

//���ض�βԪ��
void* back_linkQueue(linkQueue* queue);

//���ض�ͷԪ��
void* front_linkQueue(linkQueue* queue);

//��ȡ˳����еĳ���
int getSize_linkQueue(linkQueue* queue);

//�ͷ�˳������ڶ��з�����ڴ�ռ�
void free_linkQueue(linkQueue* queue);

#endif // !_LINKQUEUE_H
