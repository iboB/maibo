#!/bin/sh

mkdir -p makefile
cd makefile
cmake ../.. -G "Unix Makefiles"
cd ..
