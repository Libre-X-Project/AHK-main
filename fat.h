/*
	FAT12 Driver Implementation
	Written by AHK Developer(s): <Advanced/Adaptable Hybrid Kernel Team>
	See tools/fat for a in-depth overview
*/

#ifndef FAT_H
#define FAT_H

#include "common.h"
#include "ide.h"

// Bios Parameter Block + Extended Boot Record
typedef struct
{
	uint8_t BootJumpInstruction[3];			// JMP SHORT 3C NOP
	uint8_t OemIdentifier[8];				// OEM Identifier
	uint16_t BytesPerSector;				// Bytes per sector (512 Bytes)
	uint8_t SectorsPerCluster;				// Sectors per cluster
	uint16_t ReservedSectors;				// Reserved sectors
	uint8_t FatCount;						// Number of FATs on disk
	uint16_t DirEntryCount;					// Number of root directory entries on disk
	uint16_t TotalSectors;					// Total sectors on disk, If this value is 0, it means there are more 
											// than 65535 sectors in the volume. Damn boy you big.
    uint8_t MediaDescriptorType;			// Media Descriptor Type
    uint16_t SectorsPerFat;					// Sectors per FAT
    uint16_t SectorsPerTrack;				// Sectors per track
    uint16_t Heads;							// Number of Heads on disk
    uint32_t HiddenSectors;					// Number of hidden sectors
    uint32_t LargeSectorCount;				// Large sector count. If TotalSectors is more than 64KB then the sectors will be stored here.

    // EBR
    uint8_t DriveNumber;
    uint8_t _Reserved;
    uint8_t Signature;
    uint32_t VolumeId;          // serial number, value doesn't matter
    uint8_t VolumeLabel[11];    // 11 bytes, padded with spaces
    uint8_t SystemId[8];

    /* ...Nobody cares about code... */

} __attribute__((packed)) BootSector;

// FAT Directory Entry
typedef struct 
{
    uint8_t Name[11];				// 8.3 format
    uint8_t Attributes;				// Attributes
    uint8_t _Reserved;				// Why?
    uint8_t CreatedTimeTenths;		// Times in tenths
    uint16_t CreatedTime;			// Creation time
    uint16_t CreatedDate;			// Creation date
    uint16_t AccessedDate;			// Accessed date
    uint16_t FirstClusterHigh;		// ??
    uint16_t ModifiedTime;			// Modification time
    uint16_t ModifiedDate;			// Modification date
    uint16_t FirstClusterLow;		// ?
    uint32_t Size;					// Size of entry
} __attribute__((packed)) DirectoryEntry;

bool readBootSector();
bool readSectors(uint32_t lba, uint32_t count, void* bufferOut);
bool readFat();
bool readRootDirectory();
//DirectoryEntry* findFile(const char* name);
bool readFile(DirectoryEntry* fileEntry, uint8_t* outputBuffer);

#endif