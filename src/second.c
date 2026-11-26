#include <math.h>

#include "contract.h"

float E(int x) {
    if (x <= 0) {
        return NAN;
    }

    double sum = 1.0;
    double term = 1.0;
    for (int n = 0; n < x; ++n) {
        term /= n + 1;
        double next = sum + term;
        if (next == sum) {
            break;
        }
        sum = next;
    }
    return (float)sum;
}

static void quick_sort(int *array, int left, int right) {
    while (left < right) {
        int pivot = array[left + (right - left) / 2];
        int i = left - 1;
        int j = right + 1;

        while (1) {
            do {
                ++i;
            } while (array[i] < pivot);
            do {
                --j;
            } while (array[j] > pivot);

            if (i >= j) {
                break;
            }
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }

        /* Recurse into the smaller part to keep the stack small. */
        if (j - left < right - j) {
            quick_sort(array, left, j);
            left = j + 1;
        } else {
            quick_sort(array, j + 1, right);
            right = j;
        }
    }
}

int *Sort(int *array) {
    quick_sort(array + 1, 0, array[0] - 1);
    return array;
}
