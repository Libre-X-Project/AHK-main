/*
	FAT12 Driver Implementation
	Written by AHK Developer(s): Kap Petrov <Advanced/Adaptable Hybrid Kernel Team>
	See tools/fat for a in-depth overview

	[UNFINISHED]
*/

#include "fat.h"
#include "monitor.h"

// Read Sectors from disk
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