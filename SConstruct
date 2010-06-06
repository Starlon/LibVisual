import subprocess
import sys

output = " -I v8/include/"

env = Environment(CCFLAGS = Split("-g -Wall " + output))

output = " v8/libv8_g.a"

libs = env.ParseFlags(Split(output));
env.MergeFlags(libs)

plugin = env.StaticLibrary(["visscript_v8.cpp"]);



