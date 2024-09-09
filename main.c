#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "multiboot.h"
#include "ide.h"
#include "fs.h"

int main(unsigned int ebx)
{
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();

    // Initialise the screen (by clearing it)
    monitor_clear();

    monitor_write("\nxvX Operating System\nKernel: AHK 0.9.8\n");

    monitor_write("Disk Driver Initialized: \n");

    ata_init();

    const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
    const uint32 LBA = 0;
    const uint8 NO_OF_SECTORS = 1;
    char buf[ATA_SECTOR_SIZE] = {0};

    struct initrd_header 
    {
        unsigned char magic; // The magic number is there to check for consistency.
        char name[64];
        unsigned int offset; // Offset in the initrd the file starts.
        unsigned int length; // Length of the file.
    };

    memset(buf, 0, sizeof(buf));
   	ide_read_sectors(DRIVE, NO_OF_SECTORS + 9, LBA, (uint32)buf);

   	int result; // afs header
    memcpy(&result, buf, sizeof(int));

    monitor_write("AFS Header: ");
    monitor_write_dec(result);
    monitor_write("\n");

    struct initrd_header headers[64];
    memcpy(headers, buf + sizeof(int), sizeof(struct initrd_header) * 64);

    monitor_write("Listing first file on disk...\n");

    monitor_write("Name: ");
    monitor_write(headers[0].name);
    monitor_write("\n");

	monitor_write("Magic: ");
    monitor_write(headers[0].magic);
    monitor_write("\n");    

   //  monitor_write(buf);

   //  monitor_write("\nLoading /modules/testmodule.txt...    ");

   //  multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
  	// multiboot_module_t* modules = (multiboot_module_t*) mbinfo->mods_addr; 
  	// unsigned int address_of_module = modules->mod_start;

  	// monitor_write("Found! Dumping contents...\n");

  	// monitor_write((char*)address_of_module);

  	// monitor_write("\nGrub Module Loaded Successfully!\n");

    monitor_write("\nReading Hard Disk and Verifying AFS Header...    ");
    // TODO:
    // Initialize File System here... ( set fs_root to initialise_disk(); )

    if(result < 0)
    {
	    monitor_write("FAILED!\n");
	    monitor_write("No AFS Header found! Are you sure this hard disk is formatted to AHK File System (AFS)?\n");
	}
	else
	{
		monitor_write("SUCCESS!\n");
	}

    return 0;
}
