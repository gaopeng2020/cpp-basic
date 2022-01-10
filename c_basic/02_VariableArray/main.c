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
	//初始化
	unsigned int len = 20;
	variableArray* varArray = initialize_VariableArray(&len);

	//插入元素
	for (unsigned int i = 0; i < *varArray->len; i++)
	{
		float data = i + 0.0001f;
		setValue_VariableArray(varArray, i, (DATATYPE_1DARRAY)(data));
	}

	//查找变长数组中指定索引处的数据
	unsigned index = 5;
	float* value = (float*)getValue_VariableArray(varArray, index);
	printf("array[%u]=%.4lf\n", index, *value);

	//查询某个数据在变长数组中的位置
	unsigned int idex = findIndex_VariableArray(varArray,8.0001f);
	printf("Index = %u\n", idex);

	//从变长数组的指定位置删除数据
	removeValue_VariableArray(varArray, index);

	//获取变长数组的长度
	printf("varArray length = %u\n", length_VariableArray(varArray));

	//打印数组中的元素
	print_VariableArray(varArray, prinVarArray);

	//释放内存
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
	//初始化
	unsigned int len_Row = 10, len_Col = 10;
	fullyFlexibleArray * array = initialize_fullyFlexibleArray(&len_Row, &len_Col);

	//插入元素
	for (unsigned int i = 0; i < len_Row; i++)
	{
		for (unsigned int j = 0; j < len_Col; j++)
		{
			setValue_fullyFlexibleArray(array, i, j, (DATATYPE_2DARRAY)(i + j));
		}
	}

	//查找变长数组中指定索引处的数据
	unsigned int index_Row = 3, index_Col = 3;
	int* value = (int*)getValue_fullyFlexibleArray(array, index_Row, index_Col);
	printf("array[%u,%u] = %d\n", index_Row , index_Col, *value);

	//查询某个数据在变长数组中第一次出现的位置
	index_2DArray index = findIndex_fullyFlexibleArray(array, compareArrayValue);
	printf("[%u,%u]\n", index.index_Row, index.index_Col);

	//从变长数组的指定位置删除数据
	removeValue_fullyFlexibleArray(array, index_Row, index_Col);

	//打印变长二维数组
	print_fullyFlexibleArray(array, printFullyFlexbleVarArray);

	//释放变长数组在堆中分配的内存空间
	free_fullyFlexibleArray(array);
}

int main(int argc, char* argv[])
{
	//variableArrayTest();

	fullyFlexibleVariableArrayTest();
	return 0;
}
