#ifndef _ENTERPRISELISTNODE_H
#define _ENTERPRISELISTNODE_H
#include <stdlib.h>
#include <stdio.h>

//��������ڵ�
typedef struct ENTERPRISELISTNODE
{
	struct ENTERPRISELISTNODE* next;
}enterpriseListNode;

//��������
typedef struct LINKEDLIST
{
	enterpriseListNode head;
	int size;
}enterpriseList;

typedef void(*PRINTLINKEDLIST)(enterpriseListNode*);

//��ʼ������
enterpriseList* init_LinkedList();

//�������ݵ������ָ��λ��
void add_EnterpriseList(enterpriseList* list, int index, enterpriseListNode* data);

//�������ָ��λ��ɾ������
void remove_EnterpriseList(enterpriseList* list, int index);

//�ı�������ָ��λ�õ�ֵ
void modify_EnterpriseList(enterpriseList* list, int index, enterpriseListNode* data);

//��ѯĳ�������������е�λ��
int getIndex_EnterpriseList(enterpriseList* list, enterpriseListNode* data);

//��ȡ����ĳ���
int getSize_EnterpriseList(enterpriseList* list);

//��ȡ����ĵ�һ���ڵ�
enterpriseListNode* front_EnterpriseList(enterpriseList* list);

//��ӡ�����е�����
void print_EnterpriseList(enterpriseList* list, PRINTLINKEDLIST printFunc);

//�ͷ������ڶ��з�����ڴ�ռ�
void free_EnterpriseList(enterpriseList* list);


#endif // !_LINKEDLIST_H

