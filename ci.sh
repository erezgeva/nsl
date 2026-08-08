#!/bin/bash -e
# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright © 2026 Erez Geva <ErezGeva2@gmail.com>
#
# @author Erez Geva <ErezGeva2@@gmail.com>
# @copyright © 2026 Erez Geva
#
# CI (Continuous integration) script for containers
###############################################################################
distribution()
{
 if [[ -f /etc/debian_version ]]; then
   # Also Ubuntu and other Debian based distributions
   dist=debian
 else
   for n in /etc/*-release; do
     if [[ -f "$n" ]]; then
       case $n in
         /etc/os-release) ;;
         /etc/system-release) ;;
         *)
            m="${n%-release}"
            dist="${m#/etc/}"
            break
            ;;
       esac
     fi
   done
 fi
}
###############################################################################
eecmd()
{
 set +e
 out=$(eval "$@" 2>&1)
 last_ret=$?
 set -e
}
mquit()
{
 if [[ -n "$out" ]]; then
   echo "$out"
 fi
 echo "$color_red$color_red$@$color_norm"
 exit 1
}
###############################################################################
cmp_cmp()
{
 echo "====== compare compilations ======"
 local -r baseOut="$(eval out/cmp_c_ansi)"
 for n in out/cmp_*; do
   if [[ -f "$n" ]] && [[ -x "$n" ]]; then
     curOut="$(eval "$n")"
     eecmd "diff <(printf \"$baseOut\") <(printf \"$curOut\")"
     if [[ -n "$out" ]]; then
       mquit "** $n fails"
     fi
   fi
 done
}
###############################################################################
main()
{
 local n m nocolor out dist curOut
 local -i last_ret jobs=1 # Number of Make parallel jobs
 while getopts 'oj:' opt; do
   case $opt in
     o)
       nocolor=yes
       ;;
     j)
       jobs="0 + $OPTARG"
       ;;
   esac
 done
 if [[ $jobs -le 0 ]]; then
   jobs=1
 fi
 # Make sure we output to STDOUT directly, no pipes
 # check our teminal support coulors
 if [[ -z "$nocolor" ]] && [[ -t 1 ]] && tput setaf 1; then
   local -r esc=`printf '\e['`
   local -r color_red=${esc}31m
   local -r color_blue=${esc}34m
   local -r color_norm=${esc}00m
   printf "$color_norm"
 fi
 case $(uname) in
   Linux)
     make -j$jobs all utest
     ./utest
     cmp_cmp
     distribution
     case $dist in
       debian)
         make deb
         ;;
       fedora|redhat)
         make rpm
         ;;
       arch)
         make pkg
         ;;
     esac
     ;;
   Darwin)
     local -i i=20
     until which gcc-$i > /dev/null;do i+=-1;done
     make -j$jobs CC=gcc-$i CXX=g++-$i
     cmp_cmp
     ;;
   *)
     echo "uname =|$(uname)|"
     ;;
 esac
}
main "$@"
