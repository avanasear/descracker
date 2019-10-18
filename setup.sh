#!/bin/bash
echo '1'
os_str=`uname -v`
echo $os_str
if `echo $os_str | grep -q Ubuntu`; then
  pack_man='apt install'
  echo $pack_man
elif `echo $os_str | grep -q Arch`; then
  pack_man='pacman -Syu'
  echo $pack_man
fi
