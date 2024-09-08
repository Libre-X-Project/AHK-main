/*
	AFS Interface for applications and drivers.
	Written by AHK Developer(s): Kap Petrov <Advanced/Adaptable Hybrid Kernel Team>
	See tools/afs for a in-depth overview
*/

#ifndef VFS_H
#define VFS_H

#include "common.h"
#include "monitor.h"
#include "fs.h"

/*
	Implementation based on tools/afs/DOCUMENTATION.md
	read it to understand what's happening.
	- Kap Petrov
*/

typedef struct
{
   uint32_t nfiles;
} fs_header_t;

typedef struct
{
   uint8_t magic; 				// Magic number, for error checking of file system corruption.
   char name[64]; 				// Filename.
   uint32_t offset; 			// Offset in the disk that the file starts.
   uint32_t length; 			// Length of the file.
} fs_file_header_t;

fs_node_t *initialise_disk();

#endif