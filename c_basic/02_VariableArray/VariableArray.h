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

//定义函数指针方便用户根据不同的数据类型进行打印
typedef void(*PRINTVARIABLEARRAY)(unsigned int , DATATYPE_1DARRAY*);

//初始变长数组
variableArray* initialize_VariableArray(unsigned int* len);

//插入或改变变长数组指定位置的数据
void setValue_VariableArray(variableArray* array, unsigned int index, DATATYPE_1DARRAY data);

//查找变长数组中指定索引处的数据
DATATYPE_1DARRAY* getValue_VariableArray(variableArray* array, unsigned int index);

//查询某个数据在变长数组中的位置
unsigned int findIndex_VariableArray(variableArray* array, DATATYPE_1DARRAY data);

//从变长数组的指定位置删除数据
void removeValue_VariableArray(variableArray* array, unsigned int index);

//获取变长数组的长度
unsigned int length_VariableArray(variableArray* array);

//打印变长数组中的数据
void print_VariableArray(variableArray* array, PRINTVARIABLEARRAY printFunc);

//释放变长数组在堆中分配的内存空间
void free_VariableArray(variableArray* array);

#endif // !_VARIABLEARRAY_H
