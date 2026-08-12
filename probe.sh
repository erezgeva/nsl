#!/bin/bash -e
# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright © 2026 Erez Geva <ErezGeva2@gmail.com>
#
# @author Erez Geva <ErezGeva2@@gmail.com>
# @copyright © 2026 Erez Geva
#
###############################################################################
try_spec()
{
  local -r spc="$1" uspc="$2"
  printf "$start%s$end" $spc > out/tmp1.c
  if $CC2 -Wall -Wextra -Werror out/tmp1.c -o out/tmp1$EXE &> /dev/null;then
    dspec="$spc"
    uspec="$uspc"
    return 1 # error will cause find_specifier to return 0
  fi
  return 0 # this will ccause find_specifier to try the next specifier
}
find_specifier()
{
  # Find poprer uint64_t/int64_t specifier
  # long
  try_spec '"%ld"'   '"%lu"' || return 0
  # long long
  try_spec '"%lld"'  '"%llu"' || return 0
  # PRId64
  try_spec 'PRId64'  'PRIu64' || return 0
  # Microsoft visual C++ compiler specific extension
  try_spec '"%I64d"' '"%I64u"' || return 0
  # Is unsigned/signed int for uint64_t/int64_t?
  dspec='"%d"'
  uspec='"%u"'
}
main()
{
  local dspec uspec
  local -r out_h="$1"
  mkdir -p out
  [[ -n "$CC2" ]] || local -r CC2=cc
  local -r start='#include <stdio.h>
#include <stdint.h>
int main(){uint64_t x=7;printf('
  local -r end=',x);return 0;}'
  echo "#ifndef __NSL_CONFIG_H_" > $out_h
  echo "#define __NSL_CONFIG_H_" >> $out_h
  find_specifier
  echo "#define format $dspec" >> $out_h
  echo "#define uformat $uspec" >> $out_h
  echo "#endif" >> $out_h
  rm -f out/tmp1*
}
main "$@"
