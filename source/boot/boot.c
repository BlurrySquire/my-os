#include <efi.h>

void UintToStr(UINTN integer, CHAR16* string);
void IntToStr(INTN integer, CHAR16* string);

#define LOG_ERROR(SystemTable, status, fun_name) \
    status &= 0x7FFFFFFFFFFFFFFF; \
    CHAR16* error_code = CHAR_NULL; \
    UintToStr(status, error_code); \
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"[ERROR]: "); \
    SystemTable->ConOut->OutputString(SystemTable->ConOut, fun_name); \
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L": "); \
    SystemTable->ConOut->OutputString(SystemTable->ConOut, error_code); \
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    /* Init console */
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->EnableCursor(SystemTable->ConOut, TRUE);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello, UEFI!\r\n");

    CHAR16* FirmwareVersion = CHAR_NULL;
    UintToStr(SystemTable->FirmwareRevision, FirmwareVersion);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, SystemTable->FirmwareVendor);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L" ");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, FirmwareVersion);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");

    /* Get the memory map */
    EFI_MEMORY_DESCRIPTOR*      MemoryMap;
    UINTN                       MemoryMapSize;
    UINTN                       MapKey;
    UINTN                       DescriptorSize;
    UINT32                      DescriptorVersion;

    MemoryMapSize = 0;
    MemoryMap = NULL;

    EFI_STATUS status;

    status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, NULL, &DescriptorSize, NULL);
    if (EFI_ERROR(status)) {
        LOG_ERROR(SystemTable, status, L"BootServices->GetMemoryMap");
    }

    MemoryMapSize += DescriptorSize * 2;

    status = SystemTable->BootServices->AllocatePool(EfiLoaderData, MemoryMapSize, (void **)&MemoryMap);
    if (EFI_ERROR(status)) {
        LOG_ERROR(SystemTable, status, L"BootServices->AllocatePool");
    }

    status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    if (EFI_ERROR(status)) {
        LOG_ERROR(SystemTable, status, L"BootServices->GetMemoryMap");
        SystemTable->BootServices->FreePool(MemoryMap);
    }

    CHAR16* NoOfPages = CHAR_NULL;
    UintToStr(MemoryMap->NumberOfPages, NoOfPages);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, NoOfPages);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");

    /* Exit boot services before booting kernel */
    SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

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