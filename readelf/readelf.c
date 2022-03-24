/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include "types.h"
#include <stdio.h>
/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int is_elf_format(u_char *binary)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
        if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
            ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
            ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
            ehdr->e_ident[EI_MAG3] == ELFMAG3)
        {
                return 1;
        }

        return 0;
}

/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */

/*
    Exercise 1.2. Please complete func "readelf".
*/

#define align (1LL << 12)
int readelf(u_char *binary, int size)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

        int Nr;

        Elf32_Phdr *phdr1 = NULL, *phdr2 = NULL, *phdr = NULL;

        u_char *ptr_ph_table = NULL;
        Elf32_Half ph_entry_count;
        Elf32_Half ph_entry_size;

        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary))
        {
                printf("not a standard elf format\n");
                return 0;
        }

        // get section table addr, section header number and section header size.
        ptr_ph_table = binary + ehdr->e_phoff;
        ph_entry_count = ehdr->e_phnum;
        ph_entry_size = ehdr->e_phentsize;

        // for each section header, output section number and section addr.
        // hint: section number starts at 0.
        
        int flag = 0;
        for (Nr = 1; Nr < ph_entry_count; Nr++)
        {
        	phdr1 = ((Elf32_Phdr *)(ptr_ph_table + (Nr - 1) * ph_entry_size));
        	phdr2 = ((Elf32_Phdr *)(ptr_ph_table + Nr * ph_entry_size));
        	long unsigned int r1 = phdr1->p_vaddr + phdr1->p_memsz, l2 = phdr2->p_vaddr;
        	if (ROUNDDOWN(r1, align) == ROUNDDOWN(l2, align) && l2 < r1)
			{
				printf ("Conflict at page va : 0x%x\n", ROUNDDOWN(l2, align));
				return 0;
			}
			
        	if (ROUNDDOWN(r1, align) == ROUNDDOWN(l2, align))
        	{
        		printf ("Overlay at page va : 0x%x\n", ROUNDDOWN(l2, align));
        		return 0;
			}
		}
        
        for (Nr = 0; Nr < ph_entry_count; Nr++)
		{
    		phdr = ((Elf32_Phdr *)(ptr_ph_table + Nr * ph_entry_size));
        	printf("%d:0x%x,0x%x\n", Nr, phdr->p_filesz, phdr->p_memsz);
		}
        return 0;
}
