#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/snakeos.kernel isodir/boot/snakeos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "snakeos" {
	multiboot /boot/snakeos.kernel
}
EOF
grub2-mkrescue -o snakeos.iso isodir
