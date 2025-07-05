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

/* This source file implements the kernel entry function 'kmain' called
   by the bootloader, and the command-line interpreter. Other kernel functions
   were implemented separately in another source file for legibility. */

#include "kernel.h" /* Essential kernel functions.  */
#include "bios1.h"  /* For kwrite() etc.            */
#include "bios2.h"  /* For kread() etc.             */
#include "kaux.h"   /* Auxiliary kernel functions.  */
#include "file_system.h"

/* Kernel's entry function. */

void kmain(void) {
  register_syscall_handler(); /* Register syscall handler at int 0x21.*/

  splash(); /* Uncessary spash screen.              */

  shell(); /* Invoke the command-line interpreter. */

  halt(); /* On exit, halt.                       */
}

/* UnoDOS Shell (command-line interpreter). */

char buffer[BUFF_SIZE];
int go_on = 1;

void shell() {
  int i;
  clear();
  kwrite("UnoDOS 1.0\n");

  while (go_on) {

    /* Read the user input.
       Commands are single-word ASCII tokens with no blanks. */
    do {
      kwrite(PROMPT);
      kread(buffer);
    } while (!buffer[0]);

    /* Check for matching built-in commands */

    i = 0;
    while (cmds[i].funct) {
      if (!strcmp(buffer, cmds[i].name)) {
        cmds[i].funct();
        break;
      }
      i++;
    }

    if (!cmds[i].funct) {
      int result = exec_binary_file(buffer);
      if (result == 0) {
        continue;
      }

      kwrite("Command not found\n");
    }
  }
}

/* Array with built-in command names and respective function pointers.
   Function prototypes are in kernel.h. */

struct cmd_t cmds[] =
    {
        {"help", f_help}, /* Print a help message.       */
        {"quit", f_quit}, /* Exit TyDOS.                 */
        {"list", f_list}, /* List files.                 */
        {0, 0}};

/* Build-in shell command: help. */

void f_help() {
  kwrite("...me, Obi-Wan, you're my only hope!\n\n");
  kwrite("   But we can try also some commands:\n");
  kwrite("      hello   (to run a sample user program\n");
  kwrite("      quit    (to exit TyDOS)\n");
}

void f_quit() {
  kwrite("Program halted. Bye.\r\n");
  go_on = 0;
}

void f_list() {
  struct fs_header_t *fs_header = get_fs_header();

  int sector_coordinate = 1 + fs_header->number_of_boot_sectors;
  int sectors_to_read = fs_header->number_of_file_entries * DIR_ENTRY_LEN / SECTOR_SIZE;

  extern byte _MEM_POOL;
  void *directory_section = (void *)&_MEM_POOL;

  load_disk_into_memory(sector_coordinate, sectors_to_read, directory_section);

  for (int i = 0; i < fs_header->number_of_file_entries; i++) {
    char *file_name = directory_section + i * DIR_ENTRY_LEN;
    if (file_name[0]) {
      kwrite(file_name);
      kwrite("\n");
    }
  }
}

int exec_binary_file(const char *binary_file_name) {
  struct fs_header_t *fs_header = get_fs_header();

  int directory_sector_coordinate = 1 + fs_header->number_of_boot_sectors;
  int sectors_to_read = fs_header->number_of_file_entries * DIR_ENTRY_LEN / SECTOR_SIZE + 1;

  int memoryOffset = fs_header->number_of_file_entries * DIR_ENTRY_LEN - (sectors_to_read - 1) * 512;

  extern byte _MEM_POOL;
  void *directory_section = (void *)&_MEM_POOL;

  load_disk_into_memory(directory_sector_coordinate, sectors_to_read, directory_section);

  int bin_sector_coordinate = -1;
  for (int i = 0; i < fs_header->number_of_file_entries; i++) {
    char *file_name = directory_section + i * DIR_ENTRY_LEN;
    if (!strcmp(file_name, binary_file_name)) {
      bin_sector_coordinate = directory_sector_coordinate + sectors_to_read + fs_header->max_file_size * i - 1;
      break;
    }
  }

  if (bin_sector_coordinate == -1) {
    return -1;
  }

  void *program = (void *)(USER_PROGRAM_START_ADDR);
  void *program_sector_start = program - memoryOffset;

  load_disk_into_memory(bin_sector_coordinate, fs_header->max_file_size, program_sector_start);

  __asm__ volatile(
      "  call get_return_addr_into_ebx \n"  // coloca o return address em ebx

      "original_return_addr: \n"  // será usado para calcular o valor que deve ser adicionado à stack

      "  push %%ebx \n"  // colocar o ebx na stack

      "  jmp *%[progAddr] \n"  // jump pra main

      "get_return_addr_into_ebx: \n"
      "  mov (%%esp), %%ebx \n"  // coloca o topo da stack em ebx

      "  mov $prog_finish, %%ecx \n"           // ecx = endereço de prog_finish
      "  sub $original_return_addr, %%ecx \n"  // ecx -= endereço de original_return_addr

      "  add %%ecx, %%ebx \n"  // soma ecx em ebx, ou seja, faz com que ebx aponte para prog_finish
      "  ret \n"

      "prog_finish:"

      ::[progAddr] "r"(program));

  return 0;
}
