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

//初始化顺序栈
linkStack* init_linkStack();

//压栈（添加数据）
void push_linkStack(linkStack* stack, void* data);

//出栈（删除数据）
void pop_linkStack(linkStack* stack);

//返回栈顶元素
void* top_linkStack(linkStack* stack);

//获取顺序栈的长度
int getSize_linkStack(linkStack* stack);

//清空栈中的数据
void clear_linkStack(linkStack* stack);

void free_linkStack(linkStack* stack);

#endif // !_LINKSTACK_H

