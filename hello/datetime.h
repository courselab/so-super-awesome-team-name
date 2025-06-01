/*
 *    SPDX-FileCopyrightText: 2025 Arthur Vergaças <vergacas.arthur@bcg.com>
 *
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
 *  and includes modifications made by the following author(s):
 *  <CONTRIBUTOR>
 */

#ifndef DATETIME_H
#define DATETIME_H

int __attribute__((fastcall, naked)) get_time();
int __attribute__((fastcall, naked)) get_date();
int __attribute__((fastcall, naked)) get_year();

#endif
