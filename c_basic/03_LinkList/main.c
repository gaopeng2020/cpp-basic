#include <stdlib.h>
#include <stdio.h>
#include "LinkList.h"
#include "EnterpriseList.h"
#include "CircleLinkList.h"

typedef struct STUDENT_LINKLIST
{
	char name[64];
	int age;
}s_List;

typedef struct STUDENT_ENTERPRISELIST
{
	enterpriseListNode node;
	char name[64];
	int age;
}s_EnterpriseList;

void printfun_List(void* data)
{
	s_List *s = (s_List*)data;
	printf("Name:%s\t Age:%d\n",s->name,s->age);
}

void linkListTest()
{
	s_List s1 = { "aaa",11 };
	s_List s2 = { "bbb",12 };
	s_List s3 = { "ccc",13 };
	s_List s4 = { "ddd",14 };
	s_List s5 = { "eee",15 };
	s_List s6 = { "fff",16 };
	s_List s7 = { "ggg",17 };
	s_List s8 = { "hhh",18 };

	LinkList* list = init_LinkList();

	add_LinkList(list, 1, &s1);
	add_LinkList(list, 1, &s2);
	add_LinkList(list, 1, &s3);
	add_LinkList(list, 1, &s4);
	add_LinkList(list, 0, &s5);

	add_LinkList(list, 2, &s6);
	add_LinkList(list, 5, &s7);
	add_LinkList(list, 6, &s8);
	printf("List size£¨befor Delete£© is: %d\n", getSize_LinkList(list));

	remove_LinkList(list, 2);
	remove_LinkList(list, 4);
	remove_LinkList(list, 4);
	printf("List size£¨after Delete£© is: %d\n", getSize_LinkList(list));

	s_List s23 = { "ccc",23 };
	modify_LinkList(list,3, &s23);

	printf("S1 Index is: %d\n", getIndex_LinkList(list, &s1));
	printf("S5 Index is: %d\n", getIndex_LinkList(list, &s5));

	s_List* s = (s_List*)getData_LinkList(list, 6);
	if (s != NULL)
	{
		printf("Name:%s\t Age:%d\n\n", s->name, s->age);
	}

	print_LinkList(list, printfun_List);

	free_LinkList(list);
}

void enterpriseListTest()
{
	s_EnterpriseList s1 = { NULL, "aaa",11 };
	s_EnterpriseList s2 = { NULL, "bbb",12 };
	s_EnterpriseList s3 = { NULL, "ccc",13 };
	s_EnterpriseList s4 = { NULL, "ddd",14 };
	s_EnterpriseList s5 = { NULL, "eee",15 };
	s_EnterpriseList s6 = { NULL, "fff",16 };
	s_EnterpriseList s7 = { NULL, "ggg",17 };
	s_EnterpriseList s8 = { NULL, "hhh",18 };

	enterpriseList* list = init_LinkedList();

	add_EnterpriseList(list, 0, (enterpriseListNode*)&s1);

	add_EnterpriseList(list, 0, (enterpriseListNode*)&s2);
	add_EnterpriseList(list, 0, (enterpriseListNode*)&s3);
	add_EnterpriseList(list, 0, (enterpriseListNode*)&s4);
	add_EnterpriseList(list, 0, (enterpriseListNode*)&s5);

	add_EnterpriseList(list, 2, (enterpriseListNode*)&s6);
	add_EnterpriseList(list, 5, (enterpriseListNode*)&s7);
	add_EnterpriseList(list, 6, (enterpriseListNode*)&s8);
	printf("List size£¨befor Delete£© is: %d\n", getSize_EnterpriseList(list));

	remove_EnterpriseList(list, 2);
	remove_EnterpriseList(list, 4);
	remove_EnterpriseList(list, 4);
	printf("List size£¨after Delete£© is: %d\n", getSize_EnterpriseList(list));

	s_EnterpriseList s23 = { NULL, "ccc",23 };
	modify_EnterpriseList(list, 3, (enterpriseListNode*)&s23);

	printf("S1 Index is: %d\n", getIndex_EnterpriseList(list, (enterpriseListNode*)&s1));
	printf("S5 Index is: %d\n", getIndex_EnterpriseList(list, (enterpriseListNode*)&s5));

	print_EnterpriseList(list, printfun_List);

	free_EnterpriseList(list);
}

void circleListTest()
{
	s_List s1 = { "aaa",11 };
	s_List s2 = { "bbb",12 };
	s_List s3 = { "ccc",13 };
	s_List s4 = { "ddd",14 };
	s_List s5 = { "eee",15 };
	s_List s6 = { "fff",16 };
	s_List s7 = { "ggg",17 };
	s_List s8 = { "hhh",18 };

	circleLinkList* clist = init_CircleLinkList();
	add_CircleLinkList(clist, 1, &s1);
	add_CircleLinkList(clist, 2, &s2);
	add_CircleLinkList(clist, 3, &s3);
	add_CircleLinkList(clist, 4, &s4);
	add_CircleLinkList(clist, 5, &s5);
	add_CircleLinkList(clist, 6, &s6);
	add_CircleLinkList(clist, 7, &s7);
	add_CircleLinkList(clist, 8, &s8);

	remove_CircleLinkList(clist,4);

	print_CircleLinkList(clist, printfun_List);

	printf("linklist size = %d, index = %d\n", getSize_CircleLinkList(clist), getIndex_CircleLinkList(clist,&s8));
	s_List* s = (s_List*)getData_CircleLinkList(clist,3);
	printf("Name:%s\t Age:%d\n", s->name, s->age);

	free_CircleLinkList(clist);
}

int main(int argc, char* argv[])
{
	//linkListTest();

	//enterpriseListTest();

	circleListTest();
}

