#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright © 2023 Erez Geva <ErezGeva2@gmail.com>
#
# Follow FSF RESUSE Specification https://reuse.software/spec/
# Provide folders with all licenses
###############################################################################
down_lic()
{
  [[ -f "LICENSES/$1.txt" ]] || reuse download $1
}
main()
{
 [[ -n "$(which reuse)" ]] || return
 mkdir -p LICENSES
 local n
 for n in GPL-3.0-or-later GFDL-1.3-no-invariants-or-later MIT
 do down_lic $n; done
}
cd $(dirname $(realpath $0))
main
