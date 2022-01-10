#ifndef _SEQSTACK_H
#define _SEQSTACK_H
#include <stdlib.h>
#include <stdio.h>

typedef struct SEQSTACK
{
	int size;
	void* data[];
}seqStack;

//初始化顺序栈
seqStack* init_seqStack(int len);

//压栈（添加数据）
void push_seqStack(seqStack* stack,void* data);

//出栈（删除数据）
void pop_seqStack(seqStack* stack);

//返回栈顶元素
void* top_seqStack(seqStack* stack);

//获取顺序栈的长度
int getSize_seqStack(seqStack* stack);

//清空栈中的数据
void clear_seqStack(seqStack* stack);

//释放顺序栈在堆中分配的内存空间
void free_seqStack(seqStack* stack);

#endif // !_SEQSTACK_H

