#include <efi.h>

#include "format.h"

/*
    A macro that prints the status
    returned by a UEFI function. It
    also prints the function name.
*/
#define LOG_ERROR(status, fun_name) \
    status &= 0x7FFFFFFFFFFFFFFF; \
    CHAR16* error_code = CHAR_NULL; \
    UintToStr(status, error_code); \
    ST->ConOut->OutputString(ST->ConOut, L"[ERROR]: "); \
    ST->ConOut->OutputString(ST->ConOut, fun_name); \
    ST->ConOut->OutputString(ST->ConOut, L": "); \
    ST->ConOut->OutputString(ST->ConOut, error_code); \
    ST->ConOut->OutputString(ST->ConOut, L"\r\n");

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    EFI_SYSTEM_TABLE*       ST = SystemTable;
    EFI_BOOT_SERVICES*      BS = SystemTable->BootServices;
    EFI_RUNTIME_SERVICES*   RS = SystemTable->RuntimeServices;

    /* Intialise the console. */
    ST->ConOut->ClearScreen(ST->ConOut);
    ST->ConOut->EnableCursor(ST->ConOut, TRUE);
    ST->ConOut->OutputString(ST->ConOut, L"Hello, UEFI!\r\n");

    /* Print the firmware vendor and revision to the console. */
    CHAR16* FirmwareVersion = CHAR_NULL;
    UintToStr(ST->FirmwareRevision, FirmwareVersion);
    ST->ConOut->OutputString(ST->ConOut, ST->FirmwareVendor);
    ST->ConOut->OutputString(ST->ConOut, L" V");
    ST->ConOut->OutputString(ST->ConOut, FirmwareVersion);
    ST->ConOut->OutputString(ST->ConOut, L"\r\n");

    /* Get the memory map. */
    EFI_MEMORY_DESCRIPTOR* MemoryMap = NULL;
    UINTN MemoryMapSize = 0;
    UINTN MapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;

    EFI_STATUS status;

    /*
        This call will always fail, we
        need it to get the size of the
        memory map and each descriptor.
    */
    status = BS->GetMemoryMap(&MemoryMapSize, MemoryMap, NULL, &DescriptorSize, NULL);
    if (EFI_ERROR(status)) {
        LOG_ERROR(status, L"BootServices->GetMemoryMap");
    }

    MemoryMapSize += DescriptorSize * 2;
    status = BS->AllocatePool(EfiLoaderData, MemoryMapSize, (void **)&MemoryMap);
    if (EFI_ERROR(status)) {
        LOG_ERROR(status, L"BootServices->AllocatePool");
    }

    status = BS->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    if (EFI_ERROR(status)) {
        LOG_ERROR(status, L"BootServices->GetMemoryMap");
        BS->FreePool(MemoryMap);
    }

    CHAR16* NoOfPages = CHAR_NULL;
    UintToStr(MemoryMap->NumberOfPages, NoOfPages);
    ST->ConOut->OutputString(SystemTable->ConOut, NoOfPages);
    ST->ConOut->OutputString(SystemTable->ConOut, L"\r\n");

    /* Exit boot services before booting kernel */
    BS->ExitBootServices(ImageHandle, MapKey);

    while (TRUE) {
        /* Hang the system. */
    }

    /*
        Shouldn't reach here, but
        if we do then just shutdown
        the system.
    */

    RS->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, L"\0");
}