#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "multiboot.h"
#include "ide.h"
#include "fat.h"

char oem[] = "SPC";

int main(unsigned int ebx)
{
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();

    // Initialise the screen (by clearing it)
    monitor_clear();

    monitor_write("\nxvX Operating System\nKernel: AHK 0.7.3\nOEM: ");
    monitor_write(oem);
    monitor_write("\n");

    monitor_write("Disk Driver Initialized: \n");

    ata_init();

    const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
    const uint32 LBA = 0;
    const uint8 NO_OF_SECTORS = 1;
    char buf[ATA_SECTOR_SIZE] = {0};

    struct example 
    {
        int id;
        char name[32];
    };

    struct example e;
    e.id = 10012;
    strcpy(e.name, "Iron Man");

    void* ptr;

    memset(ptr, 0, sizeof(ptr));
    readSectors(LBA + 1, 1, ptr);
    strcpy(e.name, "FAT Read Sectors Failed!");
    memcpy(&e, ptr, sizeof(e));

    monitor_write(e.name);

    readBootSector();
    readFat();

    // load sample program to check if disk or iso is corrupted
    //monitor_write("\nSearching for /modules/program...    ");

    multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
  	multiboot_module_t* modules = (multiboot_module_t*) mbinfo->mods_addr; 
  	unsigned int address_of_module = modules->mod_start;

 //  	if((mbinfo->mods_count) == 1)
 //  	{
 //  		monitor_write("Found program\n");
 //  		monitor_write("Starting program...\n");
  		
 //  		typedef void (*call_module_t)(void);
 //      	call_module_t start_program = (call_module_t) address_of_module;
 //     	start_program();

 //     	//monitor_write("\nISO Image is OK! SUCCESS!\n");
	// }
	// else
	// {
	// 	monitor_write("Program not found! FAIL!\n");
	// 	monitor_write("ISO Image is CORRUPTED! FAIL!\n");
	// }

    return 0;
}
