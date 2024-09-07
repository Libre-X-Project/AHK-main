/*
	FAT12 Driver Implementation
	Written by AHK Developer(s): <Advanced/Adaptable Hybrid Kernel Team>
	See tools/fat for a in-depth overview
*/

#ifndef FAT_H
#define FAT_H

#include "common.h"
#include "ide.h"

bool readSectors(uint32_t lba, uint32_t count, void* bufferOut);

#endif