#    SPDX-FileCopyrightText: 2025 Arthur Vergaças <arthurvdm@gmail.com>
#
#    SPDX-License-Identifier: GPL-3.0-or-later
#
#  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
#  and includes modifications made by the following author(s):
#  Arthur Vergaças <arthurvdm@gmail.com>

# By default, the bootable program name will be unodos.bin
# If you export this directory to extend the example as part of a programming
# exercise, redefine $(dos) to reflect your own DOS name, say
# dos = "amazingOS"

dos=$(shell basename $$(realpath .))

# Build the OS and an example user program.

# Link all objects needed by the OS.

$(dos).bin : bootloader.o bios1.o kernel.o file_system.o kaux.o bios2.o logo.o syscall.o
	ld -melf_i386 -T unodos.ld --orphan-handling=discard $^ -o $@

# Here we are statically linking the user program 'prob.bin' into the kernel,
# so as to simulate the execution of a user program. If we were to actually load
# and execute an external program, we should remove 'prog.o' and 'libunodos.o'
# from the list of pre-requisites, and edit the linker script accordingly.
# Comment out the following line if this is the case.

$(dos).bin : prog.o libunodos.o

# Rules to build objects from either C or assembly code.

%.o : %.c
	gcc -m16 -O0 --freestanding -fno-pic -fcf-protection=none -c  $< -o $@

%.o : %.S
	as -32 $< -o $@

bootloader.o : bios1.h kernel.h
kernel.o : bios1.h bios2.h kernel.h kaux.h file_system.h
kaux.o:    bios2.h kaux.h
file_system.o: kaux.h file_system.h

$(dos).bin : .EXTRA_PREREQS = rt0.o unodos.ld

# Rules to build the user programs
# You would add new programs to this variable if bulding other user programs.
# The user library is automatically added by the linker script.

progs = prog.bin userp.bin

$(progs)  : %.bin : %.o libunodos.a
	ld -melf_i386 -T prog.ld --orphan-handling=discard $< -o $@

$(progs:%.bin=%.o) : %.o : %.c unodos.h
	gcc -m16 -O0 --freestanding -fno-pic -fcf-protection=none -c $< -o $@

$(progs:%.bin=%.o) : unodos.h

$(progs:%.bin=%.o) : .EXTRA_PREREQS = crt0.o prog.ld

# Recipes to build the user library.

libunodos.o: libunodos.c unodos.h
	gcc -m16 -O0 --freestanding -fno-pic -fcf-protection=none -c  $< -o $@

libunodos.o : unodos.h

libunodos.a : libunodos.o
	ar rcs $@ $^

# Create a 1.44 MB floppy image (2880 * 512 bytes)

disk.img:
	rm -f $@
	dd if=/dev/zero of=$@ count=2880

load-dos: $(dos).bin
	dd bs=1 if=$< of=disk.img skip=16 seek=16 conv=notrunc

# Housekeeping.

.PHONY: clean

clean:
	rm -f *.bin *.o *~ *.s *.a
