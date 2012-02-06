import os
import sys
import re
from subprocess import Popen, PIPE

import SCons.Scanner.C
import SCons.Tool


# SConstruct file
DEBUG = ARGUMENTS.get('DEBUG', 0)
env = Environment()
if DEBUG:
    env.Append(CPPFLAGS=['-g'])
    env.Append(LINKFLAGS=['-g'])
else:
    env.Append(CPPFLAGS=['-O3'])
arch_64 = re.search(r'x86_64',
        Popen(['uname', '-a'], stdout=PIPE).communicate()[0])
if arch_64:
    print 'This is a 64-bit architecture, but hMetis is 32-bit. '\
        'Forcing 32-bit compilation and linking'
    env.Append(CPPFLAGS=['-m32'])
    env.Append(LINKFLAGS=['-m32'])

from get_libs import get_lib
for key in ['LD_LIBRARY_PATH', 'LIBRARY_PATH']:
    if key in os.environ:
        env.Append(LIBPATH=os.environ[key].split(os.pathsep))
lib = get_lib('libhmetis.a', env['LIBPATH'])
if lib is None:
    print >> sys.stderr, '[error] Library could not be found.  '\
        'Please ensure it is on your LD_LIBRARY_PATH'
    raise SystemExit(-1)
print 'using:', lib
env.Program(['lib_test.cpp', lib])
