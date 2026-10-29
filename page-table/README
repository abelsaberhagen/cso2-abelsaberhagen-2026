How to customize config.h
---------------------------------------------
Config.h contains two variables, LEVELS and POBITS. LEVELS represents the number of layers of page tables. POBITS is the size of page offset or the number of offset bits that is attached to each virtual address. 

Page tables are data structures that Map virtual addresses used by programs to physical addresses stored in RAM. Page tables are made up of rows of page table entries which map to a region of physical memory. If LEVELS > 1, then you will point to a page table that is stored within the address of the rows of a lower level page table.

Each address can be split up into a page number and a page offset. When an address is accessed, that address is translated by looking through a map to find the corresponding page table entry. The page table entry will contain a physical page number that is concatenated with the page offset to receive the physical addess.

The number of offset bits will change the page size, which is 2^POBITS, the virtual page number, which is the size of the virtual address in bytes - POBITS, as well as the number of page table entries. 