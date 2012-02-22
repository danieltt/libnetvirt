#!/bin/bash
swig -python  src/libnetvirt.i
cp src/libnetvirt.py python/libnetvirt.py
