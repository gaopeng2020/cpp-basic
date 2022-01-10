#include "CircleLinkList.h"

circleLinkList* init_CircleLinkList()
{
	circleLinkList* list = (circleLinkList*)malloc(sizeof(circleLinkList));
	list->size = 0;
	list->head = (circleLinkListNode*)malloc(sizeof(circleLinkListNode));
	list->head->data = NULL;
	list->head->next = list->head;
	return list;
}

void add_CircleLinkList(circleLinkList* list, int index, void* data)
{
	if (list == NULL || data == NULL)
	{
		return;
	}
	if (index < 0 || index > list->size)
	{
		index = list->size;
	}
	//ͷ�ڵ�ռ����0���������ݽڵ����ʼλ��1.
	if (index == 0)
	{
		index = 1;
	}

	//��ȡָ��λ�õ�ǰһ���ڵ�
	circleLinkListNode* prevNode = list->head;
	for (int i = 1; i < index; i++)
	{
		prevNode = prevNode->next;
	}

	//����ڵ㱣������
	circleLinkListNode* newNode= (circleLinkListNode*)malloc(sizeof(circleLinkListNode));
	newNode->data = data;
	newNode->next = prevNode->next;
	prevNode->next = newNode;

	list->size++;
}

void remove_CircleLinkList(circleLinkList* list, int index)
{
	if (list == NULL)
	{
		return;
	}
	if (index < 0 || index > list->size)
	{
		return;
	}
	if (index == 0)
	{
		index = 1;
	}

	//��ȡ��ɾ���Ľڵ�ǰһ���ڵ�
	circleLinkListNode* prevNode = list->head;
	for (int i = 1; i < index; i++)
	{
		prevNode = prevNode->next;
	}

	circleLinkListNode* removedNode = prevNode->next;
	prevNode->next = removedNode->next;

	if (removedNode != NULL)
	{
		free(removedNode);
	}

	list->size--;
}

void modify_CircleLinkList(circleLinkList* list, int index, void* data)
{
	if (list == NULL || data == NULL)
	{
		return;
	}
	if (index < 0 || index > list->size)
	{
		return;
	}
	if (index == 0)
	{
		index = 1;
	}

	//��ȡָ��λ�ô��Ľڵ�
	circleLinkListNode* prevNode = list->head;
	for (int i = 1; i <= index; i++)
	{
		prevNode = prevNode->next;
	}

	prevNode->data = data;

}

int getIndex_CircleLinkList(circleLinkList* list, void* data)
{
	if (list == NULL)
	{
		return -1;
	}

	circleLinkListNode* currentNode = list->head;
	for (int i = 1; i <= list->size; i++)
	{
		currentNode = currentNode->next;
		if (currentNode->data = data)
		{
			return i;
		}
	}

	return -1;
}

void* getData_CircleLinkList(circleLinkList* list, int index)
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

	//��ȡָ��λ�ô��Ľڵ�
	circleLinkListNode* pIndex = list->head;
	for (int i = 1; i <= index; i++)
	{
		pIndex = pIndex->next;
	}

	return pIndex->data;
}

int getSize_CircleLinkList(circleLinkList* list)
{
	if (list == NULL)
	{
		return -1;
	}

	return list->size;
}

void print_CircleLinkList(circleLinkList* list, PRINTCLINKLIST printfun)
{
	if (list == NULL)
	{
		return;
	}

	circleLinkListNode* pIndex = list->head;
	for (int i = 0; i < list->size; i++)
	{
		pIndex = pIndex->next;
		printfun(pIndex->data);
	}
}

void free_CircleLinkList(circleLinkList* list)
{
	if (list == NULL)
	{
		return;
	}
	circleLinkListNode* pCurrent = list->head;
	for (int i = 0; i < list->size; i++)
	{
		circleLinkListNode* pNext = pCurrent->next;
		if (pCurrent !=NULL)
		{
			free(pCurrent);
		}
		pCurrent = pNext;
	}

	list->size = 0;
	free(list);
}
