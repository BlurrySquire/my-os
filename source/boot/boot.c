#include <efi.h>

void UintToStr(UINTN integer, CHAR16* string);
void IntToStr(INTN integer, CHAR16* string);

void GetMemoryMap(EFI_SYSTEM_TABLE* SystemTable, EFI_MEMORY_DESCRIPTOR* memoryMap);

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    /* Init console */
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->EnableCursor(SystemTable->ConOut, TRUE);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello, UEFI!\r\n");

    CHAR16 test[16];
    IntToStr(-123456, &test);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, test);

    EFI_MEMORY_DESCRIPTOR* memoryMap;
    GetMemoryMap(SystemTable, memoryMap);

    CHAR16 string[64];
    UintToStr(memoryMap->NumberOfPages, &string);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, string);

    /* Exit boot services before booting kernel */
    SystemTable->BootServices->ExitBootServices(ImageHandle, 0);

    while (TRUE) {
        /* Hang the system. */
    }

    /*
        Shouldn't reach here, but
        if we do then just shutdown
        the system.
    */

    SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, L"\0");
}

void GetMemoryMap(EFI_SYSTEM_TABLE* SystemTable, EFI_MEMORY_DESCRIPTOR* memoryMap) {
    UINTN memoryMapSize = 0;
    UINTN mapKey;
    UINTN descriptorSize;
    UINT32 descriptorVersion;

    EFI_STATUS status;

    status = SystemTable->BootServices->GetMemoryMap(&memoryMapSize, &memoryMap, &mapKey, &descriptorSize, &descriptorVersion);
    if (EFI_ERROR(status)) {
        status &= 0x7FFFFFFFFFFFFFFF;
        CHAR16 error_code[4];
        UintToStr(status, &error_code);
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"[ERROR]: BootServices->GetMemoryMap: ");
        SystemTable->ConOut->OutputString(SystemTable->ConOut, error_code);
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");
    }

    status = SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(EFI_MEMORY_DESCRIPTOR), &memoryMap);
    if (EFI_ERROR(status)) {
        status &= 0x7FFFFFFFFFFFFFFF;
        CHAR16 error_code[4];
        UintToStr(status, &error_code);
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"[ERROR]: BootServices->AllocatePool: ");
        SystemTable->ConOut->OutputString(SystemTable->ConOut, error_code);
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");
    }
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Allocated space for the memory map.\r\n");

    status = SystemTable->BootServices->GetMemoryMap(&memoryMapSize, &memoryMap, &mapKey, &descriptorSize, &descriptorVersion);
    if (EFI_ERROR(status)) {
        status &= 0x7FFFFFFFFFFFFFFF;
        CHAR16 error_code[4];
        UintToStr(status, &error_code);
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"[ERROR]: BootServices->GetMemoryMap: ");
        SystemTable->ConOut->OutputString(SystemTable->ConOut, error_code);
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");
    }
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Got the memory map.\r\n");
}


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

/*
void IntToStr(INTN integer, CHAR16* string) {
    UINTN index = 0;

    do {
        string[index++] = integer % 10 + L'0';
    } while ((integer /= 10) > 0);
    string[index] = L'\0';                                          

    UINTN start = 0;
    UINTN end = index - 1;

    if (integer < 0) {
        string[index] = '-';
        index++;
    }

    while (start < end) {
        CHAR16 temp = string[start];
        string[start] = string[end];
        string[end] = temp;
        
        start++;
        end--;
    }
}
*/