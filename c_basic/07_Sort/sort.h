#ifndef SORT_H_
#define SORT_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//ð������(��������)
void bubbleSort(int* array, int length);

//ѡ������(��������)
void selectionSort(int* array, int length);

//���������㷨(��������)
void insertionSort(int* array, int length);

//ϣ������(��������)
void shellSort(int* array, int length);

//�鲢����(��������)
void mergeSort(int* array, int length);

//��������(��������)
void quickSort(int* array, int length);

//��������(��������)
void quickSort_3Way(int* array, int length);

#endif //SORT_H_