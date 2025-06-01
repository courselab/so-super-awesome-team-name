/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2025 Arthur Vergaças <vergacas.arthur@bcg.com>
 *
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
 *  and includes modifications made by the following author(s):
 *  <CONTRIBUTOR>
 */

#include <stdio.h>
#include <datetime.h>

void __attribute__((fastcall)) bcd_to_str(short bcd, char* target, short size) {
  for (short i = 0; i < size; i++) {
    target[i] = ((bcd >> (size - i - 1) * 4) & 0xf) + '0';
  }

  target[size] = '\0';
}

int main(void) {
  // TIME
  short time = get_time();

  short hours = (time >> 2 * 4);
  short minutes = time;

  char hours_str[3];
  bcd_to_str(hours, hours_str, 2);

  char minutes_str[3];
  bcd_to_str(minutes, minutes_str, 2);

  // DATE

  short date = get_date();

  short month = (date >> 2 * 4);
  short day = date;
  short year = get_year();  // Needs to be called after get_date()

  char day_str[3];
  bcd_to_str(day, day_str, 2);

  char month_str[3];
  bcd_to_str(month, month_str, 2);

  char year_str[5];
  bcd_to_str(year, year_str, 4);

  printf(day_str);
  printf("/");
  printf(month_str);
  printf("/");
  printf(year_str);
  printf(" ");

  printf(hours_str);
  printf(":");
  printf(minutes_str);
  printf(" UTC");

  return 0;
}
