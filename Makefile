CC := x86_64-elf-gcc
LD := x86_64-elf-ld

SOURCE_DIR = source
BUILD_DIR  = bin
IMAGE_DIR  = image_root

all: build_efi build_disk

build_efi:
	@echo "Compiling EFI Bootloader..."
	@mkdir -p $(BUILD_DIR)/boot $(BUILD_DIR)/EFI/BOOT
	@$(MAKE) -C $(SOURCE_DIR)/boot all_efi

build_disk:
	@echo "Creating Disk Image..."
	@cp -r $(BUILD_DIR)/EFI/BOOT/BOOTX64.EFI $(IMAGE_DIR)/boot/EFI/BOOT
	@./make_disk.sh

run:
	qemu-system-x86_64 --bios OVMF.fd -net none -m 128 -M q35 --enable-kvm -drive format=raw,file=fat:rw:image_root/boot

clean:
	rm -rf bin/*

init:
	mkdir -p bin
	curl -o OVMF.fd https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF.fd
	git clone https://github.com/limine-bootloader/limine-efi