dd if=/dev/zero of=disk.img bs=1M count=256 >/dev/null 2>/dev/null

# Create the disk image
sudo parted disk.img --script mklabel gpt >/dev/null
sudo parted disk.img --script mkpart primary fat32 1mb 65mb
sudo parted disk.img --script mkpart secondary fat32 65mb 256mb
sudo parted disk.img --script set 1 boot on
sudo parted disk.img --script set 2 msftdata on
sudo parted disk.img --script name 1 "BOOT"
sudo parted disk.img --script name 2 "ROOTFS"

LOOP_DEVICE=$(sudo losetup -fP --show disk.img)

# Partition the disk image as Fat32
sudo mkfs.fat -F32 ${LOOP_DEVICE}p1 >/dev/null 2>/dev/null
sudo mkfs.fat -F32 ${LOOP_DEVICE}p2 >/dev/null 2>/dev/null

# Copy the boot files to the boot partition
sudo mkdir -p /mnt/${LOOP_DEVICE}p1
sudo mount ${LOOP_DEVICE}p1 /mnt/${LOOP_DEVICE}p1
sudo rsync -av --exclude='.gitkeep' --no-owner --no-group image_root/boot/ /mnt/${LOOP_DEVICE}p1/ >/dev/null 2>/dev/null
sudo umount /mnt/${LOOP_DEVICE}p1

# Copy the rootfs files to the rootfs partition
sudo mkdir -p /mnt/${LOOP_DEVICE}p2
sudo mount ${LOOP_DEVICE}p2 /mnt/${LOOP_DEVICE}p2
sudo rsync -av --exclude='.gitkeep' --no-owner --no-group image_root/rootfs/ /mnt/${LOOP_DEVICE}p2/ >/dev/null 2>/dev/null
sudo umount /mnt/${LOOP_DEVICE}p2


sudo losetup -d ${LOOP_DEVICE}

sudo rm -rf /mnt/${LOOP_DEVICE}p1
sudo rm -rf /mnt/${LOOP_DEVICE}p2

sudo parted disk.img --script print