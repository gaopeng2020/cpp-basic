#include "sort.h"
//交换数组内指定索引的两个元素
void swapArray(int* array, int arg1Index, int arg2Index)
{
	if (array == NULL) return;
	int tmp = array[arg1Index];
	array[arg1Index] = array[arg2Index];
	array[arg2Index] = tmp;
}

/*
1. 比较相邻的元素。如果第一个比第二个大，就交换他们两个。
2. 对每一对相邻元素作同样的工作，从开始第一对到结尾的最后一对。在这一点，最后的元素应该会是最大的数。
3. 针对所有的元素重复以上的步骤，除了最后一个。
4. 持续每次对越来越少的元素重复上面的步骤，直到没有任何一对数字需要比较。
稳定性:冒泡排序是一种稳定排序算法
*/
//冒泡排序(升序)
void bubbleSort(int* array, int length)	//O(n²)
{
	if (array == NULL || length < 2)
	{
		return;
	}
	for (int i = 0; i < length; ++i) {
		for (int j = 1; j < length - i; j++) {
			if (array[j] < array[j - 1])
			{
	/*			int temp = array[j];
				array[j] = array[j - 1];
				array[j - 1] = temp;*/
				swapArray(array,j,j-1);
			}
		}
	}
}


/* 升序排列，选择最小者。
1. 找到数组中最小的元素min；
2. 将最小元素与第一个交换位置；
3. 找到剩余数组中最小元素将其与第二个元素交换位置
4. 依次选择数组中剩余元素中的最小者。。。
*/
void  selectionSort(int* array, int length)
{
	if (array == NULL || length < 2)
	{
		return;
	}
	for (int i = 0; i < length; i++) {
		int minIndex = i;
		for (int j = i + 1; j < length; j++) {
			if (array[i] > array[j])
			{
				minIndex = j;
			}

			//只有在后一个元素大于当前元素时才交换（减少交换次数）；
			if (minIndex != i)
			{
				//int temp = array[i];
				//array[i] = array[j];
				//array[j] = temp;
				swapArray(array,i ,j);
			}
		}
	}
}


/*
1. 升序排列，与扑克牌排序类似
2. 将当前数据与之前的数据数据依次比较，
3. 若当前数据比前一个小，则先缓存当前数据，并之前所有小于当前数据的数据向右移动一位
4. 将当前数据插入到合适的位置；
*/
void insertionSort(int* array, int length)
{
	if (array == NULL || length < 2)
	{
		return;
	}
	for (int i = 1; i < length; i++) {
		int insertIndex = i;
		//缓存当前数据
		int currentData = array[i];
		for (int j = i - 1; j >= 0; j--) {
			if (currentData < array[j])
			{//当前数据比前一个小，则将之前所有小于当前数据的数据向右移动一位
				array[j + 1] = array[j];
				insertIndex = j;
			}
			else {
				break;
			}
		}
		//将当前的元素插入到合适的位置中
		array[insertIndex] = currentData;
	}
}

/*
1. 希尔排序的思想：使数组中任意间隔为h的数组元素都是有序的，这些数组被称为h有序数组；
2. 先将数组分割为N个h有序数,一般h有序数初始长度为数组总长度的1/3，
3. N不断地减小即h数组的长度不断增大，最后h数组的长度等于数组的总长度，N=1；
3. h数组用插入法实现排序
*/
void shellSort(int* array, int length)
{
	if (array == NULL || length < 2)
	{
		return;
	}
	//求h数组的最大长度
	int h = length;
	while (h > 1)
	{
		h = h / 3 + 1;
		for (int i = h; i < length; i++)
		{
			int insertIndex = i;
			int tmp = array[i];
			//将当前数据插入到a[i-h],a[i-2h],a[i-3h]。。。中
			for (int j = i - h; j >= 0 && tmp < array[j]; j -= h)
			{
				array[j + h] = array[j];
				insertIndex = j;
			}
			array[insertIndex] = tmp;
		}
	}
}


