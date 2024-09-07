#include "fs.h"

fs_node_t root[4] = {0};

fs_node_t readdir(fs_node_t node)
{
    fs_node_t r;

    if(node.flags != FS_DIRECTORY)
    {
        monitor_write("ERROR READING DIRECTORY! SPECIFIED NODE IS A FILE! INVALID DIRECTORY\n");
    }
    else
    {
        r = readnode(node);
    }

    return r;
}

fs_node_t readnode(fs_node_t node)
{
    fs_node_t result;

    const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
    const uint32 LBA = 0;
    const uint8 NO_OF_SECTORS = 1;
    char buf[ATA_SECTOR_SIZE] = {0};

    memset(buf, 0, sizeof(buf));
    ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA + node.sector, (uint32)buf);
    memcpy(&result, buf, sizeof(result));

    return result;
}

void writenode(fs_node_t node)
{
    const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
    const uint32 LBA = 0;
    const uint8 NO_OF_SECTORS = 1;
    char buf[ATA_SECTOR_SIZE] = {0};

    memset(buf, 0, sizeof(buf));
    memcpy(buf, &node, sizeof(node));
    ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA + node.sector, (uint32)buf);

    for (int i = 0; i < 4; i++) 
    {
        if (strcmp(root[i].name, "") == 0) 
        {
            root[i] = node;
            break;
        }
    }
}

void writedata(fs_node_t node, char* data)
{
    const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
    const uint32 LBA = 0;
    const uint8 NO_OF_SECTORS = 1;
    char buf[ATA_SECTOR_SIZE] = {0};

    memset(buf, 0, sizeof(buf));
    strcpy(buf, data);
    ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA + node.sector + 1, (uint32)buf);
}

void readdata(fs_node_t node, char* bufferOut)
{
    const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
    const uint32 LBA = 0;
    const uint8 NO_OF_SECTORS = 1;
    char buf[ATA_SECTOR_SIZE] = {0};

    memset(buf, 0, sizeof(buf));
    ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA + node.sector + 1, (uint32)buf);
    memcpy(bufferOut, buf, ATA_SECTOR_SIZE);
}

void writeroot()
{
    const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
    const uint32 LBA = 0;
    const uint8 NO_OF_SECTORS = 1;
    char buf[ATA_SECTOR_SIZE] = {0};

    memset(buf, 0, sizeof(buf));
    memcpy(buf, root, sizeof(root));
    ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA + 6, (uint32)buf);
}

void readroot()
{
    const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
    const uint32 LBA = 0;
    const uint8 NO_OF_SECTORS = 1;
    char buf[ATA_SECTOR_SIZE] = {0};

    memset(buf, 0, sizeof(buf));
    ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA + 6, (uint32)buf);
    memcpy(root, buf, sizeof(root));
}

fs_node_t* getroot()
{
    return root;
}

void GetRootDir(fs_node_t* node)
{
    node = root;
}

void listdir()
{
    //readroot();

    for(int i = 0; i < 4; i++)
    {
        if(root[i].flags != FS_NONE)
        {
            monitor_write("Filename: ");
            monitor_write(root[i].name);
            monitor_write("\n");

            if(root[i].flags == FS_DIRECTORY)
            {
                monitor_write("(Directory)\n");
            }
            else
            {
                monitor_write("(File)\n");
                monitor_write("Contents: \n");
                char rd[ATA_SECTOR_SIZE];
                readdata(root[i], rd);
                monitor_write(rd);
                monitor_write("\n");
            }
        }
    }
}