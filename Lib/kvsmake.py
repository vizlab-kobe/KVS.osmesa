#!/usr/bin/python

import sys
import os

LIB_NAME = "KVS.osmesa"

#=============================================================================
#  Executes kvsmake command.
#=============================================================================
def KVSMake( option ):

    s = ";"
    if os.name == 'nt': s = "&"

    make_option = ''
    if option == 'build': make_option = "kvsmake lib" + s
    elif option == 'clean': make_option = "kvsmake clean" + s
    elif option == 'distclean': make_option = "kvsmake distclean" + s
    elif option == 'rebuild': make_option = "kvsmake clean" + s + "kvsmake lib" + s
    elif option == 'debug_build': make_option = "kvsmake lib DEBUG=1" + s
    elif option == 'debug_rebuild': make_option = "kvsmake clean DEBUG=1" + s + "kvsmake lib DEBUG=1" + s
    else:
        print( "Error: Unknown option '" + option +"'" )
        print( "Usage: python kvsmake.py [clean | distclean | rebuild]" )
        sys.exit()

    command = ''
    command += "kvsmake -g " + LIB_NAME + s
    command += make_option
    os.system( command )

#=============================================================================
#  Main process.
#=============================================================================
if __name__=='__main__':
    argc = len( sys.argv )
    argv = sys.argv
    if   argc == 1: KVSMake( 'build' )
    elif argc == 2: KVSMake( argv[1] )
