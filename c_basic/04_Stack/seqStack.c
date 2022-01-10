#include "seqStack.h"
//定义函数指针方便用户根据不同的数据类型进行打印
typedef void(*PRINTSEQSTACK)(void*);

//初始化顺序栈
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

//压栈（添加数据）
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

//出栈（删除数据）
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

//返回栈顶元素
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

//获取顺序栈的长度
int getSize_seqStack(seqStack* stack)
{
	return stack->size;
}

//清空栈中的数据
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

//释放顺序栈在堆中分配的内存空间
void free_seqStack(seqStack* stack)
{
	if (stack != NULL)
	{
		free(stack);
	}
}
