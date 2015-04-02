#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT

from optparse import OptionParser
from PIDs import PIDs  

# usage description
usage = """
 Usage: """+sys.argv[0]+""" [options]
 Example: """+sys.argv[0]+""" --name mu+ --pid 16 
 For more help: """+sys.argv[0]+""" --help 
"""

defaultDataPath = os.path.dirname(os.path.abspath(__file__))+'/decayList.txt'
# Option parameters
parser = OptionParser(usage=usage)
parser.add_option("-p", "--pid", dest="pid", #type="int", #action="store", #default=0
                  help="Input particle ID")
parser.add_option("-n", "--name", dest="name", 
                  help="Input particle name based on pythia table")
parser.add_option("-d", "--data", dest="data", default=defaultDataPath,
                  help="Input particle decay list from pythia")
(options, args) = parser.parse_args()

# Main progress
if not options.pid and not options.name:
	print usage
	sys.exit()
pids = PIDs(options.data)
pids.loadDecayList()
if options.pid:
	print ">> Paritcle ID   : "+str(options.pid)+" = "+pids.showName(options.pid)
if options.name:
	print ">> Paritcle name : "+pids.correctName(options.name)+" = "+str(pids.showPID(options.name))
	

#print "|"
#if len(sys.argv)<2:
#	print "| Please give a PID"
#	print "|  e.x. "+sys.argv[0]+" -- 12"
#	print "|"
#else:
#	pids = PIDs("decayList.txt")
#	pids.loadDecayList()	
#	print "| Paritcle ID "+sys.argv[1]+" = "+pids.showName(sys.argv[1])
#	print "|"
