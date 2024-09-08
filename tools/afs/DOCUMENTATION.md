# Documentation
Written by Kap Petrov

## AFS
The AHK file system views the storage media as a flat array of sectors.

It consists of:

-    The AFS Header
-    The File Headers
-    The Data Area

## Header
The Header occupies one sector, and is always placed in logical sector number zero of the "partition". If the media is not divided into partitions, then this is the beginning of the media. This is the easiest sector on the partition for the computer to locate when it is loaded. If the storage media is partitioned (such as a hard disk), then the beginning of the actual media contains an MBR (x86) or other form of partition information. In this case each partition's first sector holds a AFS Header.

### 0x00
The header only contains the number of files in the whole disk. This occupies 4 bytes.

## File Headers
The AFS Header is followed by a set number (64) of file header structures:

### Magic bytes
The first element of the header is the magic bytes for consistency, it is a unsigned char or a uint8_t.

### Name
File name. The first 64 Bytes is just the filename.

### Offset
The offset of the hard disk that the file starts. This is an unsigned int or uint32_t.

### Length
The length of the file, this is also an unsigned int or a uint32_t.

## Data Area
This is simply the data of each file. Implementing an AFS Driver in C, you can use fseek to point at the end of the file (SEEK_END)

## Credits
- The members of the AHK Team