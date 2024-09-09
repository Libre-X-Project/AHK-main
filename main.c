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

    monitor_write("\nxvX Operating System\nKernel: AHK 0.9.9\n");

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

    monitor_write("\nReading Hard Disk and Verifying AFS Header...    ");

    if(result < 0)
    {
	    monitor_write("FAILED!\n");
	    monitor_write("No AFS Header found! Are you sure this hard disk is formatted to AHK File System (AFS)?\n");
	}
	else
	{
		monitor_write("SUCCESS!\n");
	}

    monitor_write("Files found in / (root): ");
    monitor_write_dec(result);
    monitor_write("\n");

    struct initrd_header headers[64];
    memcpy(headers, buf + sizeof(int), sizeof(struct initrd_header) * 64);

    monitor_write("Listing files on disk...\n");

 //    monitor_write("Name: ");
 //    monitor_write(headers[0].name);
 //    monitor_write("\n");

	// monitor_write("Magic: ");
 //    monitor_write_hex(headers[0].magic);
 //    monitor_write("\n");

 //    monitor_write("Offset: ");
 //    monitor_write_dec(headers[0].offset);
 //    monitor_write("\n");

	// monitor_write("Length: ");
 //    monitor_write_dec(headers[0].length);
 //    monitor_write("\n");    

 //    monitor_write("Contents: \n");

 //    char content[512];
 //    memcpy(content, buf + sizeof(int) + sizeof(struct initrd_header) * 64 + headers[0].length, sizeof(content));

 //    monitor_write(content);

    int privI = 0;

    for(int i = 0; i < result; i++)
    {
    	monitor_write("Name: ");
	    monitor_write(headers[i].name);
	    monitor_write("\n");

	    monitor_write("Contents: \n");

	    char content[512];

	    if(i == 0)
	    	memcpy(content, buf + sizeof(int) + sizeof(struct initrd_header) * 64, headers[i].length);
	    else
	    	memcpy(content, buf + sizeof(int) + sizeof(struct initrd_header) * 64 + headers[privI].length, headers[i].length);

	    monitor_write(content);
	    monitor_write("\n");

	    privI = i;
    }

    return 0;
}
