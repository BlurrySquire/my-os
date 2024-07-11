#include <efi.h>

#include "format.h"

EFI_SYSTEM_TABLE*       ST;
EFI_BOOT_SERVICES*      BS;
EFI_RUNTIME_SERVICES*   RS;

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
    ST = SystemTable;
    BS = SystemTable->BootServices;
    RS = SystemTable->RuntimeServices;

    /* Init console */
    ST->ConOut->ClearScreen(ST->ConOut);
    ST->ConOut->EnableCursor(ST->ConOut, TRUE);
    ST->ConOut->OutputString(ST->ConOut, L"Hello, UEFI!\r\n");

    CHAR16* FirmwareVersion = CHAR_NULL;
    UintToStr(ST->FirmwareRevision, FirmwareVersion);
    ST->ConOut->OutputString(ST->ConOut, ST->FirmwareVendor);
    ST->ConOut->OutputString(ST->ConOut, L" ");
    ST->ConOut->OutputString(ST->ConOut, FirmwareVersion);
    ST->ConOut->OutputString(ST->ConOut, L"\r\n");

    /* Get the memory map */
    EFI_MEMORY_DESCRIPTOR*      MemoryMap;
    UINTN                       MemoryMapSize;
    UINTN                       MapKey;
    UINTN                       DescriptorSize;
    UINT32                      DescriptorVersion;

    MemoryMapSize = 0;
    MemoryMap = NULL;

    EFI_STATUS status;

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