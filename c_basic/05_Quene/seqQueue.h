#ifndef SEQQUEUE_H
#define SEQQUEUE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct SEQQUENE
{
	int size;
	void* array[];
}seqQueue;

//初始化顺序队列
seqQueue* init_seqQueue(int len);

//压队列（添加数据）
void push_seqQueue(seqQueue* queue, void* data);

//出队列（删除数据）
void pop_seqQueue(seqQueue* queue);

//返回队尾元素
void* back_seqQueue(seqQueue* queue);

//返回队头元素
void* front_seqQueue(seqQueue* queue);

//获取顺序队列的长度
int getSize_seqQueue(seqQueue* queue);

//清空队列中的数据
void clear_seqQueue(seqQueue* queue);

//释放顺序队列在堆中分配的内存空间
void free_seqQueue(seqQueue* queue);

#endif // !SEQQUEUE_H

