#include <efi.h>

#include "format.h"

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
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->EnableCursor(SystemTable->ConOut, TRUE);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello, UEFI!\r\n");

    CHAR16* FirmwareVersion = CHAR_NULL;
    UintToStr(SystemTable->FirmwareRevision, FirmwareVersion);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, SystemTable->FirmwareVendor);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L" ");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, FirmwareVersion);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");

    // Get the memory map.
    EFI_MEMORY_DESCRIPTOR*      MemoryMap = NULL;
    UINTN                       MemoryMapSize = 0;
    UINTN                       MapKey;
    UINTN                       DescriptorSize;
    UINT32                      DescriptorVersion;

    EFI_STATUS status;

    status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, NULL, &DescriptorSize, NULL);
    if (EFI_ERROR(status)) {
        LOG_ERROR(SystemTable, status, L"BootServices->GetMemoryMap");
    }

    MemoryMapSize += DescriptorSize * 4;

    status = SystemTable->BootServices->AllocatePool(EfiLoaderData, MemoryMapSize, (void **)&MemoryMap);
    if (EFI_ERROR(status)) {
        LOG_ERROR(SystemTable, status, L"BootServices->AllocatePool");
    }

    status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    if (EFI_ERROR(status)) {
        LOG_ERROR(SystemTable, status, L"BootServices->GetMemoryMap");
    }

    UINTN NumberOfPages = 0;
    EFI_MEMORY_DESCRIPTOR* MemoryMapEntry = MemoryMap;
    do {
        if (MemoryMapEntry->Type != EfiReservedMemoryType) {
            NumberOfPages += MemoryMapEntry->NumberOfPages;
        }

        MemoryMapEntry = NextMemoryDescriptor(MemoryMapEntry, DescriptorSize);
    } while ((UINT8*)MemoryMapEntry < (UINT8*)MemoryMap + MemoryMapSize);

    CHAR16* NoOfPages = CHAR_NULL;
    UintToStr(NumberOfPages, NoOfPages);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, NoOfPages);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");

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