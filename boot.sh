#! /bin/sh -e

# Bootstrap configure system from .ac/.am files
mkdir m4
autoreconf -Wno-portability --install -I `pwd`/config --force
./up_wrap.sh

