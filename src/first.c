#include <math.h>

#include "contract.h"

float E(int x) {
    if (x <= 0) {
        return NAN;
    }
    return (float)pow(1.0 + 1.0 / x, x);
}

int *Sort(int *array) {
    int size = array[0];
    int *values = array + 1;

    for (int end = size - 1; end > 0; --end) {
        int swapped = 0;
        for (int i = 0; i < end; ++i) {
            if (values[i] > values[i + 1]) {
                int temp = values[i];
                values[i] = values[i + 1];
                values[i + 1] = temp;
                swapped = 1;
            }
        }
        if (swapped == 0) {
            break;
        }
    }
    return array;
}
