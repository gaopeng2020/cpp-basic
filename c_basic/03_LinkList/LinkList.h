#pragma once
#include <stdlib.h>
#include <stdio.h>

//��������ڵ�
typedef struct LINKLISTNODE
{
	void* data;
	struct LINKLISTNODE* next;
}LinkListNode;

//��������
typedef struct LINKLIST
{
	LinkListNode* head;
	int size;
}LinkList;

//�����ӡ�ص�����ָ��
typedef void(*PRINTCLINKLIST)(void*);

//��ʼ������
LinkList* init_LinkList();

//�������ݵ������ָ��λ��
void add_LinkList(LinkList* list, int index, void* data);

//�������ָ��λ��ɾ������
void remove_LinkList(LinkList* list, int index);

//�ı�������ָ��λ�õ�ֵ
void modify_LinkList(LinkList* list, int index, void* data);

//��ѯĳ�������������е�λ��
int getIndex_LinkList(LinkList* list, void* data);

//��ѯ����ָ��������������
void* getData_LinkList(LinkList* list, int index);

//��ȡ����ĳ���
int getSize_LinkList(LinkList* list);

//��ȡ����ĵ�һ���ڵ�
LinkListNode* front_LinkList(LinkList* list);

//��ӡ�����е�����
void print_LinkList(LinkList* list, PRINTCLINKLIST printFunc);

//�ͷ������ڶ��з�����ڴ�ռ�
void free_LinkList(LinkList* list);




