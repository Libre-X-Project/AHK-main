#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define SECTOR_SIZE 512
#define NUM_SECTORS 9

struct initrd_header {
    unsigned char magic; // The magic number is there to check for consistency.
    char name[64];
    unsigned int offset; // Offset in the initrd the file starts.
    unsigned int length; // Length of the file.
};

void print_header_info(struct initrd_header *header) {
    printf("magic: %u\n", header->magic);
    printf("name: %s\n", header->name);
    printf("offset: %u\n", header->offset);
    printf("length: %u\n", header->length);
}

void print_sector(FILE *file, long offset) {
    fseek(file, offset, SEEK_SET);
    unsigned char buffer[SECTOR_SIZE];
    size_t read_bytes = fread(buffer, 1, SECTOR_SIZE, file);
    if (read_bytes != SECTOR_SIZE) {
        if (feof(file)) {
            printf("End of file reached before expected\n");
        } else {
            perror("fread");
        }
    }

    printf("Sector at offset 0x%lx:\n", offset);
    for (size_t i = 0; i < read_bytes; ++i) {
        printf("%02X ", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

int main() 
{
    printf("Welcome! Image to process: initrd.img\n");
    FILE *file = fopen("initrd.img", "rb");
    if (!file) 
    {
        perror("fopen");
        return 1;
    }

    printf("Now processing and printing all files and directories found in initrd.img...\n");

    int nheaders;
    fread(&nheaders, sizeof(int), 1, file);
    struct initrd_header headers[64];
    fread(headers, sizeof(struct initrd_header), 64, file);

    // Print header information
    printf("Number of headers: %d\n", nheaders);
    for (int i = 0; i < nheaders; ++i) 
    {
        printf("Header %d:\n", i);
        print_header_info(&headers[i]);
        printf("\n");
    }

    // Read and print sectors 0 to 8
    for (int sector = 0; sector < NUM_SECTORS; ++sector) 
    {
        long OFFSET = SECTOR_SIZE * sector;
        print_sector(file, OFFSET);
    }

    fclose(file);

    return 0;
}
