#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>

// 指针数组类型声明
typedef int P1[10];
typedef int (*P2)[10];

void simplePointer()
{
	/*	指针是变量，无论什么类型的指针其大小都已一样的；
		32位系统，指针大小为4字节，64位系统指针大小为8字节；
		指针指向某个变量的内存地址，*p则是指向内存，通过*p可以改变被指向变量的内存值*/
	int a = 10;
	int *p;
	p = &a;
	*p = 100;
	printf("a=%d\n", a);

	// 多个指针同时指向同一个内存地址
	int t = 10;
	int *pt = &t;
	*pt = 20;

	int *ptt = &t;
	*ptt = 30;
	printf("\n");
	printf("t=%d\t*pt=%d\t*ptt=%d\n", t, *pt, *ptt);
}

int *pointerAsFunctionReturnValue(const int *a, const int *b)
{
	// int *sum = malloc(sizeof(int)); // 在堆中分配大小为size的空间
	// // 随机生成一个(x-y)的随机数:x+rand%(y-x+1)
	// srand((unsigned)time(NULL)); // 使用当前时间对随机数发生器初始化,需要include <time.h>
	// *sum = 100 + rand() % (*a + *b);

	/*函数运行结束后会销毁在它内部定义的所有局部数据，包括局部变量、局部数组和形式参数。
	函数返回的指针请尽量不要指向这些数据(可以使用形参)，它们在后续使用过程中可能会引发运行时错误 */
	int sum = (*a) + (*b);
	return &sum;

	// *b += *a;
	// return sum;
}

void strPresentation()
{
	// 字符串以“\0'”结尾，尺寸为10的数组最多存放9个字符
	char names1[] = {'j', 'a', 'c', 'k', '\0'};
	printf("name1=%s\n", names1);

	char names2[50] = "jack";
	printf("name2=%s\n", names2);

	char *names3 = "jack";
	printf("name3=%s\n", names3);

	char *str = "ABCD!@#$1234";
	printf("str=%s\n", str);

	// malloc(unsigned int size); //在堆中分配大小为size的空间
	// calloc(unsigned int n, unsigned int size);// 在堆中分配n个长度为size的连续空间
	// realloc(void *p, Size_t size);//在堆中为指针p重新分配大小为size的空间

	char *names4 = (char *)calloc(50, sizeof(char));
	names4 = "jack";
	printf("name4=%s\n", names4);
	free(names4);

	printf("\n");
}

void ctrlArrayValueByPointer()
{
	int *p, array[5], i;
	p = &array[0];
	// p = array;
	// 利用指针的偏移对数组赋值
	for (i = 0; i < sizeof(array) / sizeof(array[0]); i++)
	{
		printf("Please input array[%d]\n", i);
		scanf_s("%d", p + i);
	}

	// 利用指针的偏移对数组取值
	printf("Array Value is:\n");
	for (i = 0; i < sizeof(array) / sizeof(array[0]); i++)
	{
		printf("array[%d]=%-4d\n", i, *(p + i));
	}
	printf("\n");
}

// c[] 等价于*c
// void intPointer(int c[])
void getPointerArrayValue(int *c)
{
	int *p[6];
	int n = sizeof(p) / sizeof(p[0]);
	int i = 0;
	// 将数组元素的值赋给指针数组
	for (i = 0; i < n; i++)
	{
		// 指针数组赋值方法一：
		//&c[i]表示c[i]的地址
		// p[i] = &c[i];

		// 指针数组赋值方法二：
		// c表示a[0]的地址，c+1表示a[1]的地址...
		p[i] = c + i;
	}
	// 指针数组遍历，取值
	for (i = 0; i < n; i++)
	{
		printf("指针数组取值方法一：*p[%d]=%d\n", i, *p[i]);
		// p[i]等价于*(p+1)
		printf("指针数组取值方法二：*p[%d]=%d\n", i, **(p + i));
		printf("\n");
	}
	if (p != NULL)
	{
		free(p);
	}
}

// charPointerArray(char* p[])
void setPointerArrayValue(int n, char **p)
{
	// int size = sizeof(p) / sizeof(*p);
	// printf("sizeof(p)=%d\n", sizeof(p));

	int i = 0;
	for (i = 0; i < n; i++)
	{
		// p代表的是数组的首地址，*p代表则指向首地址的内存，
		// printf("%s\t", *(p + i));
		// p[]等价于*p
		printf("%s\t", p[i]);
	}
}

