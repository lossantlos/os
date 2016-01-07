#!/bin/sh

pushd `dirname $0` > /dev/null
SCRIPTPATH=`pwd -P`
. ./settings.cfg
popd > /dev/null

if [[ $UID -ne 0 ]]; then
    echo "Permission dennied!"
    exit 1
fi


filename="./disk.raw"

if [[ -e "$filename" ]]; then
    echo "$filename exist" >&2
    exit 1
fi

set -e

dd if=/dev/zero of=$filename bs=1MiB count=32
echo -e "o\nn\np\n1\n\n\na\nw\n" | /sbin/fdisk $filename
a=$(kpartx -a -v $filename)
partition=$(echo $a | cut -d " " -f 3)
disk=$(echo $a | cut -d " " -f 8)
sleep 1
mkfs.ext2 /dev/mapper/$partition
mount /dev/mapper/$partition ./mnt
sudo ./tools/sbin/grub-install --no-floppy --modules="part_msdos ext2 multiboot configfile" --boot-directory=./mnt/boot/ $disk
cp ./filesystem/boot/grub/grub.cfg ./mnt/boot/grub/
cp ./kernel.bin ./mnt/
./scripts/disk-image-umount.sh ./mnt
