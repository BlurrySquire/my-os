# My-OS
A minimal, UEFI only, operating system. There is a long way to go as I am still working on the bootloader, which is in its early stages.


# Building My-OS
My-OS uses [GNU Make](https://www.gnu.org/software/make/) as its build system. Currently, building is only for linux.

## Dependencies

### On Debian-based distros:
```sh
sudo apt-get install git make curl build-essential
```
You also need [qemu](https://www.qemu.org/) (optionally you can build it yourself):
```sh
sudo apt-get install qemu qemu-kvm qemu-system-x86
```

## Build Commands
- ``make init``: This command initialises the build environment, by downloading the latest nightly [OVMF](https://retrage.github.io/edk2-nightly/bin/) (UEFI firmware for qemu) and cloning the [limine-efi](https://github.com/limine-bootloader/limine-efi) repo.

- ``make clean``: This command clears the build directory. This is useful if you want to rebuild the code without making changes.

- ``make run``: This command launches My-OS through [qemu](https://www.qemu.org/).

- ``make all``: This command builds the bootloader and disk image.

- ``make build_<name-of-part>``: This command builds a specific section of My-OS.

## Build Steps
1. Clone the [repository](https://github.com/BlurrySquire/my-os) using ``git clone https://github.com/BlurrySquire/my-os``.

2. Run the command ``make init`` to initialise the build environment.

3. You can now build using ``make all``.

4. Run My-OS using ``make run``.

5. If you only need to rebuild a part of My-OS you can use ``make build_<name-of-part>``. If you recompile a section, you will likely need to rebuild the disk image for changes to take effect. You can find a list of these options below.

## Build Sections
- ``build_bootloader``: Builds the bootloader.
- ``build_disk``: Builds the disk image.