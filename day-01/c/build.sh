#!/bin/sh

set -xe

CC="${CXX:-cc}"
PKGS=""
CFLAGS="-Wall -Wextra -std=c11 -pedantic -ggdb"
LIBS=-lm
SRC="src/main.c ../../lib/c/get_input.c"

# $CC $CFLAGS `pkg-config --cflags $PKGS` -o main $SRC $LIBS `pkg-config --libs $PKGS`
$CC $CFLAGS -o main $SRC $LIBS