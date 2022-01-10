#pragma once
#include <stdlib.h>
#include <stdio.h>

//定义链表节点
typedef struct LINKLISTNODE
{
	void* data;
	struct LINKLISTNODE* next;
}LinkListNode;

//定义链表
typedef struct LINKLIST
{
	LinkListNode* head;
	int size;
}LinkList;

//定义打印回调函数指针
typedef void(*PRINTCLINKLIST)(void*);

//初始化链表
LinkList* init_LinkList();

//插入数据到链表的指定位置
void add_LinkList(LinkList* list, int index, void* data);

//从链表的指定位置删除数据
void remove_LinkList(LinkList* list, int index);

//改变链表中指定位置的值
void modify_LinkList(LinkList* list, int index, void* data);

//查询某个数据在链表中的位置
int getIndex_LinkList(LinkList* list, void* data);

//查询链表指定索引处的数据
void* getData_LinkList(LinkList* list, int index);

//获取链表的长度
int getSize_LinkList(LinkList* list);

//获取链表的第一个节点
LinkListNode* front_LinkList(LinkList* list);

//打印链表中的数据
void print_LinkList(LinkList* list, PRINTCLINKLIST printFunc);

//释放链表在堆中分配的内存空间
void free_LinkList(LinkList* list);




