import subprocess
import sys

visualCflags, visualCflagse = subprocess.Popen(["pkg-config", "--cflags", "libvisual-0.5"],
    stderr=subprocess.PIPE, stdout=subprocess.PIPE).communicate()

output = visualCflags + " -I v8/include/"

env = Environment(CCFLAGS = Split("-g -Wall " + output))

visualLdflags, visualLdflagse = subprocess.Popen(["pkg-config", "--libs", "libvisual-0.5"],
    stderr=subprocess.PIPE, stdout=subprocess.PIPE).communicate()

output = visualLdflags + " v8/libv8.a"

libs = env.ParseFlags(Split(output));
env.MergeFlags(libs)

plugin = env.SharedLibrary(["script_visscript_v8.cpp"]);

env.Install("/usr/lib/libvisual-0.5/script/", plugin);
env.Alias('install', '/usr/lib/libvisual-0.5/script')


