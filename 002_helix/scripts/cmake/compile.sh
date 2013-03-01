#!/bin/bash
d=${PWD}
bd=${d}/build

if [ ! -d ${bd} ] ; then
    mkdir ${bd}
fi

cd ${bd}
cmake ../ 
#make VERBOSE=1
make install
