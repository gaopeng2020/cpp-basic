#include "linkStack.h"

//��ʼ��˳��ջ
linkStack* init_linkStack()
{
	linkStack* stack = (linkStack*)malloc(sizeof(linkStack));
	stack->size = 0;
	stack->head.next = NULL;
	return stack;
}

//ѹջ��������ݣ�
void push_linkStack(linkStack* stack, void* data)
{
	if (stack == NULL || data == NULL)
	{
		return;
	}

	linkStackNode* newNode = (linkStackNode*)malloc(sizeof(linkStackNode));
	newNode->data = data;

	newNode->next = stack->head.next;
	stack->head.next = newNode;

	stack->size++;
}

//��ջ��ɾ�����ݣ�
void pop_linkStack(linkStack* stack)
{
	if (stack == NULL || stack->size == 0)
	{
		return;
	}

	linkStackNode* currentNode = stack->head.next;
	stack->head.next = currentNode->next;
	if (currentNode != NULL)
	{
		free(currentNode);
	}
	stack->size--;
}

//����ջ��Ԫ��
void* top_linkStack(linkStack* stack)
{
	if (stack == NULL || stack->size == 0)
	{
		return NULL;
	}
	return stack->head.next->data;
}

//��ȡ˳��ջ�ĳ���
int getSize_linkStack(linkStack* stack)
{
	return stack->size;
}

//���ջ�е�����
void clear_linkStack(linkStack* stack)
{
	if (stack == NULL)
	{
		return;
	}

	if (stack->size == 0)
	{
		return;
	}
	linkStackNode* currentNode = stack->head.next;
	while (currentNode != NULL && currentNode->next != NULL)
	{
		//������һ���ڵ�
		linkStackNode* nextNode = currentNode->next;
		if (currentNode != NULL)
		{
			free(currentNode);
		}
		currentNode = nextNode;
		stack->head.next = nextNode;
		stack->size--;
	}
}
//�ͷ�ջ�ڶ��з�����ڴ�ռ�
void free_linkStack(linkStack* stack)
{
	if (stack == NULL)
	{
		return;
	}

	linkStackNode* currentNode = stack->head.next;
	while (currentNode!= NULL && currentNode->next != NULL)
	{
		linkStackNode* nextNode = currentNode->next;
		if (currentNode != NULL)
		{
			free(currentNode);
		}
		currentNode = nextNode;
		stack->head.next = nextNode;
	}

	stack->size = 0;
	free(stack);
}
