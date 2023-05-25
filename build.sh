#!/bin/sh -e

clang-9 src/getline.c -o externals/getline.l_arm -fPIC -lc -lm -rdynamic -shared -Wl,-rpath,"\$$ORIGIN",--enable-new-dtags
