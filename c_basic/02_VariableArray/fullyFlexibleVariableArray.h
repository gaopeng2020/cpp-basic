#ifndef _FULLYFLEXIBLEfullyFlexibleArray_H
#define _FULLYFLEXIBLEfullyFlexibleArray_H
#include <stdlib.h>
#include <stdio.h>

//数组数据类型定义
typedef int DATATYPE_2DARRAY;

//fully Flexible Array 数据结构
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
	定义对比函数指针用于对比用户自定义的数据
	若相等则返回1，否则返回0，返回其他数认为对比结果不相等
*/
typedef int(*COMPAREVALUE)(DATATYPE_2DARRAY*);
typedef struct INDEX_2DARRAY
{
	int index_Row;
	int index_Col;
}index_2DArray;

//定义打印函数指针方便用户根据不同的数据类型进行打印
typedef void(*PRINTFULLYFLEXIBLEARRAY)(unsigned int,unsigned int, DATATYPE_2DARRAY*);

//初始变长数组
fullyFlexibleArray* initialize_fullyFlexibleArray(unsigned int* len_Row, unsigned int* len_Col);

//插入或改变变长数组指定位置的数据
void setValue_fullyFlexibleArray(fullyFlexibleArray * array, unsigned int index_Row, unsigned int index_Col, DATATYPE_2DARRAY data);

//查找变长数组中指定索引处的数据
DATATYPE_2DARRAY* getValue_fullyFlexibleArray(fullyFlexibleArray * array, unsigned int index_Row, unsigned int index_Col);

//查询某个数据在变长数组中第一次出现的位置
index_2DArray findIndex_fullyFlexibleArray(fullyFlexibleArray * array,COMPAREVALUE compareFun);

//从变长数组的指定位置删除数据
void removeValue_fullyFlexibleArray(fullyFlexibleArray * array, unsigned int index_Row, unsigned int index_Col);

//获取变长数组的长度
//index_2DArray length_fullyFlexibleArray(fullyFlexibleArray * array);

//打印变长数组中的数据
void print_fullyFlexibleArray(fullyFlexibleArray * array, PRINTFULLYFLEXIBLEARRAY printFun);

//释放变长数组在堆中分配的内存空间
void free_fullyFlexibleArray(fullyFlexibleArray * array);


#endif // _FULLYFLEXIBLEfullyFlexibleArray_H

