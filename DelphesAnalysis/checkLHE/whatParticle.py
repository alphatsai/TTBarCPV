#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

from optparse import OptionParser
from PIDs import PIDs  

# usage description
usage = """
 Usage: """+sys.argv[0]+""" [options]
 Example: """+sys.argv[0]+""" -p 12
 For more help: """+sys.argv[0]+""" --help 
"""

# Option parameters
parser = OptionParser(usage=usage)
parser.add_option("-p", "--pid", dest="pid", type="int", #action="store", #default=0
                  help="Input particle ID")
parser.add_option("-d", "--data", dest="data", default='decayList.txt',
                  help="Input particle decay list from pythia")
(options, args) = parser.parse_args()

# Main progress
if not options.pid:
	print usage
	sys.exit()
else:
	pids = PIDs(options.data)
	pids.loadDecayList()
	print "|"
	print "| Paritcle ID "+str(options.pid)+" = "+pids.printParticle(options.pid)
	print "|"


#print "|"
#if len(sys.argv)<2:
#	print "| Please give a PID"
#	print "|  e.x. "+sys.argv[0]+" -- 12"
#	print "|"
#else:
#	pids = PIDs("decayList.txt")
#	pids.loadDecayList()	
#	print "| Paritcle ID "+sys.argv[1]+" = "+pids.printParticle(sys.argv[1])
#	print "|"
