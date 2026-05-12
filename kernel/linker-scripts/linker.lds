/* Instruct we want an x86_64 ELF64 output file after linking to kernel main*/
OUTPUT_FORMAT(elf64-x86-64)

ENTRY(kmain)

/* Define program headers for limine */
PHDRS {
  /* here we map memory segments (PT_LOAD) with MMU permissions read, write, execute. */
   limine_requests PT_LOAD;
   text PT_LOAD;
   rodata PT_LOAD;
   data PT_LOAD;
}

/* Define section to contain request headers and assign to own program headers */
SECTIONS {
  /* Limine spec mandates places the kernel in the topmost 2gb of address space for optimization */
  . = 0xffffffff80000000;

  .limine_requests : {
    KEEP(*(.limine_requests_start))
    KEEP(*(.limine_requests))
    KEEP(*(.limine_requests_start))
  } :limine_requests

  /* Hop to next memory page */
  . = ALIGN(CONSTANT(MAXPAGESIZE));

  /* 
  * Assign text output region called text that aggregates all input .text and .text.* 
  * If still confused see stack overflow explanations. Basically just a pattern selection like in bash
  */
  .text : {
    *(.text .text.*)
  } :text

  /* Hope to next memory page for .rodata */
  . = ALIGN(CONSTANT(MAXPAGESIZE))

  /*
   * Assign READ ONLY output region to all input .rodata
   * We will ALSO puta  gnu build id in this region, alongside all other read only data, because it is not valuable for execution like text (code) or data (writeable segments)
   */
  .rodata : {
    *(.rodata .rodata.*)
  } :rodata

  .note.gnu.build-id {
    *(.note.gnu.build-id)
  } :rodata

  /* Move to next memory page for data (writeable segments) */
  . = ALIGN(CONSTANT(MAXPAGESIZE))

  .data : {
    *(.data .data.*)
  }:data

  /* 
   * Zero initialize the remaining file for memory efficiency 
   * If there were anythign else to initialize (.init_array, .finit_array), PLACE ABOVE
   */
  .bss : {
    *(.bss .bss.*)
    *(COMMON)
  }:data

  /DISCARD/ : {
      *(.eh_frame*)
      *(.note .note.*)
    }
}
