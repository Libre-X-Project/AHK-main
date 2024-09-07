#ifndef FS_H
#define FS_H

#include "common.h"
#include "monitor.h"
#include "ide.h"

// File flags/attributes
#define FS_NONE		   0x00
#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_SYS		   0x03
#define FS_HIDDEN	   0x04
#define FS_READONLY	   0x05

// 140 bytes
typedef struct fs_node
{
	char name[11];     // The filename.
	uint32_t flags;     // Includes the node type (Directory, file etc).
	uint32_t inode;		// Driver-specific number to identify file.
	uint32_t sector;
} fs_node_t;

struct fs_return
{
	char name[128];    // Filename.
	uint32_t ino;      // Inode number.
};

//fs_node_t root[4] = {0};

fs_node_t readdir(fs_node_t node);
fs_node_t readnode(fs_node_t node);
void writenode(fs_node_t node);

void writedata(fs_node_t node, char* data);
void readdata(fs_node_t node, char* bufferOut);

void writeroot();
void readroot();

fs_node_t* getroot();

void listdir();

#endif