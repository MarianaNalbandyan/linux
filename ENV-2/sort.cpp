#include "sort.h"
using namespace std;

void bubbleSort(vector<Complex>& arr)
{
    size_t n = arr.size();
    bool swapped;
    for (size_t i = 0; i < n-1; ++i)
    {
       swapped = false;
       for (size_t i = 0; i < n - 1; ++i)
       {
           swapped = false;
           for (size_t j = 0; j < n - i - 1; ++j)
           {
              if(arr[j].abs() > arr[j + 1].abs())
              {
                  swap(arr[j], arr[j + 1]);
                  swapped = true;
              }
           }
           if (!swapped)
           {
               break;
           }
        }
     }
}

