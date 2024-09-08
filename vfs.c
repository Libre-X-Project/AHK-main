#include "vfs.h"

fs_header_t *disk_header;     // The header.
fs_file_header_t *file_headers; // The list of file headers.
fs_node_t *initrd_root;             // Our root directory node.
fs_node_t *initrd_dev;              // We also add a directory node for /dev, so we can mount devfs later on.
fs_node_t *root_nodes;              // List of file nodes.
int nroot_nodes;                    // Number of file nodes.

struct dirent dirent;

static uint32_t disk_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
	fs_file_header_t header = file_headers[node->inode];
	if (offset > header.length)
		return 0;

	if (offset+size > header.length)
		size = header.length-offset;

	memcpy(buffer, (uint8_t*) (header.offset+offset), size);
	return size;
} 

static struct dirent *disk_readdir(fs_node_t *node, uint32_t index)
{
	if (node == initrd_root && index == 0)
	{
		strcpy(dirent.name, "dev");
		dirent.name[3] = 0; // Make sure the string is NULL-terminated.
		dirent.ino = 0;
		return &dirent;
	}

	if (index-1 >= nroot_nodes)
		return 0;

	strcpy(dirent.name, root_nodes[index-1].name);
	dirent.name[strlen(root_nodes[index-1].name)] = 0; // Make sure the string is NULL-terminated.
	dirent.ino = root_nodes[index-1].inode;
	return &dirent;
}

static fs_node_t *disk_finddir(fs_node_t *node, char *name)
{
	if (node == initrd_root && !strcmp(name, "dev") )
		return initrd_dev;

	int i;
	for (i = 0; i < nroot_nodes; i++)
		if (!strcmp(name, root_nodes[i].name))
			return &root_nodes[i];

	return 0;
}

fs_node_t *initialise_disk()
{
	// TODO! I still don't have a malloc so...

	// Set disk_header and file headers

	// Init root dir

	// Init dev dir

	// Allocate space for files

	// Add files in a loop

	// Return root node
}