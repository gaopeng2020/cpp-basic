#include "linkStack.h"

//初始化顺序栈
linkStack* init_linkStack()
{
	linkStack* stack = (linkStack*)malloc(sizeof(linkStack));
	stack->size = 0;
	stack->head.next = NULL;
	return stack;
}

//压栈（添加数据）
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

//出栈（删除数据）
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

//返回栈顶元素
void* top_linkStack(linkStack* stack)
{
	if (stack == NULL || stack->size == 0)
	{
		return NULL;
	}
	return stack->head.next->data;
}

//获取顺序栈的长度
int getSize_linkStack(linkStack* stack)
{
	return stack->size;
}

//清空栈中的数据
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
		//缓存下一个节点
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
//释放栈在堆中分配的内存空间
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