/*
 借用辅助数组对要排序的数组进行排序；

*/
void mergeSort_Merger(int* array, int startIndex, int midIndex, int endIndex, int* auxArray)
{
	//复制数组元素到辅助数组,并判断数组是否有序
	bool flag = true;
	for (int i = startIndex; i <= endIndex; i++)
	{
		auxArray[i] = array[i];
		if (flag && (i + 1) <= endIndex && array[i] > array[i + 1])
		{
			flag = false;
		}
	}
	//如果数组有序则直接返回
	if (flag) return;

	//排序左右两侧数组并合并
	int leftStart = startIndex;	//左有序序列起点
	int leftEnd = midIndex;	//左有序序列终点
	int rightStart = midIndex + 1;	//右有序序列起点
	int rightEnd = endIndex;	//右有序序列终点
	for (int i = leftStart; i <= rightEnd; i++)
	{
		//左半边用尽，取辅助数组右半边元素
		if (leftStart > leftEnd)
		{
			array[i] = auxArray[rightStart];
			rightStart++;
		}
		//右半边用尽，取辅助数组左半边元素
		else if (rightStart > rightEnd)
		{
			array[i] = auxArray[leftStart];
			leftStart++;
		}
		//当辅助数组右半边的元素大于或等于左半边元素时，取辅助数组的左半边元素；
		else if (auxArray[rightStart] >= auxArray[leftStart])
		{
			array[i] = auxArray[leftStart];
			leftStart++;
		}
		//当辅助数组右半边的元素小于左半边元素时，取辅助数组的右半边元素；
		else if (auxArray[rightStart] < auxArray[leftStart])
		{
			array[i] = auxArray[rightStart];
			rightStart++;
		}

	}

}
void mergeSort_Sort(int* array, int startIndex, int endIndex, int* auxArray)
{
	if (startIndex >= endIndex) return;

	int midIndex = startIndex + (endIndex - startIndex) / 2;

	//数组的左半边排序
	if (startIndex < midIndex)
		mergeSort_Sort(array, startIndex, midIndex, auxArray);

	//数组右半边排序
	if ((midIndex + 1) < endIndex)
		mergeSort_Sort(array, midIndex + 1, endIndex, auxArray);

	//归并左右半边数组
	mergeSort_Merger(array, startIndex, midIndex, endIndex, auxArray);
}

/*
 1. 归并排序思想：将待排序的数组分割成左右两个子数组分别排序，并将有序的子数组归并实现整个数组排序；
 2. 对左半部分数组继续分割成左右两个数组，直到数组长度等于2为止，然后对分割后长度为2的数组排序；
 3. 将长度为2，4，8，16...的左右两个数组进行归并排序；
 3. 右半部分数组处理方式相同；
 4. 最后对左右两个数组归并排序。
 5. 递归调用发生在处理整个数组之前。
*/
void mergeSort(int* array, int length)
{
	if (array == NULL || length < 2)
	{
		return;
	}

	//为辅助数组申请内存空间；
	int* auxArray = (int*)malloc(sizeof(int) * length);

	//开始排序
	mergeSort_Sort(array, 0, length - 1, auxArray);

	//排序结束，释放内存空间
	if (auxArray != NULL)
	{
		free(auxArray);
	}
}

