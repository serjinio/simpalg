# Build script for simgalg library

import os
import shutil
import subprocess


Help("""
Usage:
      'scons release'    build the release version,
      'scons debug'      build the debug version (default),
      'scons test'       run unit tests.
""")

############################################################
# Global environment setup
############################################################

env = Environment(CC='gcc',
                  CFLAGS='-std=c99 -Wall',
                  CPPPATH='#src')

env['ENV']['LD_LIBRARY_PATH'] = os.path.abspath('src')

if 'release' not in COMMAND_LINE_TARGETS:
    env.Append(CCFLAGS = '-g -O0')

############################################################
# Definitions of custom builders
############################################################

def UnitTest(target, source, env):
    app = str(source[0].abspath)
    proc = subprocess.Popen([app], env=env['ENV'])
    proc.wait()
    if proc.returncode == 0:
        print 'UnitTest: Tests completed successfully.'
        open(str(target[0]), 'w').write("PASSED\n")
    else:
        print 'UnitTest: There were errors during unit tests execution!'
        return 1
        
# Create a builder for tests
env.Append(BUILDERS = {'Test': Builder(action = UnitTest)})

############################################################
# Build
############################################################

SConscript('src/Sconscript', exports='env')
if 'test' in COMMAND_LINE_TARGETS:
    test_env = env.Clone()
    SConscript('tests/Sconscript', exports='test_env')

