echo Launching QEMU with FAT12 Formatted Hard Drive...
echo NOTE: Using monitor stdio...
qemu-system-i386 -boot d -cdrom os.iso -drive file=main_floppy.img,format=raw -monitor stdio