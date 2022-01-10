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

//�������ݵ������ָ��λ��
void add_LinkList(LinkList* list, int index, void* data)
{
	if (list == NULL || data == NULL)
	{
		return;
	}

	//����������������ڷ�Χ�ڣ������ݲ��뵽β��
	if (index < 0 || index > list->size)
	{
		index = list->size;
	}
	//ͷ�ڵ�ռ����0���������ݽڵ����ʼλ��1.
	if (index == 0)
	{
		index = 1;
	}

	//�����ڵ㱣������
	LinkListNode* newNode = (LinkListNode*)malloc(sizeof(LinkListNode));
	newNode->data = data;
	newNode->next = NULL;

	//����ָ��λ�ô���ǰһ���ڵ�
	LinkListNode* prevNode = list->head;
	for (int i = 1; i < index; i++)
	{
		prevNode = prevNode->next;
	}

	newNode->next = prevNode->next;
	prevNode->next = newNode;

	list->size++;
}

//�������ָ��λ��ɾ������
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

	//����ָ��λ�ô���ǰһ���ڵ�
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

//�ı�������ָ��λ�õ�ֵ
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

	//����Ҫ����ֵ�Ľڵ�
	LinkListNode* tempNode = list->head;
	for (int i = 1; i <= index; i++)
	{
		tempNode = tempNode->next;
	}
	tempNode->data = data;
}

//��ѯĳ�������������е�λ��
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

//��ѯ����ָ��������������
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

//��ȡ����ĳ���
int getSize_LinkList(LinkList* list)
{
	return list->size;
}

//��ȡ�����е�һ���ڵ�
LinkListNode* front_LinkList(LinkList* list)
{
	return list->head->next;
}

//��ӡ�����е�����
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


//�ͷ������ڶ��з�����ڴ�ռ�
void free_LinkList(LinkList* list)
{
	if (list == NULL)
	{
		return;
	}

	//�ͷ����������еĽڵ�,����ͷ�ڵ�
	LinkListNode* currentNode = list->head;
	while (currentNode != NULL)
	{
		//������һ���ڵ�
		LinkListNode* nextNode = currentNode->next;
		if (currentNode!=NULL)
		{
			free(currentNode);
		}
		currentNode = nextNode;
	}
	//���ͷ�����
	list->size = 0;
	free(list);
}