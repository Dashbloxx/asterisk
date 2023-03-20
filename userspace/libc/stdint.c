#include "stdint.h"

size_t digit_size(int num)
{
    size_t size = 0;
    if(num == 0)
        return 1;
        while(num > 0) {
            size++;
        num = num / 10;
    }
    return size;
}