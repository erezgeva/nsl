#!/bin/bash -e
# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright © 2026 Erez Geva <ErezGeva2@gmail.com>
#
# @author Erez Geva <ErezGeva2@@gmail.com>
# @copyright © 2026 Erez Geva
#
# Script to build doxygen in GitHub
###############################################################################
apt_install()
{
 export DEBIAN_FRONTEND=noninteractive
 sudo apt-get update
 sudo apt-get install -y --no-install-recommends $@
}
main()
{
 apt_install doxygen graphviz pdf2svg astyle
 make doxygen
 rm -Rf doc/*/latex doc/*/man doc/*/html/*.md5
 mv doc _site
}
main "$@"
