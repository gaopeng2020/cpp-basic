#ifndef _SEQSTACK_H
#define _SEQSTACK_H
#include <stdlib.h>
#include <stdio.h>

typedef struct SEQSTACK
{
	int size;
	void* data[];
}seqStack;

//��ʼ��˳��ջ
seqStack* init_seqStack(int len);

//ѹջ��������ݣ�
void push_seqStack(seqStack* stack,void* data);

//��ջ��ɾ�����ݣ�
void pop_seqStack(seqStack* stack);

//����ջ��Ԫ��
void* top_seqStack(seqStack* stack);

//��ȡ˳��ջ�ĳ���
int getSize_seqStack(seqStack* stack);

//���ջ�е�����
void clear_seqStack(seqStack* stack);

//�ͷ�˳��ջ�ڶ��з�����ڴ�ռ�
void free_seqStack(seqStack* stack);

#endif // !_SEQSTACK_H

