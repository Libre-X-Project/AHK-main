# Notes
Written by Kap Petrov

## FAT12
The FAT file system views the storage media as a flat array of clusters. If the physical media does not address its data as a flat list of sectors (really old hard disks and floppy disks) then the cluster numbers will need to be translated before being sent to the disk. The storage media is organized into three basic areas.

-    The boot record
-    The File Allocation Table (FAT)
-    The directory and data area

## Boot record
The boot record occupies one sector, and is always placed in logical sector number zero of the "partition". If the media is not divided into partitions, then this is the beginning of the media. This is the easiest sector on the partition for the computer to locate when it is loaded. If the storage media is partitioned (such as a hard disk), then the beginning of the actual media contains an MBR (x86) or other form of partition information. In this case each partition's first sector holds a Volume Boot Record.

### Bios Parameter Block
The boot record contains both code and data, mixed together. The data that isn't code is known as the BPB. 

## BPB Structure

### JMP SHORT 3C NOP
The first three bytes EB 3C 90 disassemble to JMP SHORT 3C NOP. (The 3C value may be different.) The reason for this is to jump over the disk format information (the BPB and EBPB). Since the first sector of the disk is loaded into ram at location 0x0000:0x7c00 and executed, without this jump, the processor would attempt to execute data that isn't code. Even for non-bootable volumes, code matching this pattern (or using the E9 jump opcode) is required to be present by both Windows and OS X. To fulfil this requirement, an infinite loop can be placed here with the bytes EB FE 90. 

### OEM Identifier
OEM identifier. The first 8 Bytes (3 - 10) is the version of DOS being used. The next eight Bytes 29 3A 63 7E 2D 49 48 and 43 read out the name of the version. The official FAT Specification from Microsoft says that this field is really meaningless and is ignored by MS FAT Drivers, however it does recommend the value "MSWIN4.1" as some 3rd party drivers supposedly check it and expect it to have that value. Older versions of dos also report MSDOS5.1, linux-formatted floppy will likely to carry "mkdosfs" here, and FreeDOS formatted disks have been observed to have "FRDOS5.1" here. If the string is less than 8 bytes, it is padded with spaces. 

### Bps
The number of Bytes per sector (remember, all numbers are in the little-endian format). 

### Spc
Number of sectors per cluster. 

### Rs
Number of reserved sectors. The boot record sectors are included in this value.

### Fats
Number of File Allocation Tables (FAT's) on the storage media. Often this value is 2. 

### Rde
Number of root directory entries (must be set so that the root directory occupies entire sectors). 

### Ts
The total sectors in the logical volume. If this value is 0, it means there are more than 65535 sectors in the volume, and the actual count is stored in the Large Sector Count entry at 0x20. Damn boy you have a large disk.

### 0x15
This Byte indicates the media descriptor type. [https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system#BPB20_OFS_0Ah]

### 0x16
Number of sectors per FAT. FAT12/FAT16 only. 

### 0x18
Number of sectors per track.

### 0x1A
Number of heads or sides on the storage media.

### 0x1C
Number of hidden sectors. (i.e. the LBA of the beginning of the partition.) 
### 0x20
Large sector count. This field is set if there are more than 65535 sectors in the volume, resulting in a value which does not fit in the Number of Sectors entry at 0x13. 

## Extended Boot Record (EBR) (FAT12/FAT16 only)

### 0x024
Drive number. The value here should be identical to the value returned by BIOS interrupt 0x13, or passed in the DL register; i.e. 0x00 for a floppy disk and 0x80 for hard disks. This number is useless because the media is likely to be moved to another machine and inserted in a drive with a different drive number. 

### 0x025
Flags in Windows NT. Reserved otherwise. (SO ALWAYS RESERVED MOTHERFU-)

### 0x026
Signature (must be 0x28 or 0x29). 

### 0x027
VolumeID 'Serial' number. Used for tracking volumes between computers. You can ignore this if you want. 

### 0x02B	
Volume label string. This field is padded with spaces.

### 0x036	
System identifier string. This field is a string representation of the FAT file system type. It is padded with spaces. The spec says never to trust the contents of this string for any use.

### 0x03E
Boot code.

### 0x1FE 
Bootable partition signature 0xAA55.

## File Allocation Table

### FAT12
FAT 12 uses 12 bits to address the clusters on the disk. Each 12 bit entry in the FAT points to the next cluster of a file on the disk. Given a valid cluster number, here is how you extract the value of the next cluster in the cluster chain: 

```
unsigned char FAT_table[sector_size * 2]; // needs two in case we straddle a sector
unsigned int fat_offset = active_cluster + (active_cluster / 2);// multiply by 1.5
unsigned int fat_sector = first_fat_sector + (fat_offset / sector_size);
unsigned int ent_offset = fat_offset % sector_size;

//at this point you need to read two sectors from disk starting at "fat_sector" into "FAT_table".

unsigned short table_value = *(unsigned short*)&FAT_table[ent_offset];

table_value = (active_cluster & 1) ? table_value >> 4 : table_value & 0xfff;

//the variable "table_value" now has the information you need about the next cluster in the chain
```

## Directories on FAT12/16/32 (Directory_Entry)

### Offset 0
8.3 file name. The first 8 characters are the name and the last 3 are the extension. 

### Offset 11
Attributes of the file. The possible attributes are:
```
READ_ONLY=0x01 
HIDDEN=0x02 SYSTEM=0x04 
VOLUME_ID=0x08 
DIRECTORY=0x10
ARCHIVE=0x20 
LFN=READ_ONLY|HIDDEN|SYSTEM|VOLUME_ID 
```

(LFN means that this entry is a long file name entry) 

### Offset 12
Reserved for use by Windows NT. (NOBODY CARES MOTHERFU-)

### Offset 13
Creation time in hundredths of a second, although the official FAT Specification from Microsoft says it is tenths of a second. Range 0-199 inclusive. Based on simple tests, Ubuntu16.10 stores either 0 or 100 while Windows7 stores 0-199 in this field. 

### Offset 14
The time that the file was created. Multiply Seconds by 2.
```
Hour    	5 bits
Minutes 	6 bits
Seconds 	5 bits 
```

### Offset 16
The date on which the file was created.

```
Year 	7 bits
Month 	4 bits
Day 	5 bits 
```

## 18 
Last accessed date. Same format as the creation date.

## 20
The high 16 bits of this entry's first cluster number. For FAT 12 and FAT 16 this is always zero.

# 22 
Last modification time. Same format as the creation time.

## 24 	
Last modification date. Same format as the creation date.

## 26
The low 16 bits of this entry's first cluster number. Use this number to find the first cluster for this entry.

## 28 
The size of the file in bytes. 

## Credits
- OSDev Wiki [https://wiki.osdev.org/FAT]
- Nanobyte [https://github.com/nanobyte-dev/nanobyte_os]