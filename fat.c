/*
	FAT12 Driver Implementation
	Written by AHK Developer(s): Kap Petrov <Advanced/Adaptable Hybrid Kernel Team>
	See tools/fat for a in-depth overview

	[UNFINISHED]
*/

#include "fat.h"
#include "monitor.h"

// Initialize variables
BootSector g_BootSector;
uint8_t BootSectorBytes[ATA_SECTOR_SIZE];

static uint8_t* g_Fat = NULL;
static uint32_t g_DataSectionLba;

// Read FAT Boot Sector
bool readBootSector()
{
	bool ok = true;

	const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
    const uint32 LBA = 0;
    const uint8 NO_OF_SECTORS = 1;
    char buf[ATA_SECTOR_SIZE] = {0};

    ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);

    memcpy(BootSectorBytes, buf, ATA_SECTOR_SIZE);

    // monitor_write_dec((u32int)BootSectorBytes);
    // monitor_write("\n\n");
    // monitor_write(BootSectorBytes);

	return ok;
}

// Read Sectors from disk

/*
	You can actually use FAT12 on hard drives. using that knowledge,
	i was able to run my formatted disk with no issues in qemu.
	Good to know that i don't have to make another driver for
	FAT32 support. Although i may do it later. FAT12 doesn't
	really fit the scheme of the modern era. It's all FAT32
	now. Well, with bits of NTFS, APFS, HFS+, and, ...
	nothing i guess.
	-Kap Petrov, AHK Developer Lead.
*/
bool readSectors(uint32_t lba, uint32_t count, void* bufferOut)
{
	bool ok = true;

	const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
    const uint32 LBA = lba;
    const uint8 NO_OF_SECTORS = count;
    char buf[ATA_SECTOR_SIZE] = {0};

    ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);

    memcpy(bufferOut, buf, ATA_SECTOR_SIZE);

	return ok;
}

// Read File Allocation Table
bool readFat()
{
	bool ok = true;

	const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
    const uint32 LBA = g_BootSector.ReservedSectors;
    const uint8 NO_OF_SECTORS = g_BootSector.SectorsPerFat;
    char buf[ATA_SECTOR_SIZE] = {0};

    ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);

    memcpy(g_Fat, buf, ATA_SECTOR_SIZE);

    monitor_write("\n");
    monitor_write_dec((u32int)g_Fat);
    monitor_write("\n\n");
    monitor_write(g_Fat);

	return ok;
}

// Read FAT Root Directory (/)
bool readRootDirectory()
{
	bool ok = true;
	return ok;
}

// Read File from FAT
bool readFile(DirectoryEntry* fileEntry, uint8_t* outputBuffer)
{
	bool ok = true;
	return ok;
}