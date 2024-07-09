#ifndef BOOTLOADER_FORMAT_H
#define BOOTLOADER_FORMAT_H

#include <efibind.h>

void UintToStr(UINTN integer, CHAR16* string);
void IntToStr(INTN integer, CHAR16* string);

#endif