#ifndef _CIRCLELINKLIST_H
#define _CIRCLELINKLIST_H
#include <stdlib.h>
#include <stdio.h>

//循环链表头节点
typedef struct CIRCLELINKLISTNODE
{
	void* data;
	struct CIRCLELINKLISTNODE* next;
}circleLinkListNode;
//循环链表
typedef struct CIRCLELINKLIST
{
	int size;
	circleLinkListNode* head;
}circleLinkList;

//定义打印回调函数指针
typedef void(*PRINTCLINKLIST)(void*);

//初始化循环链表的头节点，头节点不保存数据
circleLinkList* init_CircleLinkList();

//插入数据到链表的指定位置
void add_CircleLinkList(circleLinkList* list,int index, void* data);

//从链表的指定位置删除数据
void remove_CircleLinkList(circleLinkList* list, int index);

//改变链表中指定位置的值
void modify_CircleLinkList(circleLinkList* list, int index, void* data);

//查询某个数据在链表中的位置
int getIndex_CircleLinkList(circleLinkList* list, void* data);

//获取链表中指定位置处的数据
void* getData_CircleLinkList(circleLinkList* list, int index);

//获取链表的长度
int getSize_CircleLinkList(circleLinkList* list);

//打印链表中的数据
void print_CircleLinkList(circleLinkList* list, PRINTCLINKLIST printfun);

//释放链表在堆中分配的内存空间
void free_CircleLinkList(circleLinkList* list);

#endif // _CIRCLELINKLIST_H
