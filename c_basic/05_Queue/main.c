
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "seqQueue.h"
#include "linkQueue.h"

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
employee employee4 = { "DDD",38,Manager };
employee employee5 = { "EEE",20,Salesman };
employee employee6 = { "FFF",20,Salesman };
employee employee7 = { "GGG",20,SalesManager };
employee employee8 = { "HHH",50,CEO };

void seqQueueTest()
{
	seqQueue* queue = init_seqQueue(10);
	push_seqQueue(queue, &employee1);
	push_seqQueue(queue, &employee2);
	push_seqQueue(queue, &employee3);
	push_seqQueue(queue, &employee4);
	push_seqQueue(queue, &employee5);
	push_seqQueue(queue, &employee6);
	push_seqQueue(queue, &employee7);
	push_seqQueue(queue, &employee8);

	while (front_seqQueue(queue) != NULL)
	{
		employee* employe = (employee*)front_seqQueue(queue);
		printf("Name = %s, Age = %d, Title = %d", employe->name, employe->age, employe->title);
		printf("\n");
		pop_seqQueue(queue);
	}

	clear_seqQueue(queue);

	push_seqQueue(queue, &employee3);
	push_seqQueue(queue, &employee6);

	printf("-------------\n");
	while(front_seqQueue(queue)!= NULL)
	{
		employee* employe = (employee*)front_seqQueue(queue);
		printf("Name = %s, Age = %d, Title = %d\n", employe->name, employe->age, employe->title);
		pop_seqQueue(queue);
	}

	free_seqQueue(queue);
}

void linkqueueTest()
{
	linkQueue* queue =  init_linkQueue();
	push_linkQueue(queue, &employee1);
	push_linkQueue(queue, &employee2);
	push_linkQueue(queue, &employee3);
	push_linkQueue(queue, &employee4);
	push_linkQueue(queue, &employee5);
	push_linkQueue(queue, &employee6);
	push_linkQueue(queue, &employee7);
	push_linkQueue(queue, &employee8);


	while (front_linkQueue(queue) != NULL)
	{
		employee* employe_front = (employee*)front_linkQueue(queue);
		printf("Name = %s, Age = %d, Title = %d\n", employe_front->name, employe_front->age, employe_front->title);

		printf("-----------------------------\n");

		employee* employe_back = (employee*)back_linkQueue(queue);
		printf("Name = %s, Age = %d, Title = %d\n", employe_back->name, employe_back->age, employe_back->title);

		pop_linkQueue(queue);
		printf("linkQueue size = %d\n\n", getSize_linkQueue(queue));
	}

	free_linkQueue(queue);
}

int main(int argc, char* argv[])
{
	//seqQueueTest();

	linkqueueTest();

	struct MyStruct
	{
		unsigned char arg1;
		unsigned int arg2;
		double arg4;
		int arg5;
		unsigned long long arg3;
	}mystruct;

	printf("mystruct size = %d\n", sizeof(mystruct));
	return 0;
}

