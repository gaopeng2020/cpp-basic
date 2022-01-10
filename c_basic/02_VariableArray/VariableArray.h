#ifndef _VARIABLEARRAY_H
#define _VARIABLEARRAY_H
#include <stdlib.h>
#include <stdio.h>

typedef float DATATYPE_1DARRAY;
typedef struct VARIABLEARRAY
{
	unsigned int* len;
	DATATYPE_1DARRAY* varArray[];

}variableArray;

typedef struct VARIABLEARRAY2
{
	unsigned int len;
	DATATYPE_1DARRAY varArray[];

}variableArray2;

//���庯��ָ�뷽���û����ݲ�ͬ���������ͽ��д�ӡ
typedef void(*PRINTVARIABLEARRAY)(unsigned int , DATATYPE_1DARRAY*);

//��ʼ�䳤����
variableArray* initialize_VariableArray(unsigned int* len);

//�����ı�䳤����ָ��λ�õ�����
void setValue_VariableArray(variableArray* array, unsigned int index, DATATYPE_1DARRAY data);

//���ұ䳤������ָ��������������
DATATYPE_1DARRAY* getValue_VariableArray(variableArray* array, unsigned int index);

//��ѯĳ�������ڱ䳤�����е�λ��
unsigned int findIndex_VariableArray(variableArray* array, DATATYPE_1DARRAY data);

//�ӱ䳤�����ָ��λ��ɾ������
void removeValue_VariableArray(variableArray* array, unsigned int index);

//��ȡ�䳤����ĳ���
unsigned int length_VariableArray(variableArray* array);

//��ӡ�䳤�����е�����
void print_VariableArray(variableArray* array, PRINTVARIABLEARRAY printFunc);

//�ͷű䳤�����ڶ��з�����ڴ�ռ�
void free_VariableArray(variableArray* array);

#endif // !_VARIABLEARRAY_H
