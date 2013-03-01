#!/bin/bash
d=${PWD}
bd=${d}/build

if [ ! -d ${bd} ] ; then
    mkdir ${bd}
fi

cd ${bd}
cmake -G "Xcode" ../
