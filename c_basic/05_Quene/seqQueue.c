#include "seqQueue.h"

//��ʼ��˳�����
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

//����У�������ݣ�
void push_seqQueue(seqQueue* queue, void* data)
{
	if (queue == NULL || data == NULL)
	{
		return;
	}
	//�������β���������
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

//�����У�ɾ�����ݣ�
void pop_seqQueue(seqQueue* queue)
{
	if (queue == NULL)
	{
		return;
	}
	//�Ӷ��е�ͷ��ɾ��,��Ҫ����������ǰ��һλ
	for (int i = 0; i < queue->size - 1; i++)
	{
		queue->array[i] = NULL;
		queue->array[i] = queue->array[i + 1];
	}
}

//���ض�βԪ��
void* back_seqQueue(seqQueue* queue)
{
	if (queue == NULL)
	{
		return NULL;
	}
	return queue->array[queue->size];
}

//���ض�ͷԪ��
void* front_seqQueue(seqQueue* queue)
{
	if (queue == NULL)
	{
		return NULL;
	}
	return queue->array[0];
}

//��ȡ˳����еĳ���
int getSize_seqQueue(seqQueue* queue)
{
	if (queue == NULL)
	{
		return -1;
	}
	return queue->size;
}

//��ն����е�����
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

//�ͷ�˳������ڶ��з�����ڴ�ռ�
void free_seqQueue(seqQueue* queue)
{
	if (queue == NULL)
	{
		return;
	}
	free(queue);
}

