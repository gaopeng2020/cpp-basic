#include "VariableArray.h"

variableArray* initialize_VariableArray(unsigned int* len)
{
	variableArray* array = (variableArray*)malloc(sizeof(variableArray)+sizeof(DATATYPE_1DARRAY*)**len);
	*array->len = len;
	return array;
}

void setValue_VariableArray(variableArray* array, unsigned int index, DATATYPE_1DARRAY value)
{
	if (array == NULL)
	{
		return;
	}
	if (index < 0 || index > * array->len)
	{
		return;
	}
	array->varArray[index] = (DATATYPE_1DARRAY*)malloc(sizeof(DATATYPE_1DARRAY));
	*array->varArray[index] = value;
}

DATATYPE_1DARRAY* getValue_VariableArray(variableArray* array, unsigned int index)
{
	return array->varArray[index];
}

unsigned int findIndex_VariableArray(variableArray* array, DATATYPE_1DARRAY data)
{
	for (unsigned int i = 0; i < *array->len; i++)
	{
		if (*array->varArray[i] = data)
			return i;
	}
	return -1;
}

void removeValue_VariableArray(variableArray* array, unsigned int index)
{
	if (array == NULL || index < 0 || index > *array->len)
	{
		return;
	}

	array->varArray[index] = NULL;
}

unsigned int length_VariableArray(variableArray* array)
{
	return *array->len;
}

void print_VariableArray(variableArray* array, PRINTVARIABLEARRAY printFunc)
{
	for (unsigned int i = 0; i < *array->len; i++)
	{
		if (array->varArray[i] == NULL)
		{
			continue;
		}
		printFunc(i,array->varArray[i]);
	}
}

void free_VariableArray(variableArray* array)
{
	for (unsigned int i = 0; i < *array->len; i++)
	{
		if (array->varArray[i]!=NULL)
		{
			//printf("varArray[%u]正在释放内存\n",i);
			free(array->varArray[i]);
		}
	}

	if (array!=NULL)
	{
		//printf("struct正在释放内存\n");
		free(array);
	}
}



