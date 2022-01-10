#ifndef _CIRCLELINKLIST_H
#define _CIRCLELINKLIST_H
#include <stdlib.h>
#include <stdio.h>

//ѭ������ͷ�ڵ�
typedef struct CIRCLELINKLISTNODE
{
	void* data;
	struct CIRCLELINKLISTNODE* next;
}circleLinkListNode;
//ѭ������
typedef struct CIRCLELINKLIST
{
	int size;
	circleLinkListNode* head;
}circleLinkList;

//�����ӡ�ص�����ָ��
typedef void(*PRINTCLINKLIST)(void*);

//��ʼ��ѭ�������ͷ�ڵ㣬ͷ�ڵ㲻��������
circleLinkList* init_CircleLinkList();

//�������ݵ������ָ��λ��
void add_CircleLinkList(circleLinkList* list,int index, void* data);

//�������ָ��λ��ɾ������
void remove_CircleLinkList(circleLinkList* list, int index);

//�ı�������ָ��λ�õ�ֵ
void modify_CircleLinkList(circleLinkList* list, int index, void* data);

//��ѯĳ�������������е�λ��
int getIndex_CircleLinkList(circleLinkList* list, void* data);

//��ȡ������ָ��λ�ô�������
void* getData_CircleLinkList(circleLinkList* list, int index);

//��ȡ����ĳ���
int getSize_CircleLinkList(circleLinkList* list);

//��ӡ�����е�����
void print_CircleLinkList(circleLinkList* list, PRINTCLINKLIST printfun);

//�ͷ������ڶ��з�����ڴ�ռ�
void free_CircleLinkList(circleLinkList* list);

#endif // _CIRCLELINKLIST_H
