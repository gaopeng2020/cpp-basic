
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include<malloc.h>

//ָ��������������
typedef int P1[10];
typedef int(*P2)[10];

void simplePointer()
{
	/*	ָ���Ǳ���������ʲô���͵�ָ�����С����һ���ģ�
		32λϵͳ��ָ���СΪ4�ֽڣ�64λϵͳָ���СΪ8�ֽڣ�
		ָ��ָ��ĳ���������ڴ��ַ��*p����ָ���ڴ棬ͨ��*p���Ըı䱻ָ��������ڴ�ֵ*/
	int a = 10;
	int* p;
	p = &a;
	*p = 100;
	printf("a=%d\n", a);

	//���ָ��ͬʱָ��ͬһ���ڴ��ַ
	int t = 10;
	int* pt = &t;
	*pt = 20;

	int* ptt = &t;
	*ptt = 30;
	printf("\n");
	printf("t=%d\t*pt=%d\t*ptt=%d\n", t, *pt, *ptt);
}

int* pointerAsFunctionReturnValue(int* a, int* b)
{
	//�������һ��(x-y)�������:x+rand%(y-x+1)
	srand((unsigned)time(NULL)); //ʹ�õ�ǰʱ����������������ʼ��,��Ҫinclude <time.h>
	*b = 100 + rand() % (200 - 100 + 1);
	*a += 100;

	/*�������н���������������ڲ���������оֲ����ݣ������ֲ��������ֲ��������ʽ������
	�������ص�ָ���뾡����Ҫָ����Щ����(����ʹ���β�)�������ں���ʹ�ù����п��ܻ���������ʱ���� */
	//int sum = (*a + *b);
	//return &sum;

	*b += *a;
	return b;
}

void strPresentation()
{
	//�ַ����ԡ�\0'����β���ߴ�Ϊ10�����������9���ַ�
	char names1[] = { 'j', 'a', 'c', 'k', '\0' };
	printf("name1=%s\n", names1);

	char names2[50] = "jack";
	printf("name2=%s\n", names2);

	char* names3 = "jack";
	printf("name3=%s\n", names3);

	char* str = "ABCD!@#$1234";

	//malloc(unsigned int size);�ڶ��з����СΪsize�Ŀռ�
	//calloc(unsigned int n, unsigned int size) �ڶ��з���n������Ϊsize�������ռ�
	//realloc(void *p, Size_t size)�ڶ���Ϊָ��p���·����СΪsize�Ŀռ�
	//char* names4 = (char *)calloc(50, sizeof(char));
	//names4 ="jack";
	//printf("name4=%s\n", names4);
	//free(names4);

	printf("\n");
}

void ctrlArrayValueByPointer()
{
	int* p, array[5], i;
	p = &array[0];
	//p = array;
	//����ָ���ƫ�ƶ����鸳ֵ
	for (i = 0; i < sizeof(array) / sizeof(array[0]); i++)
	{
		printf("Please input array[%d]\n", i);
		scanf_s("%d", p + i);
	}

	//����ָ���ƫ�ƶ�����ȡֵ
	printf("Array Value is:\n");
	for (i = 0; i < sizeof(array) / sizeof(array[0]); i++)
	{
		printf("array[%d]=%-4d\n", i, *(p + i));
	}
	printf("\n");
}

// c[] �ȼ���*c
//void intPointer(int c[])
void getPointerArrayValue(int* c)
{
	int* p[6];
	int n = sizeof(p) / sizeof(p[0]);
	int i = 0;
	//������Ԫ�ص�ֵ����ָ������
	for (i = 0; i < n; i++)
	{
		//ָ�����鸳ֵ����һ��
		//&c[i]��ʾc[i]�ĵ�ַ
		//p[i] = &c[i];

		//ָ�����鸳ֵ��������
		//c��ʾa[0]�ĵ�ַ��c+1��ʾa[1]�ĵ�ַ...
		p[i] = c + i;
	}
	//ָ�����������ȡֵ
	for (i = 0; i < n; i++)
	{
		printf("ָ������ȡֵ����һ��*p[%d]=%d\n", i, *p[i]);
		//p[i]�ȼ���*(p+1)
		printf("ָ������ȡֵ��������*p[%d]=%d\n", i, **(p + i));
		printf("\n");
	}
	free(p);
}


//charPointerArray(char* p[])
void setPointerArrayValue(int n, char** p)
{
	//int size = sizeof(p) / sizeof(*p);
	//printf("sizeof(p)=%d\n", sizeof(p));

	int i = 0;
	for (i = 0; i < n; i++)
	{
		//p�������������׵�ַ��*p������ָ���׵�ַ���ڴ棬
		//printf("%s\t", *(p + i));
		//p[]�ȼ���*p
		printf("%s\t", p[i]);
	}

}