void arrayPointerTest(P1 *ptr)
{
	printf("==============================================\n数组指针方式1:\n");
	int i = 0;
	for (i = 0; i < (sizeof(*ptr) / sizeof(int)); i++)
	{
		(*ptr)[i] = i;
	}

	for (i = 0; i < (sizeof(*ptr) / sizeof(int)); i++)
	{
		printf("*p[%d] = %d ", i, (*ptr)[i]);
	}
	printf("\n");
}

void variable1DimArray(const int *len)
{
	typedef struct VARIABLE1DARRAY
	{
		uint8_t len;
		float array[];
	} variable1DimArray;

	variable1DimArray *p = (variable1DimArray *)malloc(sizeof(variable1DimArray) + sizeof(float) * (*len));
	p->len = *len;

	printf("variable1DimArray Size = %d\n", sizeof(*p));

	for (int i = 0; i < p->len; i++)
	{
		// 标准C允许浮点数使用后缀.后缀为“f”或“F”
		p->array[i] = i + 1.0f;
	}

	for (int i = 0; i < p->len; i++)
	{
		printf("%f\n", p->array[i]);
	}

	if (p != NULL)
	{
		free(p);
	}
}

void fullyFlexibleVariableArray(const unsigned int *len_Dim1, const unsigned int *len_Dim2)
{
	typedef struct VARDIM2ARRAY
	{
		unsigned int dim2Len;
		double *dim2Array[];
	} varDim2Array;

	typedef struct FULLYFLEXIBLEVARIABLEARRAY
	{
		unsigned int dim1Len;
		varDim2Array *dim1Array[];
	} fullyFlexibleArray;

	fullyFlexibleArray *p1 = (fullyFlexibleArray *)malloc(sizeof(fullyFlexibleArray) + sizeof(varDim2Array) * (*len_Dim1));
	p1->dim1Len = *len_Dim1;

	for (unsigned int i = 0; i < p1->dim1Len; i++)
	{
		p1->dim1Array[i] = (varDim2Array *)malloc(sizeof(varDim2Array) + sizeof(double) * (*len_Dim2));
		p1->dim1Array[i]->dim2Len = *len_Dim2;
		for (unsigned short j = 0; j < p1->dim1Array[i]->dim2Len; j++)
		{
			double data = j + 1.0005;
			p1->dim1Array[i]->dim2Array[j] = &(data);
			printf("array[%u,%u]=%f\n", i, j, *p1->dim1Array[i]->dim2Array[j]);
		}

		printf("\n");
	}

	// 释放堆内存空间
	for (unsigned int i = 0; i < p1->dim1Len; i++)
	{
		if (p1->dim1Array[i] != NULL)
		{
			free(p1->dim1Array[i]);
			// printf("=========正在释放dim2Array memory===========\n");
		}
	}
	if (p1 != NULL)
	{
		free(p1);
		// printf("=========正在释放dim1Array memory===========\n");
	}
}

int functionPointer(int x, int y)
{
	return (x * 10 + y);
}

int callBackFunction(int x, int y, int (*fp)(int a, int b))
{

	return fp(x, y);
}

int addition(int x, int y)
{
	return x + y;
}
int subtraction(int x, int y)
{
	return x - y;
}
int multiplication(int x, int y)
{
	return x * y;
}
int division(int x, int y)
{
	return x / y;
}

