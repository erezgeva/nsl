#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright © 2026 Erez Geva <ErezGeva2@gmail.com>
#
# @author Erez Geva <ErezGeva2@@gmail.com>
# @copyright © 2026 Erez Geva
#
###############################################################################
main()
{
  local -r temp_c=tmp.c out_h="$1"
  echo "#ifndef __NSL_CONFIG_H_" > $out_h
  echo "#define __NSL_CONFIG_H_" >> $out_h
  if printf "%s\n%s\nint main(){%s;return 0;}" \
     '#include <stdio.h>' '#include <stdint.h>' \
     'uint64_t x=7;printf("%lu",x);' |\
     cc -pipe -S -x c - -Werror -o $temp_c &> /dev/null;then
    # uint64_t is 'unsigned long'
    echo "#define uformat \"%lu\"" >> $out_h
    # int64_t is 'long'
    echo "#define format \"%ld\"" >> $out_h
  else
    # uint64_t is 'unsigned long long'
    echo "#define uformat \"%llu\"" >> $out_h
    # uint64_t is 'long long'
    echo "#define format \"%lld\"" >> $out_h
  fi
  echo "#endif" >> $out_h
  rm -f $temp_c
}
main "$@"
