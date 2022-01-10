#include "fullyFlexibleVariableArray.h"


fullyFlexibleArray* initialize_fullyFlexibleArray(unsigned int* len_Row, unsigned int* len_Col)
{
	fullyFlexibleArray* rowArray = (fullyFlexibleArray*)malloc(sizeof(fullyFlexibleArray) + sizeof(varDim2Array*) * *len_Row);
	rowArray->len_Row = len_Row;
	for (unsigned int i = 0; i < *len_Row; i++)
	{
		rowArray->rowArray[i] = (varDim2Array*)malloc(sizeof(varDim2Array) + sizeof(DATATYPE_2DARRAY*) * *len_Col);
		rowArray->rowArray[i]->len_Col = len_Col;
	}
	return rowArray;
}

void setValue_fullyFlexibleArray(fullyFlexibleArray* array, unsigned int index_Row, unsigned int index_Col, DATATYPE_2DARRAY data)
{
	if (array == NULL)
	{
		return;
	}
	if (index_Row < 0 || index_Row > * array->len_Row)
	{
		return;
	}
	if (index_Col < 0 || index_Col > * array->rowArray[index_Row]->len_Col)
	{
		return;
	}
	//为传递进来的数据分配内存空间
	array->rowArray[index_Row]->colArray[index_Col] = (DATATYPE_2DARRAY*)malloc(sizeof(DATATYPE_2DARRAY));
	*array->rowArray[index_Row]->colArray[index_Col] = data;
}

DATATYPE_2DARRAY* getValue_fullyFlexibleArray(fullyFlexibleArray* array, unsigned int index_Row, unsigned int index_Col)
{
	if (array == NULL)
	{
		return NULL;
	}
	if (index_Row < 0 || index_Row > * array->len_Row)
	{
		return NULL;
	}
	if (index_Col < 0 || index_Col > * array->rowArray[index_Row]->len_Col)
	{
		return NULL;
	}

	return array->rowArray[index_Row]->colArray[index_Col];
}

index_2DArray findIndex_fullyFlexibleArray(fullyFlexibleArray* array, COMPAREVALUE compareFun)
{
	index_2DArray invalidIndex = { -1,-1 };
	if (array == NULL)
	{
		return invalidIndex;
	}

	for (unsigned int i = 0; i < *array->len_Row; i++)
	{
		for (unsigned int j = 0; j < *array->rowArray[i]->len_Col; j++)
		{
			if (compareFun(array->rowArray[i]->colArray[j]) == 1)
			{
				index_2DArray arrayIndex = { i,j };
				return arrayIndex;
			}
		}
	}
	return invalidIndex;
}

void removeValue_fullyFlexibleArray(fullyFlexibleArray* array, unsigned int index_Row, unsigned int index_Col)
{
	if (array == NULL)
	{
		return;
	}
	if (index_Row < 0 || index_Row > * array->len_Row)
	{
		return;
	}
	if (index_Col < 0 || index_Col > * array->rowArray[index_Row]->len_Col)
	{
		return;
	}
	array->rowArray[index_Row]->colArray[index_Col] = NULL;
}

//index_2DArray length_fullyFlexibleArray(fullyFlexibleArray* array)
//{
//	index_2DArray arrayIndex = {*array->len_Row,};
//	return 0;
//}

void print_fullyFlexibleArray(fullyFlexibleArray* array, PRINTFULLYFLEXIBLEARRAY printFunc)
{
	if (array == NULL)
	{
		return;
	}
	for (unsigned int i = 0; i < *array->len_Row; i++)
	{
		for (unsigned int j = 0; j < *array->rowArray[i]->len_Col; j++)
		{
			if (array->rowArray[i]->colArray[j] == NULL)
			{
				continue;
			}
			printFunc(i, j, array->rowArray[i]->colArray[j]);
		}
		printf("\n");
	}
}

void free_fullyFlexibleArray(fullyFlexibleArray* array)
{
	if (array == NULL)
	{
		return;
	}

	for (unsigned int i = 0; i < *array->len_Row; i++)
	{
		//先释放数组中的数据
		for (unsigned int j = 0; j < *array->rowArray[i]->len_Col; j++)
		{
			if (array->rowArray[i]->colArray[j] != NULL)
			{
				free(array->rowArray[i]->colArray[j]);
			}
		}

		//在释放第二维度的数组
		if (array->rowArray[i] != NULL)
		{
			free(array->rowArray[i]);
		}
	}
	//最后释放数组
	free(array);
}
