#!/bin/sh

mkdir -p makefile_release
cd makefile_release
cmake ../.. -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
cd ..
