import os
import sys
import time
import datetime

abort_build_msg = "Build program: aborting"

#################  I N T R O D U C T I O N  ##################
print "\n\tCall trans opt: received.",                       \
time.strftime(" %m-%d-%Y", time.localtime()),                \
time.strftime(" %H:%M:%S", time.localtime()), " REC:Log>"
print "\t\t\tK O L O U R M A T I C A"
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
print "\t  moc args: processed.  " + moc,
if moc not in ['on', 'off']:
   print "\t\t RTN:Error>" + "\n\t" + abort_build_msg
   Exit(1)
elif moc == 'on':
   if not os.path.exists('./build/moc'):
      os.makedirs('./build/moc')
   moc_msg = "meta object code: generated."
   print "\n\tBuild program: compiling meta objects"

   os.system('moc -o ./build/moc/moc_ConversionConsole.cpp  ./src/gui/ConversionConsole.hpp')
   print "\t  " + moc_msg + "  ./build/moc/moc_ConversionConsole.cpp"

   os.system('moc -o ./build/moc/moc_View.cpp  ./src/gui/view/View.hpp')
   print "\t  " + moc_msg + "  ./build/moc/moc_View.cpp"
else:
   print



#################         B U I L D         ##################
print "\tBuild program: building\n\n"
VariantDir('build', 'src', duplicate = 0)
env = Environment(CPPPATH = ['#src/core', '#src/gui', '#src/gui/view'])


if mode == 'release':
   env.Append(CCFLAGS = '-O2 -Wall -s -DQT_NO_DEBUG_OUTPUT')
   env.Append(LINKFLAGS   = '-lboost_thread')
elif mode == 'debug':
   env.Append(CCFLAGS = '-O2 -Wall -s')
   env.Append(LINKFLAGS   = '-lboost_thread')
elif mode == 'valgrind':
   env.Append(CCFLAGS = '-O0 -Wall -g')
   env.Append(LINKFLAGS   = '-lboost_thread')
elif mode == 'gprof':
   env.Append(CCFLAGS   = '-O2 -Wall -pg -g')
   env.Append(LINKFLAGS = '-O2 -Wall -pg -g -lboost_thread')

core_source = Glob('./build/core/*.cpp')
gui_source  = Glob('./build/gui/*.cpp') + Glob('./build/gui/view/*.cpp')
moc_source  = Glob('./build/moc/*.cpp')

env.Program(target='./bin/kolourmatica', source=core_source + gui_source + moc_source)
env.ParseConfig( 'pkg-config --cflags --libs QtGui QtCore eigen3' )

