#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: $0 unslashed-folder-name"
    exit
fi

cd $1

g++ -std=c++11 -I./../../include/ -fpic -c $1.cpp
g++ -std=c++11 -shared -o lib$1.so $1.o
rm $1.o

mv lib$1.so ..
