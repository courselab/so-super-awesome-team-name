/*
 *    SPDX-FileCopyrightText: 2024 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2025 Arthur Vergaças <arthurvdm@gmail.com>
 *
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
 *  and includes modifications made by the following author(s):
 *  Arthur Vergaças <arthurvdm@gmail.com>
 */

/* Library libunodos.a should be statically linked against user programs meant
   for running on TyDOS. It provides some custom C functions that invoke system
   calls for trivial tasks.

   This is the header file that should be included in the user programs. */

#ifndef UNODOS_H
#define UNODOS_H

/* Syscall numbers. */

#define SYS_INVALID 0
#define SYS_EXIT 1
#define SYS_WRITE 2
#define SYS_READ 3

#define GETS_BUFFER_MAX_SIZE 10

void puts(const char* str); /* Outputs 'str' on the screen. */

void gets(char* buffer);

#endif /* UNODOS_H  */
