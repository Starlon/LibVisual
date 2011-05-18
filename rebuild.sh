#!/bin/sh

gcc -o main `pkg-config --libs --cflags freetype2` -lm main.c
