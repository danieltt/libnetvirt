#! /bin/sh -e

# Bootstrap configure system from .ac/.am files
autoreconf -Wno-portability --install -I `pwd`/config --force
./up_wrap.sh

