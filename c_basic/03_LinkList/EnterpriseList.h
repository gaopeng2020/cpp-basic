#ifndef _ENTERPRISELISTNODE_H
#define _ENTERPRISELISTNODE_H
#include <stdlib.h>
#include <stdio.h>

//定义链表节点
typedef struct ENTERPRISELISTNODE
{
	struct ENTERPRISELISTNODE* next;
}enterpriseListNode;

//定义链表
typedef struct LINKEDLIST
{
	enterpriseListNode head;
	int size;
}enterpriseList;

typedef void(*PRINTLINKEDLIST)(enterpriseListNode*);

//初始化链表
enterpriseList* init_LinkedList();

//插入数据到链表的指定位置
void add_EnterpriseList(enterpriseList* list, int index, enterpriseListNode* data);

//从链表的指定位置删除数据
void remove_EnterpriseList(enterpriseList* list, int index);

//改变链表中指定位置的值
void modify_EnterpriseList(enterpriseList* list, int index, enterpriseListNode* data);

//查询某个数据在链表中的位置
int getIndex_EnterpriseList(enterpriseList* list, enterpriseListNode* data);

//获取链表的长度
int getSize_EnterpriseList(enterpriseList* list);

//获取链表的第一个节点
enterpriseListNode* front_EnterpriseList(enterpriseList* list);

//打印链表中的数据
void print_EnterpriseList(enterpriseList* list, PRINTLINKEDLIST printFunc);

//释放链表在堆中分配的内存空间
void free_EnterpriseList(enterpriseList* list);


#endif // !_LINKEDLIST_H

