I_Creator:
./I_Creator {Filename HOST} {Output Filename IMAGE}

The image is initrd.img. (obviously.)
I_Creator is a tool that can be used to Transfer Host files to the Filesystem directly (Injecting it directly into the OS while the system is live can be dangerous for multiple reasons),
AHK Will load Initrd.img, which is the filesystem.

viewinitrd:
./viewinitrd
Self explanatory, lets you get an overview of files in initrd, for an executable, it must have a 32 bit elf structure (file filename.elf)

It is recommended to recompile them from source.

gcc -o viewinitrd viewinitrd.c
gcc -o I_Creator Initrdcreator.c

due to LIBC and GCC Versions often mismatching (when you open, say I_Creator, you can see it specifies elf32 and gcc)