void arrayPointer1Oper(P1 *ptr)
{
	printf("==============================================\n����ָ�뷽ʽ1:\n");
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

void arrayPointer2Oper(P2 ptr)
{
	printf("����ָ�뷽ʽ2:\n");
	int i = 0;
	for (i=0;i<(sizeof(*ptr)/sizeof(int));i++)
	{
		(*ptr)[i] = i;
	}

	for (i = 0; i < (sizeof(*ptr) / sizeof(int)); i++)
	{
		printf("*p[%d] = %d ",i,(*ptr)[i]);
	}
	printf("\n");
}

void variable1DimArray(const int *len)
{
	typedef struct VARIABLE1DARRAY
	{
		uint8_t len;
		float array[];
	}variable1DimArray;

	variable1DimArray *p = (variable1DimArray *)malloc(sizeof(variable1DimArray) + sizeof(float) * (*len));
	p->len = *len;

	printf("variable1DimArray Size = %d\n", sizeof(*p));

	for (int i = 0; i < p->len; i++)
	{
		//��׼C��������ʹ�ú�׺.��׺Ϊ��f����F��
		p->array[i] = i + 1.0f;
	}

	for (int i = 0; i < p->len; i++)
	{
		printf("%f\n", p->array[i]);
	}

	if (p!=NULL)
	{
		free(p);
	}
}

void fullyFlexibleVariableArray(const unsigned int* len_Dim1, const unsigned int* len_Dim2)
{
	typedef struct VARDIM2ARRAY
	{
		unsigned int dim2Len;
		double* dim2Array[];
	}varDim2Array;

	typedef struct FULLYFLEXIBLEVARIABLEARRAY
	{
		unsigned int dim1Len;
		varDim2Array* dim1Array[];
	}fullyFlexibleArray;

	fullyFlexibleArray* p1 = (fullyFlexibleArray*)malloc(sizeof(fullyFlexibleArray) + sizeof(varDim2Array) * (*len_Dim1));
	p1->dim1Len = *len_Dim1;

	for (unsigned int i = 0; i < p1->dim1Len; i++)
	{
		p1->dim1Array[i] = (varDim2Array*)malloc(sizeof(varDim2Array) + sizeof(double) * (*len_Dim2));
		p1->dim1Array[i]->dim2Len = *len_Dim2;
		for (unsigned short j = 0; j < p1->dim1Array[i]->dim2Len; j++)
		{
			double data = j + 1.0005;
			p1->dim1Array[i]->dim2Array[j] = &(data);
			printf("array[%u,%u]=%f\n", i, j, *p1->dim1Array[i]->dim2Array[j]);
		}

		printf("\n");
	}

	//�ͷŶ��ڴ�ռ�
	for (unsigned int i = 0; i < p1->dim1Len; i++)
	{
		if (p1->dim1Array[i] != NULL)
		{
			free(p1->dim1Array[i]);
			//printf("=========�����ͷ�dim2Array memory===========\n");
		}
	}
	if (p1 != NULL)
	{
		free(p1);
		//printf("=========�����ͷ�dim1Array memory===========\n");
	}

}

int functionPointer(int x, int y) {
	return (x * 10 + y);
}

int callBackFunction(int x, int y, int (*fp)(int a, int b))
{
	
	return fp(x,y);
}

int addition(int x , int y)
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


int main(int argc, char* argv[])
{
	//1. ��򵥵�ָ��
	//simplePointer();


	//2. ָ����Ϊ����ʵ���Լ�����ֵ 
	//Ұָ�룺ָ��ָ����һ����ַ�ǲ�ȷ���ı�����
	//����ָ�룺ָ��������ʼ������ָ��һ�����󣬸ö��������ˣ�����ָ��δ�ƿգ���ô�ͳ�������ָ�롣
	//����ָ���ʱ��Ͷ�����г�ʼ���������ʱ��֪���ó�ʼ����ʲôֵ�����Ȱ�ָ���ÿ�
	// �ڴ汻free��delete��ָ����ڴ��ָ�������ÿ�
	//ʹ��malloc����Ϊָ��pb�ڶ��ж�̬����һ��4�ֽ��ڴ棬������ʧ�ܷ��ؿ�
	int value = 100;
	int* ptr = (int*)malloc(sizeof(int));
	if (ptr != NULL)
	{
		//ȡ��ַ��Ϊ������ʵ�Σ���֮���õ���ָ���βΣ�����ָ��ʱ��ı��˵��ô�������ֵ
		int* sum = pointerAsFunctionReturnValue(&value, ptr);
		printf("sum=%d,b=%d\n", *sum, value);
	}
	free(ptr); //�ͷŶ��ж�̬�����4�ֽ��ڴ�
	ptr = NULL;


	////3. �ַ��������ֶ��巽��
	strPresentation();

	//===========================================================
	//1. ������ָ��Ĺ�ϵ:ָ��ָ��������Ԫ�ص�ַ��p=array����p=&array[0];
	//2. ͨ��ָ���ƫ�ƿ���ʵ�ֶ�����Ԫ�صĸ��ģ�
	//3. ������array[]��Ϊʵ�Σ�ʵ���ϴ��ݹ�ȥ����������Ԫ�ص�ַ��
	//4. ��˺������β���Ȼ������һ��ָ���ʾ������Ԫ�ص�ַ��
	//5. ָ�����飬�������飬������ÿ��Ԫ�ض���ָ�룻
	//===========================================================*/

	//4. ����ָ��ƫ��ȡ����Ԫ�ص�ֵ
	//ctrlArrayValueByPointer();

	//5. ָ�����鸳ֵ
	//int c[] = { 1,2,3,4,5,6 };
	//getPointerArrayValue(c);

	//ָ�����飬�������飬ÿ��Ԫ�ض���ָ��
	//[]��*���ȼ���,char* p[]��һ��ָ������

	//6. ָ������ȡֵ
	//char* p[] = { "A1","A2","A3","A4","A5" };
	//int n = sizeof(p) / sizeof(*p);
	//setPointerArrayValue(n, p);

	//7. ����ָ��: ����ָ�룬ָ��һ�������ָ��
	//��ʽ1���ȶ����������ͣ��ٶ���ָ������������ã�
	int array[10] = { 0 };
	//typedef int P1[10]; //A��������
	P1 *ptr1 = &array; //ָ�����
	arrayPointer1Oper(ptr1);

	//��ʽ2���ȶ�������ָ�����ͣ��������Ͷ������
	//typedef int(*P2)[10]; //�ȶ�������ָ������
	//������[]���ȼ�һ������������,��������ָ�룬��������һ��ָ�룬[]�����飬�������ָ�������ָ��;
	// ����typedef����������һ������ָ������
	P2 ptr2 = &array; //�ٶ���ָ�����
	arrayPointer2Oper(ptr2);

	//��ʽ3��ֱ�Ӷ�������ָ�����
	int i = 0;
	int(*ptr3)[10];
	ptr3 = &array;
	printf("����ָ�뷽ʽ3:\n");
	for(i = 0; i < (sizeof(*ptr3)/sizeof(int));i++){
		(*ptr3)[i] = i*30;
		printf("*ptr3[%d] = %d ", i, (*ptr3)[i]);
	}
	printf("\n");


	//8. ����ָ�룬����һ��ָ��ָ��һ�������������ֶ��巽ʽ
	//��ʽһ���ȶ��庯�����ͣ��ٶ��庯��ָ������������ã�
	typedef int FUNCTION_POINTER1(int x, int y); //�ȶ��庯������
	FUNCTION_POINTER1* fp1 = functionPointer; // �ٶ��庯��ָ�����
	int result = 0;
	result = fp1(10, 20);

	//��ʽ2���ȶ��庯��ָ�����ͣ��������Ͷ���ָ����������ã�
	typedef int (*FUNCTION_POINTER2)(int x, int y);
	FUNCTION_POINTER2 fp2 = functionPointer;
	result = fp2(20, 20);

	//��ʽ2��ֱ�Ӷ��庯��ָ�루���ã�
	int (*fp3)(int x, int y) = functionPointer;

	//����ָ���һ�����ã��ص���������������ʵ�ֺ͵��÷��룩
	printf("###########################################\n");
	result = callBackFunction(30, 20, fp3);
	printf("result = %d\n", result);


	//����ָ�����飺���������ŵ��Ǻ���ָ��
	//int x=0, y=0;
	//char operator[2];
	//char *buf[] = {"+","-","*","/"};
	//int (*calculator[4])(int x, int y) = {addition,subtraction ,multiplication,division};
	////scanf_s("%d%s%d", &x, operator,2, &y);//(���ַ�ʽ��ȡ�Ľ��������)
	////printf("x = %d, operator = %s,y = %d\n", x, operator,y);
	////scanf_s("%s",&operator,sizeof(operator));
	////printf("operator = %s\n",operator);

	//while (1) {
	//	printf("please input x\n");
	//	scanf_s("%d", &x);
	//	printf("please input operator\n");
	//	scanf_s("%s", &operator,sizeof(operator));
	//	printf("please input y\n");
	//	scanf_s("%d",&y);
	//	for (i = 0; i < 4; i++) {
	//		if (strcmp(buf[i], operator) == 0) {
	//			printf("result is : %d\n", calculator[i](x, y));
	//			printf("please input Ctrl+C if you want to eixt!!!\n");
	//			break;
	//		}
	//	}
	//}

	//9. �䳤һά����
	int len = 5;
	variable1DimArray(&len);

	unsigned int lenDim1 = 1, lemDim2 = 2;
	fullyFlexibleVariableArray(&lenDim1, &lemDim2);

	return 0;
}


