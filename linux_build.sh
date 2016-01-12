#!/bin/sh
rm -rf cmakebuild
CMaker project:maraton head:*.h src:*.cpp,*.c
cd cmakebuild
cmake ./
make -j 4
