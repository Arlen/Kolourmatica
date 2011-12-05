import os
import sys
import time
import datetime

abort_build_msg = "Build program: aborting"

#################  I N T R O D U C T I O N  ##################
print "\n\tCall trans opt: received.",                       \
time.strftime(" %m-%d-%Y", time.localtime()),                \
time.strftime(" %H:%M:%S", time.localtime()), " REC:Log>"
print "\n\t\t\tK O L O U R M A T I C A\n"
print "\tBuild program: running"



#################    B U I L D . M O D E    ##################
mode = ARGUMENTS.get('mode', 'release')
print "\t  build args: processed.  " + mode,
if mode not in ['release', 'debug', 'valgrind', 'gprof']:
   print "\t\t RTN:Error>" + "\n\t" + abort_build_msg
   Exit(1)
else:
   print



#################           M O C           ##################
moc = ARGUMENTS.get('moc', 'on')
print "\t  moc args: processed.  " + moc

moc_cmd = ''

if os.path.abspath('moc-qt4'):
   moc_cmd = 'moc-qt4'
elif os.path.abspath('moc'):
   moc_cmd = 'moc'

if moc not in ['on', 'off']:
   print "\t\t RTN:Error>" + "\n\t" + abort_build_msg
   Exit(1)
elif moc == 'on':
   if not os.path.exists('./build/moc'):
      os.makedirs('./build/moc')
   moc_msg = "meta object code: generated."
   print "\n\tBuild program: compiling meta objects"

   os.system(moc_cmd + ' -o ./build/moc/moc_Console.cpp  ./src/gui/Console.hpp')
   print "\t  " + moc_msg + "  ./build/moc/moc_Console.cpp"

   os.system(moc_cmd + ' -o ./build/moc/moc_View.cpp  ./src/gui/View.hpp')
   print "\t  " + moc_msg + "  ./build/moc/moc_View.cpp"
else:
   print



#################         B U I L D         ##################
print "\tBuild program: building\n\n"
VariantDir('build', 'src', duplicate = 0)
env = Environment(CPPPATH = ['#src/core', '#src/gui'])


if mode == 'release':
   env.Append(CCFLAGS = '-std=c++0x -O2 -Wall -s -DQT_NO_DEBUG_OUTPUT -DQT_NO_WARNING_OUTPUT')
elif mode == 'debug':
   env.Append(CCFLAGS = '-std=c++0x -O0 -Wall -s -g')
elif mode == 'valgrind':
   env.Append(CCFLAGS = '-std=c++0x -O0 -Wall -pg')
elif mode == 'gprof':
   env.Append(CCFLAGS = '-O2 -Wall -pg -g')

core_source = Glob('./build/core/*.cpp')
gui_source  = Glob('./build/gui/*.cpp')
moc_source  = Glob('./build/moc/*.cpp')

env.Program(target='./bin/kolourmatica', source=core_source + gui_source + moc_source)
env.ParseConfig( 'pkg-config --cflags --libs QtGui QtCore eigen3' )

