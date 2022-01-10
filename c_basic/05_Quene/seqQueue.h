#ifndef SEQQUEUE_H
#define SEQQUEUE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct SEQQUENE
{
	int size;
	void* array[];
}seqQueue;

//��ʼ��˳�����
seqQueue* init_seqQueue(int len);

//ѹ���У�������ݣ�
void push_seqQueue(seqQueue* queue, void* data);

//�����У�ɾ�����ݣ�
void pop_seqQueue(seqQueue* queue);

//���ض�βԪ��
void* back_seqQueue(seqQueue* queue);

//���ض�ͷԪ��
void* front_seqQueue(seqQueue* queue);

//��ȡ˳����еĳ���
int getSize_seqQueue(seqQueue* queue);

//��ն����е�����
void clear_seqQueue(seqQueue* queue);

//�ͷ�˳������ڶ��з�����ڴ�ռ�
void free_seqQueue(seqQueue* queue);

#endif // !SEQQUEUE_H

