import os
import sys
import time
import datetime

print "\n\tCall trans opt: received.", \
time.strftime(" %m-%d-%Y"   , time.localtime()), \
time.strftime(" %I:%M:%S %p", time.localtime()), \
" REC:Log>"
print "\t\t---===== Build programe: running =====---\n"

#############  M O C  ##############
os.system('moc -o ./src/moc/moc_ConversionConsole.cpp  ./src/gui/ConversionConsole.hpp')


###########  F L A G S  ############
env = Environment()
valgrind = ARGUMENTS.get('valgrind', 0)
gprof = ARGUMENTS.get('gprof', 0)
release = ARGUMENTS.get('release', 0)

if int(valgrind):
   env.Append(CCFLAGS   = '-O0 -Wall -g')
elif int(gprof):
     env.Append(CCFLAGS   = '-O2 -Wall -pg -g')
     env.Append(LINKFLAGS = '-O2 -Wall -pg -g')
elif int(release):
     env.Append(CCFLAGS   = '-O2 -Wall -s')
else:
     env.Append(CCFLAGS   = '-O2 -Wall --verbose')


###########  F I L E S  ############
core_source = Glob('./src/core/*.cpp')
gui_source  = Glob('./src/gui/*.cpp')
moc_source  = Glob('./src/moc/*.cpp')


#########  C O N F I G S  ##########
env.Program(target='./bin/kolourmatica', source=core_source + gui_source + moc_source)
env.ParseConfig( 'pkg-config --cflags --libs QtGui QtCore' )

