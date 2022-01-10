#ifndef SORT_H_
#define SORT_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//Ã°ÅİÅÅĞò(ÉıĞòÅÅÁĞ)
void bubbleSort(int* array, int length);

//Ñ¡ÔñÅÅĞò(ÉıĞòÅÅÁĞ)
void selectionSort(int* array, int length);

//²åÈëÅÅĞòËã·¨(ÉıĞòÅÅÁĞ)
void insertionSort(int* array, int length);

//Ï£¶ûÅÅĞò(ÉıĞòÅÅÁĞ)
void shellSort(int* array, int length);

//¹é²¢ÅÅĞò(ÉıĞòÅÅÁĞ)
void mergeSort(int* array, int length);

//¿ìËÙÅÅĞò(ÉıĞòÅÅÁĞ)
void quickSort(int* array, int length);

//¿ìËÙÅÅĞò(ÉıĞòÅÅÁĞ)
void quickSort_3Way(int* array, int length);

#endif //SORT_H_