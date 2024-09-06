/*
	Fat32 Driver from AHK for Host System.
	Do not use for production
	Free to modify this file
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef uint8_t bool;
#define true 1
#define false 0

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

    /* Nobody cares about you code... */

} __attribute__((packed)) BootSector;

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

BootSector g_BootSector;
uint8_t* g_Fat = NULL;
DirectoryEntry* g_RootDirectory = NULL;
uint32_t g_RootDirectoryEnd;

// Read the boot sector from the disk image
bool readBootSector(FILE* disk)
{
    return fread(&g_BootSector, sizeof(g_BootSector), 1, disk) > 0;
}

// Read sectors off disk
bool readSectors(FILE* disk, uint32_t lba, uint32_t count, void* bufferOut)
{
    bool ok = true;
    ok = ok && (fseek(disk, lba * g_BootSector.BytesPerSector, SEEK_SET) == 0);
    ok = ok && (fread(bufferOut, g_BootSector.BytesPerSector, count, disk) == count);
    return ok;
}

// Read File Allocation Table
bool readFat(FILE* disk)
{
    g_Fat = (uint8_t*) malloc(g_BootSector.SectorsPerFat * g_BootSector.BytesPerSector);
    return readSectors(disk, g_BootSector.ReservedSectors, g_BootSector.SectorsPerFat, g_Fat);
}

// Read Root Directory Entry
bool readRootDirectory(FILE* disk)
{
    uint32_t lba = g_BootSector.ReservedSectors + g_BootSector.SectorsPerFat * g_BootSector.FatCount;
    uint32_t size = sizeof(DirectoryEntry) * g_BootSector.DirEntryCount;
    uint32_t sectors = (size / g_BootSector.BytesPerSector);
    if (size % g_BootSector.BytesPerSector > 0)
        sectors++;

    g_RootDirectoryEnd = lba + sectors;
    g_RootDirectory = (DirectoryEntry*) malloc(sectors * g_BootSector.BytesPerSector);
    return readSectors(disk, lba, sectors, g_RootDirectory);
}

// Find file off of FAT12
DirectoryEntry* findFile(const char* name)
{
    for (uint32_t i = 0; i < g_BootSector.DirEntryCount; i++)
    {
        if (memcmp(name, g_RootDirectory[i].Name, 11) == 0)
            return &g_RootDirectory[i];
    }

    return NULL;
}

// Read file off of FAT12
bool readFile(DirectoryEntry* fileEntry, FILE* disk, uint8_t* outputBuffer)
{
    bool ok = true;
    uint16_t currentCluster = fileEntry->FirstClusterLow;

    do {
        uint32_t lba = g_RootDirectoryEnd + (currentCluster - 2) * g_BootSector.SectorsPerCluster;
        ok = ok && readSectors(disk, lba, g_BootSector.SectorsPerCluster, outputBuffer);
        outputBuffer += g_BootSector.SectorsPerCluster * g_BootSector.BytesPerSector;

        uint32_t fatIndex = currentCluster * 3 / 2;
        if (currentCluster % 2 == 0)
            currentCluster = (*(uint16_t*)(g_Fat + fatIndex)) & 0x0FFF;
        else
            currentCluster = (*(uint16_t*)(g_Fat + fatIndex)) >> 4;

    } while (ok && currentCluster < 0x0FF8);

    return ok;
}

int main(int argc, char** argv)
{
	printf("FAT32 Driver from AHK for Host Systems Linux\n");

	if (argc < 3) 
	{
        printf("Syntax: %s <disk image> <file name>\n", argv[0]);
        return -1;
    }

    FILE* disk = fopen(argv[1], "rb");
    if (!disk) 
    {
        fprintf(stderr, "Cannot open disk image %s!\n", argv[1]);
        return -1;
    }

    if (!readBootSector(disk)) 
    {
        fprintf(stderr, "Could not read boot sector!\n");
        return -2;
    }

    if (!readFat(disk)) 
    {
        fprintf(stderr, "Could not read FAT!\n");
        free(g_Fat);
        return -3;
    }

    if (!readRootDirectory(disk)) 
    {
        fprintf(stderr, "Could not read FAT Root Directory!\n");
        free(g_Fat);
        free(g_RootDirectory);
        return -4;
    }

    DirectoryEntry* fileEntry = findFile(argv[2]);
    if (!fileEntry) 
    {
        fprintf(stderr, "Could not find file %s!\n", argv[2]);
        free(g_Fat);
        free(g_RootDirectory);
        return -5;
    }

    uint8_t* buffer = (uint8_t*) malloc(fileEntry->Size + g_BootSector.BytesPerSector);
    if (!readFile(fileEntry, disk, buffer)) 
    {
        fprintf(stderr, "Could not read file %s!\n", argv[2]);
        free(g_Fat);
        free(g_RootDirectory);
        free(buffer);
        return -5;
    }

    for (size_t i = 0; i < fileEntry->Size; i++)
    {
        if (isprint(buffer[i])) fputc(buffer[i], stdout);
        else printf("<%02x>", buffer[i]);
    }
    printf("\n");

    free(buffer);
    free(g_Fat);
    free(g_RootDirectory);
    return 0;
}
