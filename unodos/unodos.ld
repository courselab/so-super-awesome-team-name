/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2025 Arthur Vergaças <arthurvdm@gmail.com>
 *
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
 *  and includes modifications made by the following author(s):
 *  Arthur Vergaças <arthurvdm@gmail.com>
 */

/* This is the linker script used to build TyDOS. */

OUTPUT_FORMAT(binary)		/* Output flat binary (no structure). */
SECTIONS
{
        . = 0x7C00;		/* Line counter is now 0x7c00.    */

        .bootloader :		/* Bootloader and required files. */
	{
          rt0.o        (.text .data .bss .rodata) /* Runtime initializer. */
          bootloader.o (.text .data .bss .rodata) /* Bootloader itself.   */
          bios1.o      (.text .data .bss .rodata) /* Low-level code.      */
	}

        . = 0x7c00 + 510;	/* Advance 510 bytes. */

        .signature : 		/* Add a signadure section with this content. */
	{
	    	BYTE(0x55)
				BYTE(0xAA)
  }

	_KERNEL_ADDR = .;    /* We'll load the kernel after the bootloader. */

	.kernel :		/* The kernel and remaining files. */
	{
	  kernel.o       (.text .data .bss .rodata) /* The kernel itself.     */
	  kaux.o         (.text .data .bss .rodata) /* Aux. kernel functions. */
	  file_system.o  (.text .data .bss .rodata) /* Aux. file system functions */
	  bios2.o        (.text .data .bss .rodata) /* More low-level code .  */
	  syscall.o      (.text .data .bss .rodata) /* System calls.          */
	  logo.o         (.rodata)		  /* Some ASCII "art".      */
	}

	_KERNEL_SIZE = . - _KERNEL_ADDR; /* How many bytes we'll read.      */

	_END_STACK = 0x7c00;	/* Place the stack bellow the program.      */

	_MEM_POOL = .; /* Start of memory pool */

}
STARTUP(rt0.o)			 /* Prepend with the start file. */




