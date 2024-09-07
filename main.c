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

    monitor_write("\nxvX Operating System\nKernel: AHK 0.9.7\n");

    monitor_write("Disk Driver Initialized: \n");

    ata_init();

    const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
    const uint32 LBA = 0;
    const uint8 NO_OF_SECTORS = 1;
    char buf[ATA_SECTOR_SIZE] = {0};

    strcpy(buf, "Hello World From Disk!");
    ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);

    memset(buf, 0, sizeof(buf));
   	ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);

    monitor_write(buf);

    monitor_write("\nLoading /modules/testmodule.txt...    ");

    multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
  	multiboot_module_t* modules = (multiboot_module_t*) mbinfo->mods_addr; 
  	unsigned int address_of_module = modules->mod_start;

  	monitor_write("Found! Dumping contents...\n");

  	monitor_write((char*)address_of_module);

  	monitor_write("\nGrub Module Loaded Successfully!\n");

    fs_node_t testFile;

    strcpy(testFile.name, "test.txt   ");
    testFile.flags = FS_FILE;
    testFile.inode = 0;
    testFile.sector = 1;

    writenode(testFile);
    writedata(testFile, "Banana.");

    fs_node_t testDir;

    strcpy(testDir.name, "mydir       ");
    testDir.flags = FS_DIRECTORY;
    testDir.inode = 1;
    testDir.sector = 3;

    writenode(testDir);

    fs_node_t testCap;

    strcpy(testCap.name, "THEtAb.txt  ");
    testCap.flags = FS_FILE;
    testCap.inode = 2;
    testCap.sector = 5;

    writenode(testCap);
    writedata(testCap, "This is a test file for testing capitals on the filenames.");

    //writeroot();

    monitor_write("\nDirectory: /\n");
    listdir();

    return 0;
}
