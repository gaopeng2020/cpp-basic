#ifndef _FULLYFLEXIBLEfullyFlexibleArray_H
#define _FULLYFLEXIBLEfullyFlexibleArray_H
#include <stdlib.h>
#include <stdio.h>

//�����������Ͷ���
typedef int DATATYPE_2DARRAY;

//fully Flexible Array ���ݽṹ
typedef struct VARDIM2ARRAY
{
	unsigned int* len_Col;
	DATATYPE_2DARRAY* colArray[];
}varDim2Array;

typedef struct FULLYFLEXIBLEfullyFlexibleArray
{
	unsigned int* len_Row;
	varDim2Array* rowArray[];
}fullyFlexibleArray;

/*
	����ԱȺ���ָ�����ڶԱ��û��Զ��������
	������򷵻�1�����򷵻�0��������������Ϊ�ԱȽ�������
*/
typedef int(*COMPAREVALUE)(DATATYPE_2DARRAY*);
typedef struct INDEX_2DARRAY
{
	int index_Row;
	int index_Col;
}index_2DArray;

//�����ӡ����ָ�뷽���û����ݲ�ͬ���������ͽ��д�ӡ
typedef void(*PRINTFULLYFLEXIBLEARRAY)(unsigned int,unsigned int, DATATYPE_2DARRAY*);

//��ʼ�䳤����
fullyFlexibleArray* initialize_fullyFlexibleArray(unsigned int* len_Row, unsigned int* len_Col);

//�����ı�䳤����ָ��λ�õ�����
void setValue_fullyFlexibleArray(fullyFlexibleArray * array, unsigned int index_Row, unsigned int index_Col, DATATYPE_2DARRAY data);

//���ұ䳤������ָ��������������
DATATYPE_2DARRAY* getValue_fullyFlexibleArray(fullyFlexibleArray * array, unsigned int index_Row, unsigned int index_Col);

//��ѯĳ�������ڱ䳤�����е�һ�γ��ֵ�λ��
index_2DArray findIndex_fullyFlexibleArray(fullyFlexibleArray * array,COMPAREVALUE compareFun);

//�ӱ䳤�����ָ��λ��ɾ������
void removeValue_fullyFlexibleArray(fullyFlexibleArray * array, unsigned int index_Row, unsigned int index_Col);

//��ȡ�䳤����ĳ���
//index_2DArray length_fullyFlexibleArray(fullyFlexibleArray * array);

//��ӡ�䳤�����е�����
void print_fullyFlexibleArray(fullyFlexibleArray * array, PRINTFULLYFLEXIBLEARRAY printFun);

//�ͷű䳤�����ڶ��з�����ڴ�ռ�
void free_fullyFlexibleArray(fullyFlexibleArray * array);


#endif // _FULLYFLEXIBLEfullyFlexibleArray_H

