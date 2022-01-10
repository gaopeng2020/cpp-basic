#include "EnterpriseList.h"

enterpriseList* init_LinkedList()
{
	enterpriseList* list = (enterpriseList*)malloc(sizeof(enterpriseList));
	list->head.next = NULL;
	list->size = 0;
	return list;
}

void add_EnterpriseList(enterpriseList* list, int index, enterpriseListNode* data)
{
	if (list == NULL || data == NULL)
	{
		return;
	}
	if (index < 0 || index > list->size)
	{
		index = list->size;
	}

	enterpriseListNode* pNode = &(list->head);
	for (int i = 0; i < index; i++)
	{
		pNode = pNode->next;
	}

	data->next = pNode->next;
	pNode->next = data;
	list->size++;
}

void remove_EnterpriseList(enterpriseList* list, int index)
{
	if (list == NULL)
	{
		return;
	}

	enterpriseListNode* pNode = &(list->head);
	for (int i = 0; i < index; i++)
	{
		pNode = pNode->next;
	}

	if (pNode->next != NULL)
	{
		enterpriseListNode* delNode = pNode->next;
		pNode->next = delNode->next;
		//free(delNode);
		list->size--;
	}
}

void modify_EnterpriseList(enterpriseList* list, int index, enterpriseListNode* data)
{

	if (list == NULL || data == NULL)
	{
		return;
	}
	if (index < 0 || index > list->size)
	{
		index = list->size;
	}

	enterpriseListNode* pNode = &(list->head);
	for (int i = 0; i < index; i++)
	{
		pNode = pNode->next;
	}
	data->next = pNode->next;
	pNode->next = data;
}

int getIndex_EnterpriseList(enterpriseList* list, enterpriseListNode* data)
{
	if (list == NULL || data == NULL)
	{
		return-1;
	}

	enterpriseListNode* pNode = &(list->head);
	for (int i = 0; i <= list->size; i++)
	{
		if (pNode == data)
			return  i;
		pNode = pNode->next;
	}
	return -1;
}

int getSize_EnterpriseList(enterpriseList* list)
{
	return list->size;
}

enterpriseListNode* front_EnterpriseList(enterpriseList* list)
{
	return &(list->head);
}

void print_EnterpriseList(enterpriseList* list, PRINTLINKEDLIST printFunc)
{
	if (list == NULL)
	{
		return;
	}
	enterpriseListNode* pNode = list->head.next;
	for (int i = 0; i < list->size; i++)
	{
		printFunc(pNode);
		pNode = pNode->next;
	}
}

void free_EnterpriseList(enterpriseList* list)
{
	if (list != NULL)
	{
		list->size = 0;
		free(list);
	}
}