/*
切分的目的：保证切分元素之前的子数组元素（不一定有序）都小于切分元素；
切分元素之后的子数组元素（不一定有序）都大于切分元素；
*/
int quickSort_Partition(int* array, int startIndex, int endIndex)
{
	int partitionIndex = startIndex;
	int leftPointerIndex = partitionIndex + 1;
	int rightPointerIndex = endIndex;
	while (true)
	{
		//从数组左端扫描直到一个大于切分元素的元素
		while (leftPointerIndex < endIndex)
		{
			if (array[leftPointerIndex] >= array[partitionIndex]) break;
			leftPointerIndex++;
		}
		//切分元素是数组中的最大值，此时直接将切分元素与数组最后一个元素交换，即可保证切分元素之前的子数组元素（不一定有序）都小于切分元素；
		if (leftPointerIndex == endIndex) break;

		//从数组右侧扫描直到一个小于切分元素的元素
		while (rightPointerIndex > startIndex)
		{
			if (array[rightPointerIndex] < array[partitionIndex]) break;
			rightPointerIndex--;
		}
		//切分元素是数组中的最小值，此时直接返回切分元素索引，可保证切分元素之后的子数组元素（不一定有序）都大于切分元素；
		if (rightPointerIndex == startIndex) return rightPointerIndex;

		//但左指针索引小于右指针索引时才交换，否则跳出外层循环
		if (leftPointerIndex >= rightPointerIndex) break;

		//交换左右指针数据
		//int temp = array[leftPointerIndex];
		//array[leftPointerIndex] = array[rightPointerIndex];
		//array[rightPointerIndex] = temp;
		swapArray(array, leftPointerIndex, rightPointerIndex);
	}
	//交换切分元素与右指针数据(右指针指向的元素必定小于切分元素)
	if (array[partitionIndex] > array[rightPointerIndex])
	{
		//int partitionData = array[partitionIndex];
		//array[partitionIndex] = array[rightPointerIndex];
		//array[rightPointerIndex] = partitionData;
		swapArray(array, partitionIndex, rightPointerIndex);
	}
	return rightPointerIndex;
}

quickSort_Sort(int* array, int startIndex, int endIndex)
{
	if (endIndex <= startIndex)
	{
		return;
	}
	int partitionIndex = quickSort_Partition(array, startIndex, endIndex);
	quickSort_Sort(array, startIndex, partitionIndex - 1);
	quickSort_Sort(array, partitionIndex + 1, endIndex);
}

/*
1. 快速排序：分治的排序方法将数组分成两个子数组，将两部分独立地排序，但两个子数组都有序时整个数组自然就有序了。
2. 从数组左侧扫描直到一个大于切分元素a[p]的元素，记为指针a[lp],
3. 从数组右侧扫描直到一个小于切分元素a[p]的元素，记为指针a[rp],
4. 交换a[lp]与a[rp]，当a[lp]与a[rp]相遇时，指针之前的元素都小于切分元素，之后的元素都大于切分元素；
5. 将切分元素a[p]与指针元a[rp]素交换,即可保证a[0，p-1]<=a[p]<=a[p+1,end]；
6. 对a[0,p-1]通过切分排序；对a[p+1,end]通过切分排序；
7. 以上过程始终选择子数组的首元素作为切分元素a[p];
*/
void quickSort(int* array, int length)
{
	if (array == NULL || length < 2)
	{
		return;
	}

	quickSort_Sort(array, 0, length - 1);
}

/*
三向切分快速排序：适用于有大量重复的元素，否则效率没有快速排序高
1. 维护两个指针，leftIndex, rightIndex将数组分割成三个组数组；
1. a[0，leftIndex-1]的元素小于切分元素（不一定有序）；
2. a[leftIndex,rightIndex]的元素等于切分元素；
3. a[rightIndex+1,length]的元素大于切分元素（不一定有序）；
*/
void quickSort_3Way(int* array, int length)
{
	if (array == NULL || length < 2)
	{
		return;
	}
	int leftIndex = 0, rightIndex = length-1;
	int partitionData = array[0]; //首元素作为切分元素
	int i = 1;
	while (i<=rightIndex)
	{
		int currentData = array[i];
		int cmpResult = array[i] - partitionData;
		//小于切分元素，将该元素与切分元素交换，此时切分元素索引为leftIndex++;
		if (cmpResult < 0) swapArray(array, leftIndex++, i++);
		//大于切分元素，将该元素放在大于切分元素的子数组中
		else if (cmpResult > 0) swapArray(array, i, rightIndex--);
		//等于切分元素，
		else i++;
	}
	//至此，a[0，leftIndex-1] < a[leftIndex,rightIndex] < a[rightIndex+1,length]成立；

	//然后只需对小于切分元素的子数组以及大于切分元素的子数组排序即可
	quickSort_Sort(array, 0, leftIndex - 1);
	quickSort_Sort(array, rightIndex+1,length-1);
}



