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

/* This is the linker script used to build user programs. */

OUTPUT_FORMAT(binary)		/* Output flat binary (no structure). */
SECTIONS
{
	PRG_LOAD_ADDR = 0xFE00;	/* Program load address.              */

  . = PRG_LOAD_ADDR;

	.prepend :
	{
		crt0.o     (.text .data .bss .rodata) /* Runtime initializer. */
	}

  .bin :
	{
         *.o   (.text .data .bss .rodata) /* Command line objects. */
	  libunodos.o (.text .data .bss .rodata) /* Runtime library. */
	}
}
INPUT(libunodos.a)		/* Link with the UnoDOS user library. */




