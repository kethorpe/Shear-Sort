#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "quickSort.h"

using namespace std;
//----------------------------------------------------------------
//// Recursive Quicksort algorithm using basic partition function.
//----------------------------------------------------------------
void quickSort::quickSortForward(int arr[], int low, int high)
{
   int left = low, right = high;
   int tmp;
   int pivot = arr[(left + right) / 2];

   /* partition */
   while (left <= right) {
     while(arr[left] < pivot){
         left++;
     }
     while (arr[right] > pivot){
         right--;
     }
     if (left <= right) {
         tmp = arr[left];
         arr[left] = arr[right];
         arr[right] = tmp;
         left++;
         right--;
     }
  };

   /* recursion */
   if (low < right)
      quickSortForward(arr, low, right);
   if (left < high)
      quickSortForward(arr, left, high);
}

void quickSort::quickSortBackward(int arr[], int low, int high)
{
   int left = low, right = high;
   int tmp;
   int pivot = arr[(left + right) / 2];

   /* partition */
   while (left <= right) {
     while(arr[left] > pivot){
         left++;
     }
     while (arr[right] < pivot){
         right--;
     }
     if (left <= right) {
         tmp = arr[left];
         arr[left] = arr[right];
         arr[right] = tmp;
         left++;
         right--;
     }
   };

   /* recursion */
   if (low < right)
      quickSortBackward(arr, low, right);
   if (left < high)
      quickSortBackward(arr, left, high);
}
