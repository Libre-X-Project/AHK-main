#!/bin/bash

FILE="initrd.img"
SECTOR_SIZE=512
TOTAL_SECTORS=$(stat -c%s "$FILE")
TOTAL_SECTORS=$((TOTAL_SECTORS / SECTOR_SIZE))

for ((i=0; i<TOTAL_SECTORS; i++)); do
	    OFFSET=$((i * SECTOR_SIZE))
	        echo "Sector $i:"
		    hexdump -s "$OFFSET" -n "$SECTOR_SIZE" -C "$FILE"
		        echo
		done
		#!/bin/bash

		FILE="initrd.img"
		SECTOR_SIZE=512
		TOTAL_SECTORS=$(stat -c%s "$FILE")
		TOTAL_SECTORS=$((TOTAL_SECTORS / SECTOR_SIZE))

		for ((i=0; i<TOTAL_SECTORS; i++)); do
			    OFFSET=$((i * SECTOR_SIZE))
			        echo "Sector $i:"
				    hexdump -s "$OFFSET" -n "$SECTOR_SIZE" -C "$FILE"
				        echo
				done

