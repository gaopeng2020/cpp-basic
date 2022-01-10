#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "VariableArray.h"
#include "fullyFlexibleVariableArray.h"

void prinVarArray(unsigned int index, DATATYPE_1DARRAY* data)
{
	float *value = (float*)data;
	printf("array[%u]=%.4lf\n", index ,*value);
}


void variableArrayTest()
{
	//��ʼ��
	unsigned int len = 20;
	variableArray* varArray = initialize_VariableArray(&len);

	//����Ԫ��
	for (unsigned int i = 0; i < *varArray->len; i++)
	{
		float data = i + 0.0001f;
		setValue_VariableArray(varArray, i, (DATATYPE_1DARRAY)(data));
	}

	//���ұ䳤������ָ��������������
	unsigned index = 5;
	float* value = (float*)getValue_VariableArray(varArray, index);
	printf("array[%u]=%.4lf\n", index, *value);

	//��ѯĳ�������ڱ䳤�����е�λ��
	unsigned int idex = findIndex_VariableArray(varArray,8.0001f);
	printf("Index = %u\n", idex);

	//�ӱ䳤�����ָ��λ��ɾ������
	removeValue_VariableArray(varArray, index);

	//��ȡ�䳤����ĳ���
	printf("varArray length = %u\n", length_VariableArray(varArray));

	//��ӡ�����е�Ԫ��
	print_VariableArray(varArray, prinVarArray);

	//�ͷ��ڴ�
	free_VariableArray(varArray);
}

void printFullyFlexbleVarArray(unsigned int len_Row, unsigned int len_Col, DATATYPE_2DARRAY* data)
{
	unsigned int* value = (unsigned int*)data;
	printf("array[%u,%u] = %u\t", len_Row, len_Col, *value);
}

int compareArrayValue(DATATYPE_2DARRAY* data)
{
	if (*(int*)data == 5)
	{
		return 1;
	}
	return 0;
}

void fullyFlexibleVariableArrayTest()
{
	//��ʼ��
	unsigned int len_Row = 10, len_Col = 10;
	fullyFlexibleArray * array = initialize_fullyFlexibleArray(&len_Row, &len_Col);

	//����Ԫ��
	for (unsigned int i = 0; i < len_Row; i++)
	{
		for (unsigned int j = 0; j < len_Col; j++)
		{
			setValue_fullyFlexibleArray(array, i, j, (DATATYPE_2DARRAY)(i + j));
		}
	}

	//���ұ䳤������ָ��������������
	unsigned int index_Row = 3, index_Col = 3;
	int* value = (int*)getValue_fullyFlexibleArray(array, index_Row, index_Col);
	printf("array[%u,%u] = %d\n", index_Row , index_Col, *value);

	//��ѯĳ�������ڱ䳤�����е�һ�γ��ֵ�λ��
	index_2DArray index = findIndex_fullyFlexibleArray(array, compareArrayValue);
	printf("[%u,%u]\n", index.index_Row, index.index_Col);

	//�ӱ䳤�����ָ��λ��ɾ������
	removeValue_fullyFlexibleArray(array, index_Row, index_Col);

	//��ӡ�䳤��ά����
	print_fullyFlexibleArray(array, printFullyFlexbleVarArray);

	//�ͷű䳤�����ڶ��з�����ڴ�ռ�
	free_fullyFlexibleArray(array);
}

int main(int argc, char* argv[])
{
	//variableArrayTest();

	fullyFlexibleVariableArrayTest();
	return 0;
}
