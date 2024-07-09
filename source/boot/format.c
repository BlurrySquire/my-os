#include "format.h"

void UintToStr(UINTN integer, CHAR16* string) {
    UINTN index = 0;

    do {
        string[index++] = integer % 10 + L'0';
    } while ((integer /= 10) > 0);
    string[index] = L'\0';                                          

    UINTN start = 0;
    UINTN end = index - 1;

    while (start < end) {
        CHAR16 temp = string[start];
        string[start] = string[end];
        string[end] = temp;
        
        start++;
        end--;
    }
}

void IntToStr(INTN integer, CHAR16* string) {
    UINTN index = 0;
    UINT8 negative = 0;

    if (integer < 0) {
        negative = 1;
        integer *= -1;
    }

    do {
        string[index++] = integer % 10 + L'0';
    } while ((integer /= 10) > 0);

    if (negative > 0) {
        string[index++] = '-';
    }

    string[index] = L'\0';

    UINTN start = 0;
    UINTN end = index - 1;

    while (start < end) {
        CHAR16 temp = string[start];
        string[start] = string[end];
        string[end] = temp;
        
        start++;
        end--;
    }
}