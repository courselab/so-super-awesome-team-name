#    SPDX-FileCopyrightText: 2025 Arthur Vergaças <vergacas.arthur@bcg.com>
#
#    SPDX-License-Identifier: GPL-3.0-or-later
#
#  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
#  and includes modifications made by the following author(s):
#  <CONTRIBUTOR>

	/* Suppose we receive the argument in %cx. */

	.code16gcc
	.global printf
        .global get_time
        .global get_date
        .global get_year

	.section .text

printf:

	mov   %cx, %bx

        mov   $0x0e,%ah
        mov   $0x0, %si
loop:
        mov   (%bx, %si), %al
        cmp   $0x0, %al
        je    end
        int   $0x10
        add   $0x1, %si
        jmp   loop
end:
        mov %si, %ax
        ret


get_time:
        mov $0x2, %ah # Read time from RTC

        int $0x1a # RTC interrupt service

        mov %cx, %ax # CX contains the time in BCD format

        ret

get_date:
        mov $0x4, %ah # Read date from RTC

        int $0x1a # int 1ah with 04h in %ah puts the month and day in DX,
                  # and the year in CX (BCD encoded)

        mov %dx, %ax

        ret

get_year:
        # Assuming get_date was called before this, so CX already contains the year
        mov %cx, %ax # CX contains the date in BCD format

        ret

