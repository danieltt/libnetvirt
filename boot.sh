#! /bin/sh -e

# Bootstrap configure system from .ac/.am files
mkdir -p m4
./up_wrap.sh
autoreconf -Wno-portability --install -I `pwd`/config --force


