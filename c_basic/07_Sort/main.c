#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>	//利用时间生成种子
#include <math.h>
#include <sys/timeb.h>
#include "sort.h"

#define LENGTH 250000

//获取系统时间,精确到毫秒
long long getSystemTime()
{
	struct timeb t;
	ftime(&t);
	return 1000 * t.time + t.millitm ;
}

//生成随机数
void randNumber(int* array, int len)
{
	int i;
	srand((unsigned)time(NULL));
	for (i = 0; i < len; ++i)
	{
		array[i] = rand() % len;
	}
}

void printArray(long* array, int length)
{
	for (int i = 0; i < length; i++)
	{
		printf("Array[%d] = %d\n",i,array[i]);
	}
	printf("==================================================\n");
}

int main(int argc, char* argv[])
{
	long long start, end;
	int array[LENGTH];

	////冒泡排序
	//randNumber(array, LENGTH);
	//start = getSystemTime();
	//bubbleSort(array,LENGTH);
	//end = getSystemTime();
	//printf("冒泡排序耗时：%lld ms\n",end-start);

	////选择排序
	//randNumber(array, LENGTH);
	//start = getSystemTime();
	//selectionSort(array,LENGTH);
	//end = getSystemTime();
	//printf("选择排序耗时：%lld ms\n", end - start);
	//
	////插入排序
	//randNumber(array, LENGTH);
	//start = getSystemTime();
	//insertionSort(array,LENGTH);
	//end = getSystemTime();
	//printf("插入排序耗时：%lld ms\n", end - start);

	////希尔排序（升序）
	//randNumber(array, LENGTH);
	//start = getSystemTime();
	//shellSort(array,LENGTH);
	//end = getSystemTime();
	//printf("希尔排序耗时：%lld ms\n", end - start);

	////归并排序
	//randNumber(array, LENGTH);
	////printArray(array, LENGTH);
	//start = getSystemTime();
	//mergeSort(array, LENGTH);
	//end = getSystemTime();
	//printf("归并排序耗时：%lld ms\n", end - start);


	//array[0] = 100; array[1] = 10; array[2] = 200; array[3] = 30; 
	//array[4] = 40; array[5] = 500; array[6] = 80; array[7] = 90;

	//快速排序
	randNumber(array, LENGTH);
	//printArray(array, LENGTH);
	start = getSystemTime();
	quickSort(array, LENGTH);
	end = getSystemTime();
	printf("快速排序耗时：%lld ms\n", end - start);

	//三向切分快速排序
	randNumber(array, LENGTH);
	//printArray(array, LENGTH);
	start = getSystemTime();
	quickSort_3Way(array, LENGTH);
	end = getSystemTime();
	printf("三向切分快速排序耗时：%lld ms\n", end - start);


	//打印排序后的数组
	//printArray(array, LENGTH);

	return 0;
}

