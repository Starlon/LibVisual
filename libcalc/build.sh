#!/bin/sh

gcc -o testcalc main.c libcalc.a `pkg-config --cflags --libs glib-2.0` -lm -laudcore
