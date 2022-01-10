#include "seqStack.h"
//���庯��ָ�뷽���û����ݲ�ͬ���������ͽ��д�ӡ
typedef void(*PRINTSEQSTACK)(void*);

//��ʼ��˳��ջ
seqStack* init_seqStack(int len)
{
	seqStack* stack = (seqStack*)malloc(sizeof(seqStack)+ sizeof(void*) * len);
	stack->size = len;
	for (int i = 0; i < len; i++)
	{
		stack->data[i] = NULL;
	}
	return stack;
}

//ѹջ��������ݣ�
void push_seqStack(seqStack* stack, void* data)
{
	if (stack == NULL || data == NULL)
	{
		return;
	}

	for (int i = 0; i < stack->size; i++)
	{
		if (stack->data[i] == NULL)
		{
			stack->data[i] = data;
			return;
		}
	}
}

//��ջ��ɾ�����ݣ�
void pop_seqStack(seqStack* stack)
{
	if (stack == NULL)
	{
		return;
	}

	for (int i = stack->size-1; i >= 0; i--)
	{
		if (stack->data[i] != NULL)
		{
			stack->data[i] = NULL;
			return;
		}
	}
}

//����ջ��Ԫ��
void* top_seqStack(seqStack* stack)
{
	if (stack == NULL)
	{
		return NULL;
	}

	for (int i = stack->size-1; i >= 0; i--)
	{
		if (stack->data[i] != NULL)
		{
			return stack->data[i];
		}
	}
	return NULL;
}

//��ȡ˳��ջ�ĳ���
int getSize_seqStack(seqStack* stack)
{
	return stack->size;
}

//���ջ�е�����
void clear_seqStack(seqStack* stack)
{
	if (stack == NULL)
	{
		return;
	}
	for (int i = stack->size-1; i >= 0; i--)
	{
		if (stack->data[i] != NULL)
		{
			stack->data[i] = NULL;
		}
	}
}

//�ͷ�˳��ջ�ڶ��з�����ڴ�ռ�
void free_seqStack(seqStack* stack)
{
	if (stack != NULL)
	{
		free(stack);
	}
}
