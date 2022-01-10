#include "LinkList.h"


LinkList* init_LinkList()
{
	LinkList* list = (LinkList*)malloc(sizeof(LinkList));
	list->size = 0;
	list->head = (LinkListNode*)malloc(sizeof(LinkListNode));
	list->head->data = NULL;
	list->head->next = NULL;
	return list;
}

//插入数据到链表的指定位置
void add_LinkList(LinkList* list, int index, void* data)
{
	if (list == NULL || data == NULL)
	{
		return;
	}

	//如果给定的索引不在范围内，则将数据插入到尾部
	if (index < 0 || index > list->size)
	{
		index = list->size;
	}
	//头节点占用了0索引，数据节点的起始位是1.
	if (index == 0)
	{
		index = 1;
	}

	//创建节点保存数据
	LinkListNode* newNode = (LinkListNode*)malloc(sizeof(LinkListNode));
	newNode->data = data;
	newNode->next = NULL;

	//查找指定位置处的前一个节点
	LinkListNode* prevNode = list->head;
	for (int i = 1; i < index; i++)
	{
		prevNode = prevNode->next;
	}

	newNode->next = prevNode->next;
	prevNode->next = newNode;

	list->size++;
}

//从链表的指定位置删除数据
void remove_LinkList(LinkList* list, int index)
{
	if (list == NULL || index <= 0 || index >list->size)
	{
		return;
	}
	if (index == 0)
	{
		index = 1;
	}

	//查找指定位置处的前一个节点
	LinkListNode* prevNode = list->head;
	for (int i = 1; i < index; i++)
	{
		prevNode = prevNode->next;
	}

	LinkListNode* removeNode = prevNode->next;
	prevNode->next = removeNode->next;
	if (removeNode != NULL)
	{
		free(removeNode);
	}

	list->size--;
}

//改变链表中指定位置的值
void modify_LinkList(LinkList* list, int index, void* data)
{
	if (list == NULL || data == NULL)
	{
		return;
	}
	if (index <0 || index > list->size)
	{
		return;
	}
	if (index == 0)
	{
		index = 1;
	}

	//查找要更改值的节点
	LinkListNode* tempNode = list->head;
	for (int i = 1; i <= index; i++)
	{
		tempNode = tempNode->next;
	}
	tempNode->data = data;
}

//查询某个数据在链表中的位置
int getIndex_LinkList(LinkList* list, void* data)
{
	if (list == NULL || data == NULL)
	{
		return -1;
	}

	LinkListNode* tempNode = list->head;
	for (int i = 0; i <= list->size; i++)
	{
		if (tempNode->data == data)
		{
			return i;
		}
		tempNode = tempNode->next;
	}
	return -1;
}

//查询链表指定索引处的数据
void* getData_LinkList(LinkList* list, int index)
{
	if (list == NULL)
	{
		return NULL;
	}
	if (index < 0 || index > list->size)
	{
		return NULL;
	}
	if (index == 0)
	{
		index = 1;
	}

	LinkListNode* targetNode = list->head;
	for (int i = 1; i <= index; i++)
	{
		targetNode = targetNode->next;
	}
	return targetNode->data;
}

//获取链表的长度
int getSize_LinkList(LinkList* list)
{
	return list->size;
}

//获取链表中第一个节点
LinkListNode* front_LinkList(LinkList* list)
{
	return list->head->next;
}

//打印链表中的数据
void print_LinkList(LinkList* list, PRINTCLINKLIST printFunc)
{
	if (list == NULL)
	{
		return;
	}

	LinkListNode* tempNode = list->head->next;
	while (tempNode != NULL)
	{
		printFunc(tempNode->data);
		tempNode = tempNode->next;
	}
}


//释放链表在堆中分配的内存空间
void free_LinkList(LinkList* list)
{
	if (list == NULL)
	{
		return;
	}

	//释放链表内所有的节点,包括头节点
	LinkListNode* currentNode = list->head;
	while (currentNode != NULL)
	{
		//缓存下一个节点
		LinkListNode* nextNode = currentNode->next;
		if (currentNode!=NULL)
		{
			free(currentNode);
		}
		currentNode = nextNode;
	}
	//再释放链表
	list->size = 0;
	free(list);
}