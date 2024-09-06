echo Making FAT12 image...
dd if=/dev/zero of=main_floppy.img bs=512 count=2880
mkfs.fat -F 12 -n "AHOS" main_floppy.img
mcopy -i main_floppy.img test.txt "::test.txt"

echo Compiling FAT Driver...
gcc fat.c -o fat

echo Done!