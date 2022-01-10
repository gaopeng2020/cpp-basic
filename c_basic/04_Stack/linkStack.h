#ifndef _LINKSTACK_H
#define _LINKSTACK_H
#include <stdlib.h>
#include <stdio.h>

typedef struct LINKSTACKNODE
{
	void* data;
	struct LINKSTACKNODE* next;
}linkStackNode;

typedef struct linkStack
{
	int size;
	linkStackNode head;
}linkStack;

//��ʼ��˳��ջ
linkStack* init_linkStack();

//ѹջ��������ݣ�
void push_linkStack(linkStack* stack, void* data);

//��ջ��ɾ�����ݣ�
void pop_linkStack(linkStack* stack);

//����ջ��Ԫ��
void* top_linkStack(linkStack* stack);

//��ȡ˳��ջ�ĳ���
int getSize_linkStack(linkStack* stack);

//���ջ�е�����
void clear_linkStack(linkStack* stack);

void free_linkStack(linkStack* stack);

#endif // !_LINKSTACK_H

