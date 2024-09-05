#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "multiboot.h"

char oem[] = "SPC";

int main(unsigned int ebx)
{
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();

    // Initialise the screen (by clearing it)
    monitor_clear();

    monitor_write("\nxvX Operating System\nKernel: AHK 0.6.5\nOEM: ");
    monitor_write(oem);
    monitor_write("\n");

    monitor_write("\nSearching for /modules/program...    ");

    multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
  	multiboot_module_t* modules = (multiboot_module_t*) mbinfo->mods_addr; 
  	unsigned int address_of_module = modules->mod_start;

  	if((mbinfo->mods_count) == 1)
  	{
  		monitor_write("Found program\n");
  		monitor_write("Starting program...\n");
  		
  		typedef void (*call_module_t)(void);
        call_module_t start_program = (call_module_t) address_of_module;
       	start_program();

       	monitor_write("\nProgram returned! FAIL!\n");
	}
	else
	{
		monitor_write("Program not found! FAIL!\n");
	}

    return 0;
}
