CC := gcc
LD := ld

ROOT_DIR   := ../..
SOURCE_DIR := .
BUILD_DIR  := $(ROOT_DIR)/bin
IMAGE_DIR  := $(ROOT_DIR)/image_root

CFLAGS     = -Wall -Wextra \
			 -march=x86-64 -m64 -fshort-wchar -fPIE -ffreestanding \
			 -fno-stack-protector -fno-stack-check -fno-strict-aliasing -fno-lto \
			 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone \
			 -I$(ROOT_DIR)/limine-efi/inc

LDFLAGS    = -nostdlib -pie -m elf_x86_64 \
			 -z text -z max-page-size=0x1000 \
			 -T $(ROOT_DIR)/limine-efi/gnuefi/elf_x86_64_efi.lds

EFI_SOURCES := $(wildcard $(SOURCE_DIR)/*.c)
EFI_OBJECTS := $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/boot/%.o, $(EFI_SOURCES))

all_efi: $(BUILD_DIR)/EFI/BOOT/BOOTX64.EFI

$(EFI_OBJECTS): $(EFI_SOURCES)
	@echo "    Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/EFI/BOOT/BOOTX64.EFI: $(EFI_OBJECTS)
	@echo "    Compiling crt0-efi-x86_64.S..."
	@$(CC) $(CFLAGS) $(EFI_CFLAGS) -std=gnu11 $(ROOT_DIR)/limine-efi/gnuefi/crt0-efi-x86_64.S -c -o $(BUILD_DIR)/boot/crt0-efi-x86_64.o
	@echo "    Compiling reloc_x86_64.c..."
	@$(CC) $(CFLAGS) $(EFI_CFLAGS) -std=gnu11 $(ROOT_DIR)/limine-efi/gnuefi/reloc_x86_64.c -c -o $(BUILD_DIR)/boot/reloc_x86_64.o
	@echo "    Linking boot.elf..."
	@$(LD) $(LDFLAGS) $(BUILD_DIR)/boot/crt0-efi-x86_64.o $(BUILD_DIR)/boot/reloc_x86_64.o $(EFI_OBJECTS) -o $(BUILD_DIR)/boot/boot.elf
	@echo "    Creating EFI/BOOT/BOOTX64.efi..."
	@objcopy -O binary $(BUILD_DIR)/boot/boot.elf $(BUILD_DIR)/EFI/BOOT/BOOTX64.EFI
	@dd if=/dev/zero of=$(BUILD_DIR)/EFI/BOOT/BOOTX64.EFI bs=4096 count=0 seek=$$(( ($$(wc -c < $(BUILD_DIR)/EFI/BOOT/BOOTX64.EFI) + 4095) / 4096 )) 2>/dev/null