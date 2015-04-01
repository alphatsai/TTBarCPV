#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

from optparse import OptionParser
#from PIDs import PIDs  
from readLHE import readLHE 

# usage description
usage = """
 Usage: """+sys.argv[0]+""" [options]
 Example: """+sys.argv[0]+""" --max 12 --lhe unweighted_events.lhe
 For more help: """+sys.argv[0]+""" --help 
"""

# Option parameters
parser = OptionParser(usage=usage)
parser.add_option("-m", "--max", dest="maxEvt", type="int", 
                  help="Show events until maximum event")
parser.add_option("-e", "--evt", dest="evt", type="int", 
                  help="Show spacial event")
parser.add_option("-d", "--data", dest="data", default='decayList.txt',
                  help="Input particle decay list from pythia")
parser.add_option("-l", "--lhe", dest="lheFile", 
                  help="Input lhe file")
(options, args) = parser.parse_args()

# Main progress
if not options.lheFile:
	print usage
	sys.exit()
if not options.maxEvt and not options.evt:
	print usage
	sys.exit()
elif options.maxEvt and not options.evt:
	lheInfo = readLHE( options.data, options.lheFile )
	lheInfo.loadLHE()
	lheInfo.loadEvents(options.maxEvt)
	#lheInfo.loadEvents()  # Without set limit, it will load all events
	lheInfo.showEvent(options.maxEvt)	
elif not options.maxEvt and options.evt:
	lheInfo = readLHE( options.data, options.lheFile )
	lheInfo.loadLHE()
	lheInfo.loadSpEvents(options.evt) # Only load interested event 
	#lheInfo.loadEvents()  # Without set limit, it will load all events
	lheInfo.showSpEvent(options.evt)	
else:
	print " Shell not use --max and --evt at the same time"
	print usage
	sys.exit()
	