int main(int argc, char *argv[])
{
	// 1. 最简单的指针
	printf("Test1 -----------------------------------------------------------------\n");
	// simplePointer();

	// 2. 指针作为函数实参以及返回值
	// 野指针：指针指向了一个地址是不确定的变量；
	// 垂悬指针：指针正常初始化，曾指向一个对象，该对象被销毁了，但是指针未置空，那么就成了悬空指针。
	// 声明指针的时候就对其进行初始化，如果暂时不知道该初始化成什么值，就先把指针置空
	//  内存被free或delete后，指向该内存的指针马上置空
	// 使用malloc函数为指针pb在堆中动态分配一块4字节内存，若分配失败返回空
	printf("Test2 -----------------------------------------------------------------\n");
	int value = 100;
	int *ptr = (int *)malloc(sizeof(int));
	if (ptr != NULL)
	{
		// 取地址作为函数的实参，与之对用的是指针形参，操作指针时亦改变了调用处变量的值
		int *sum = NULL;
		sum = pointerAsFunctionReturnValue(&value, ptr);
		printf("Test2: sum=%d,b=%d\n", *sum, value);
		free(sum);
		sum = NULL;
	}
	free(ptr); // 释放堆中动态分配的4字节内存
	ptr = NULL;

	// 3. 字符串的三种定义方法
	printf("Test3 -----------------------------------------------------------------\n");
	strPresentation();

	//===========================================================
	// 1. 数组与指针的关系:指针指向数组首元素地址；p=array或者p=&array[0];
	// 2. 通过指针的偏移可以实现对数组元素的更改；
	// 3. 将数组array[]作为实参，实际上传递过去的是数组首元素地址；
	// 4. 因此函数的形参自然可以用一个指针表示数组首元素地址；
	// 5. 指针数组，它是数组，数组内每个元素都是指针；
	//===========================================================*/

	// 4. 利用指针偏移取数组元素的值
	printf("Test4 -----------------------------------------------------------------\n");
	ctrlArrayValueByPointer();

	// 5. 指针数组取值
	int c[] = {1, 2, 3, 4, 5, 6};
	printf("Test5 -----------------------------------------------------------------\n");
	getPointerArrayValue(c);

	// 指针数组，它是数组，每个元素都是指针
	//[]比*优先级高,char* p[]是一个指针数组

	// 6. 指针数组取值
	char *p[] = {"A1", "A2", "A3", "A4", "A5"};
	int n = sizeof(p) / sizeof(*p);
	setPointerArrayValue(n, p);

	// 7. 数组指针: 它是指针，指向一个数组的指针
	// 方式1：先定义数组类型，再定义指针变量（不常用）
	int array[10] = {0};
	typedef int P1[sizeof(array) / sizeof(array[0])]; // 数组类型
	P1 *ptr1 = &array;								  // 指针变量
	arrayPointerTest(ptr1);

	// 方式2：先定义数组指针类型，根据类型定义变量
	typedef int(*P2)[sizeof(array) / sizeof(array[0])]; // 先定义数组指针类型
	// （）和[]优先级一样，从左往右,（）内有指针，所以它是一个指针，[]是数组，因此它是指向数组的指针;
	//  它有typedef，所以它是一个数组指针类型
	P2 ptr2 = &array; // 再定义指针变量
	arrayPointerTest(ptr2);

	// 方式3：直接定义数组指针变量（常用）
	int(*ptr3)[sizeof(array) / sizeof(array[0])];
	ptr3 = &array;
	arrayPointerTest(ptr3);

	// 8. 函数指针，定义一个指针指向一个函数，有三种定义方式
	// 方式一：先定义函数类型，再定义函数指针变量（不常用）
	typedef int FUNCTION_POINTER1(int x, int y); // 先定义函数类型
	FUNCTION_POINTER1 *fp1 = functionPointer;	 // 再定义函数指针变量
	int result = 0;
	result = fp1(10, 20);

	// 方式2：先定义函数指针类型，根据类型定义指针变量（常用）
	typedef int (*FUNCTION_POINTER2)(int x, int y);
	FUNCTION_POINTER2 fp2 = functionPointer;
	result = fp2(20, 20);

	// 方式3：直接定义函数指针（常用）
	int (*fp3)(int x, int y) = functionPointer;

	// 函数指针的一个作用：回调函数（将函数的实现和调用分离）
	printf("###########################################\n");
	result = callBackFunction(30, 20, fp3);
	printf("result = %d\n", result);

	// 函数指针数组：数组里面存放的是函数指针
	int x = 0, y = 0,i=0;
	char operator[2];
	char *buf[] = {"+", "-", "*", "/"};
	int (*calculator[4])(int x, int y) = {addition, subtraction, multiplication, division};
	// scanf_s("%d%s%d", &x, operator,2, &y);//(这种方式获取的结果有问题)
	// printf("x = %d, operator = %s,y = %d\n", x, operator,y);
	// scanf_s("%s",&operator,sizeof(operator));
	// printf("operator = %s\n",operator);

	while (1)
	{
		printf("please input x\n");
		scanf_s("%d", &x);
		printf("please input operator\n");
		scanf_s("%s", &operator, sizeof(operator));
		printf("please input y\n");
		scanf_s("%d", &y);
		for (i = 0; i < 4; i++)
		{
			if (strcmp(buf[i], operator) == 0)
			{
				printf("result is : %d\n", calculator[i](x, y));
				printf("please input Ctrl+C if you want to eixt!!!\n");
				break;
			}
		}
	}

	// 9. 变长一维数组
	// int len = 5;
	// variable1DimArray(&len);

	// unsigned int lenDim1 = 1, lemDim2 = 2;
	// fullyFlexibleVariableArray(&lenDim1, &lemDim2);

	return 0;
}
