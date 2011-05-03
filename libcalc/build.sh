#!/bin/sh

gcc -o testcalc main.c `pkg-config --cflags --libs glib-2.0` -lm -laudcore -lcalc -L./.libs
