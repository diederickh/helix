#!/bin/bash
d=${PWD}
if [ -d ${d}/build ] ; then 
    cd ${d}/build
    rm cmake_install.cmake
    rm CMakeCache.txt
    rm -r CMakeFiles
    rm Makefile
fi
