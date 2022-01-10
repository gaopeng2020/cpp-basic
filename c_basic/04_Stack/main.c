#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "seqStack.h"
#include "linkStack.h"

typedef enum JOBTITLE
{
	Engineer, Manager, Salesman, SalesManager, CEO
}jobTitle;

typedef struct EMPLOYEE
{
	char name[64];
	int age;
	jobTitle title;
}employee;

employee employee1 = { "AAA",25,Engineer };
employee employee2 = { "BBB",23,Engineer };
employee employee3 = { "CCC",28,Engineer };
employee employee4 = { "CCC",38,Manager };
employee employee5 = { "CCC",20,Salesman };
employee employee6 = { "CCC",20,Salesman };
employee employee7 = { "DDD",20,SalesManager };
employee employee8 = { "EEE",50,CEO };

void seqStackTest()
{
	seqStack* stack = init_seqStack(10);
	push_seqStack(stack, &employee1);
	push_seqStack(stack, &employee2);
	push_seqStack(stack, &employee3);
	push_seqStack(stack, &employee4);
	push_seqStack(stack, &employee5);
	push_seqStack(stack, &employee6);
	push_seqStack(stack, &employee7);
	push_seqStack(stack, &employee8);

	for (int i = stack->size; i >= 0; i--)
	{
		employee* employe = (employee*)top_seqStack(stack);
		printf("Name = %s, Age = %d, Title = %d", employe->name, employe->age, employe->title);
		printf("\n");
		pop_seqStack(stack);
	}

	clear_seqStack(stack);
	for (int i = stack->size; i >= 0; i--)
	{
		employee* employe = (employee*)top_seqStack(stack);
		printf("Name = %s, Age = %d, Title = %d", employe->name, employe->age, employe->title);
		printf("\n");
		pop_seqStack(stack);
	}

	free_seqStack(stack);
}

void linkStackTest()
{
	linkStack* stack = init_linkStack();
	push_linkStack(stack, &employee1);
	push_linkStack(stack, &employee2);
	push_linkStack(stack, &employee3);
	push_linkStack(stack, &employee4);
	push_linkStack(stack, &employee5);
	push_linkStack(stack, &employee6);
	push_linkStack(stack, &employee7);
	push_linkStack(stack, &employee8);

	while (stack->head.next != NULL)
	{
		employee* employe = (employee*)top_linkStack(stack);
		printf("Name = %s, Age = %d, Title = %d\n", employe->name, employe->age, employe->title);
		pop_linkStack(stack);
		printf("stack size = %d\n", getSize_linkStack(stack));
	}

	clear_linkStack(stack);
	printf("stack size = %d\n", getSize_linkStack(stack));

	free_linkStack(stack);
}

int main(int argc, char* argv[])
{
	//seqStackTest();
	linkStackTest();

	return 0;
}

